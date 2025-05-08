#include "ccc/toolchain.h"

#include <iostream>

namespace ccc {
namespace built_in_toolchain {

ccc::toolchain gnu_toolchain(ccc::system_type target_os) {
    // Check target OS
    if (target_os != system_type::windows_os &&
        target_os != system_type::linux_os) {
        std::cerr << "Error: Unsupported target OS for gnu toolchain."
                  << std::endl;
        exit(1);
    }

    return ccc::toolchain(
        target_os, "gnu",

        "g++", "g++",

        ccc::Format(""),
        ccc::Format("$(COMPILER) -c $(SOURCE_FILE) -o "
                    "$(OBJECT_FILE) $(COMPILE_FLAGS) {-I$(HEADER_FOLDERS)} "
                    "{-D$(MACROS)} -fdiagnostics-color=always 2>&1"),
        ccc::Format("$(COMPILER) -c $(SOURCE_FILE) -o "
                    "$(OBJECT_FILE) $(COMPILE_FLAGS) {-I$(HEADER_FOLDERS)} "
                    "{-D$(MACROS)} -fdiagnostics-color=always 2>&1"),
        ccc::Format(
            "$(COMPILER) -c $(SOURCE_FILE) -o "
            "$(OBJECT_FILE) $(COMPILE_FLAGS) -fPIC {-I$(HEADER_FOLDERS)} "
            "{-D$(MACROS)} -fdiagnostics-color=always 2>&1"),

        ccc::Format(""),
        ccc::Format(
            "$(LINKER) {$(OBJECT_FILES)} -o $(OUTPUT_FILE) {$(LINK_FLAGS)} "
            "-fdiagnostics-color=always 2>&1"),
        ccc::Format("ar rcs $(OUTPUT_FILE) $(OBJECT_FILES)"),
        ccc::Format("$(LINKER) {$(OBJECT_FILES)} -o $(OUTPUT_FILE) "
                    "{$(LINK_FLAGS)} -shared "
                    "-fdiagnostics-color=always 2>&1"));
}

ccc::toolchain clang_toolchain(ccc::system_type target_os) {
    // Check target OS
    if (target_os != system_type::windows_os &&
        target_os != system_type::linux_os) {
        std::cerr << "Error: Unsupported target OS for gnu toolchain."
                  << std::endl;
        exit(1);
    }

    auto tc = built_in_toolchain::gnu_toolchain(target_os);
    tc.name = "clang";
    tc.compiler = "clang++";
    tc.linker = "clang++";
    tc.static_library_link_format =
        ccc::Format("llvm-ar rcs $(OUTPUT_FILE) $(OBJECT_FILES)");

    if (target_os == ccc::windows_os) {
        // Clang on Windows does not support the - fPIC option.
        tc.shared_library_compile_format = tc.static_library_compile_format;
    }

    return tc;
};

ccc::toolchain msvc_toolchain(ccc::system_type target_os) {
    // Check target OS
    if (target_os != system_type::windows_os) {
        std::cerr << "Error: The MSVC toolchain only supports use under the "
                     "Windows operating system."
                  << std::endl;
        exit(1);
    }

    return ccc::toolchain(
        target_os, "msvc",

        "", "",

        ccc::Format(""), // compile_format
        ccc::Format("cl /c $(SOURCE_FILE) /Fo$(OBJECT_FILE) "
                    "$(COMPILE_FLAGS) {/I$(HEADER_FOLDERS)} "
                    "{/D{MACROS}"), // execution_compile_format
        ccc::Format("cl /c $(SOURCE_FILE) /Fo$(OBJECT_FILE) "
                    "$(COMPILE_FLAGS) {/I$(HEADER_FOLDERS)} "
                    "{/D{MACROS}"), // static_library_compile_format
        ccc::Format("cl /c $(SOURCE_FILE) /Fo$(OBJECT_FILE) "
                    "$(COMPILE_FLAGS) {/I$(HEADER_FOLDERS)} "
                    "{/D{MACROS}"), // shared_library_compile_format

        ccc::Format(""), // link_format
        ccc::Format("cl /Fe$(OUTPUT_FILE) $(OBJECT_FILES) "
                    "$(LINK_FLAGS)"), // execution_link_format
        ccc::Format("lib /OUT:$(OUTPUT_FILE) $(OBJECT_FILES) "
                    "$(LINK_FLAGS)"), // static_library_link_format
        ccc::Format("link /DLL /OUT:$(OUTPUT_FILE) {$(OBJECT_FILES)} "
                    "$(LINK_FLAGS)") // shared_library_link_format
    );
}

} // namespace built_in_toolchain
} // namespace ccc