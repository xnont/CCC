#ifndef __CCC_FORMAT_H__
#define __CCC_FORMAT_H__

#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

namespace ccc {
class Format {
  public:
    std::string format;

    Format() = default;

    Format(const std::string& format) { this->format = format; }

    Format(const Format& other) { format = other.format; };

    Format& operator=(const Format&) = default;

    bool operator==(const Format& other) const {
        return format == other.format;
    }

    std::string
    replace(std::unordered_map<std::string, std::vector<std::string>>& mp) {
        std::string ret = format;

        // First, match all templates that are wrapped by {}
        std::regex brace_pattern(R"(\{(.*?)\})");
        std::smatch brace_match;

        while (std::regex_search(ret, brace_match, brace_pattern)) {
            std::string segment = brace_match[1];
            std::string replacement;

            // Separate prefixe, variable name, and suffixe
            std::regex var_pattern(R"((.*?)\$\((\w+)\)(.*))");
            std::smatch var_match;

            if (std::regex_match(segment, var_match, var_pattern)) {
                std::string prefix = var_match[1];
                std::string key = var_match[2];
                std::string suffix = var_match[3];

                if (mp.count(key)) {
                    for (const auto& val : mp[key]) {
                        if (!replacement.empty())
                            replacement += " "; // Maintain space separation
                                                // between parameters
                        replacement += prefix + val + suffix;
                    }
                }
            }

            ret.replace(brace_match.position(), brace_match.length(),
                        replacement);
        }

        // Handling ordinary variable replacement
        for (auto& [key, values] : mp) {
            std::string replacement = "";
            if (!values.empty()) {
                for (const auto& val : values) {
                    if (!replacement.empty())
                        replacement += " ";
                    replacement += val;
                }
            }
            ret = std::regex_replace(
                ret, std::regex(R"(\$\(()" + key + R"()\))"), replacement);
        }

        return ret;
    }
};

class toolchain {
  public:
    std::string name;

    /* The compiler */
    std::string compiler;
    /* The linker */
    std::string linker;

    ccc::Format compile_format;
    ccc::Format execution_compile_format;
    ccc::Format static_library_compile_format;
    ccc::Format shared_library_compile_format;

    ccc::Format link_format;
    ccc::Format execution_link_format;
    ccc::Format static_library_link_format;
    ccc::Format shared_library_link_format;

    toolchain() = default;

    toolchain(const std::string& name, const std::string& compiler,
              const std::string& linker,

              const ccc::Format& compile_format,
              const ccc::Format& execution_compile_format,
              const ccc::Format& static_library_compile_format,
              const ccc::Format& shared_library_compile_format,

              const ccc::Format& link_format,
              const ccc::Format& execution_link_format,
              const ccc::Format& static_library_link_format,
              const ccc::Format& shared_library_link_format)
        : name(name), compiler(compiler), linker(linker),

          compile_format(compile_format),
          execution_compile_format(execution_compile_format),
          static_library_compile_format(static_library_compile_format),
          shared_library_compile_format(shared_library_compile_format),

          link_format(link_format),
          execution_link_format(execution_link_format),
          static_library_link_format(static_library_link_format),
          shared_library_link_format(shared_library_link_format) {}

    toolchain(const toolchain& other) = default;

    bool operator==(const toolchain& other) const {
        return this->name == other.name;
    }

    bool operator!=(const toolchain& other) const { return !(*this == other); }

    bool is_empty() const {
        return compiler.empty() && linker.empty() &&
               compile_format.format.empty() &&
               execution_compile_format.format.empty() &&
               static_library_compile_format.format.empty() &&
               shared_library_compile_format.format.empty() &&

               link_format.format.empty() &&
               execution_link_format.format.empty() &&
               static_library_link_format.format.empty() &&
               shared_library_link_format.format.empty();
    }
};

namespace built_in_toolchain {
inline auto gnu_toolchain = ccc::toolchain(
    "gnu", "g++", "g++",

    ccc::Format(""),
    ccc::Format("$(COMPILER) -c $(SOURCE_FILE) -o "
                "$(OBJECT_FILE) $(COMPILE_FLAGS) {-I$(HEADER_FOLDERS)} "
                "{-D$(MACROS)} -fdiagnostics-color=always 2>&1"),
    ccc::Format("$(COMPILER) -c $(SOURCE_FILE) -o "
                "$(OBJECT_FILE) $(COMPILE_FLAGS) {-I$(HEADER_FOLDERS)} "
                "{-D$(MACROS)} -fdiagnostics-color=always 2>&1"),
    ccc::Format("$(COMPILER) -c $(SOURCE_FILE) -o "
                "$(OBJECT_FILE) $(COMPILE_FLAGS) -fPIC {-I$(HEADER_FOLDERS)} "
                "{-D$(MACROS)} -fdiagnostics-color=always 2>&1"),

    ccc::Format(""),
    ccc::Format("$(LINKER) {$(OBJECT_FILES)} -o $(OUTPUT_FILE) {$(LINK_FLAGS)} "
                "-fdiagnostics-color=always 2>&1"),
    ccc::Format("ar rcs $(OUTPUT_FILE) $(OBJECT_FILES)"),
    ccc::Format(
        "$(LINKER) {$(OBJECT_FILES)} -o $(OUTPUT_FILE) {$(LINK_FLAGS)} -shared "
        "-fdiagnostics-color=always 2>&1"));

inline auto clang_toolchain = []() {
    auto tc = built_in_toolchain::gnu_toolchain;
    tc.name = "clang";
    tc.compiler = "clang++";
    tc.linker = "clang++";
    tc.static_library_link_format =
        ccc::Format("llvm-ar rcs $(OUTPUT_FILE) $(OBJECT_FILES)");

#ifdef _WIN32
    tc.shared_library_link_format = tc.static_library_compile_format;
#endif

    return tc;
}();
} // namespace built_in_toolchain

} // namespace ccc

#endif