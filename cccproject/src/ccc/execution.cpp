#include "ccc/execution.h"
#include "util/io.h"

ccc::execution::execution(std::string name, std::string description)
    : ccc::compile_task(name, description) {}

void ccc::execution::set_toolchain(const ccc::config& project_cfg) {
    // Set toolchain
    this->config.toolchain =
        // If the toolchain of the execution is not empty, use it.
        !this->config.toolchain.is_empty() ? this->config.toolchain

        // If the toolchain of the project is not empty, use it.
        : !project_cfg.toolchain.is_empty()
            ? project_cfg.toolchain

            // Use the built-in toolchain.
            : ccc::built_in_toolchain::gnu_toolchain();

    // Set the compile foramt and the link format
    this->config.toolchain.compile_format =
        this->config.toolchain.execution_compile_format;
    this->config.toolchain.link_format =
        this->config.toolchain.execution_link_format;
}

void ccc::execution::link(const ccc::config& project_cfg) {
    // If the output_path doesn't exist, create it.
    std::string target_folder =
        extractPath(this->output_path + "/" + this->name);
    if (!(fs::exists(target_folder) && fs::is_directory(target_folder))) {
        fs::create_directories(target_folder);
    }

    // For using clang or msvc on Windows, the dynamic library suffix needs to
    // be changed to '. lib'.
    if (this->config.toolchain ==
            built_in_toolchain::clang_toolchain(ccc::system_type::windows_os) ||
        this->config.toolchain ==
            built_in_toolchain::msvc_toolchain(ccc::system_type::windows_os)) {
        for (auto& file : this->obj_files) {
            if (file.size() >= 4 && file.substr(file.size() - 4) == ".dll") {
                file = file.substr(0, file.size() - 4) + ".lib";
            }
        }
    }

    // Add the suffix '.exe' when the target os is windows.
    if (this->config.toolchain.target_os == system_type::windows_os &&
        this->name.find(".exe") == std::string::npos) {
        this->name += ".exe";
    }

    auto replacements =
        std::unordered_map<std::string, std::vector<std::string>>{
            {"LINKER",
             {!this->config.toolchain.linker.empty()
                  ? this->config.toolchain.linker
              : !project_cfg.toolchain.linker.empty()
                  ? project_cfg.toolchain.linker
                  : "g++"}},
            {"OBJECT_FILES", {this->obj_files.begin(), this->obj_files.end()}},
            {"OUTPUT_FILE",
             {(this->output_path.empty() ? "./build/bin" : this->output_path) +
              "/" + this->name}},
            {"LINK_FLAGS",
             {this->config.link_flags.begin(), this->config.link_flags.end()}}};

    std::string cmd = this->config.toolchain.link_format.replace(replacements);

    // Link
    ccc::io::exec_command(cmd, project_cfg.is_print && this->config.is_print,
                          project_cfg.is_print && this->config.is_print);
}

#define RED "\033[31m"
#define ORANGE "\033[38;5;202m"
#define RESET "\033[0m"
bool ccc::execution::check(const ccc::config& project_cfg) {
    if (project_cfg.toolchain.is_empty())
        return true;
    return true;
}