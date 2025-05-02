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
    Format(const std::string& format) { this->format = format; }

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
            if (!values.empty()) {
                std::string replacement;
                for (const auto& val : values) {
                    if (!replacement.empty())
                        replacement += " ";
                    replacement += val;
                }
                ret = std::regex_replace(
                    ret, std::regex(R"(\$\(()" + key + R"()\))"), replacement);
            }
        }

        return ret;
    }
};

namespace built_in_format {
inline auto gnu_compile_format =
    ccc::Format("$(COMPILER) -c $(SOURCE_FILE) -o "
                "$(OBJECT_FILE) $(COMPILER_FLAGS) {-I$(HEADER_FOLDER)} "
                "{-D$(MACRO)} -fdiagnostics-color=always 2>&1");

inline auto gnu_link_format =
    ccc::Format("$(LINKER) $(OBJECT_FILES) -o $(OUTPUT_FILE) $(LINK_FLAGS) "
                "-fdiagnostics-color=always 2>&1");
} // namespace built_in_format

} // namespace ccc

#endif