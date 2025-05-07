#include "ccc/library.h"

#include "util/file.hpp"
#include "util/io.h"

ccc::library::library(std::string name, ccc::library_type type,
                      std::string description)
    : ccc::compile_task(name, description) {
    // Set the type
    this->type = type;
}

void ccc::library::set_toolchain(const ccc::config& project_cfg) {
    // Set toolchain
    this->config.toolchain =
        // If the toolchain of the library is not empty, use it.
        !this->config.toolchain.is_empty() ? this->config.toolchain

        // If the toolchain of the project is not empty, use it.
        : !project_cfg.toolchain.is_empty()
            ? project_cfg.toolchain

            // Use the built-in toolchain.
            : ccc::built_in_toolchain::gnu_toolchain();

    // Set the compile foramt and the link format
    if (this->type == library_type::static_library) {
        this->config.toolchain.compile_format =
            this->config.toolchain.static_library_compile_format;
        this->config.toolchain.link_format =
            this->config.toolchain.static_library_link_format;
    } else if (this->type == library_type::shared_library ||
               this->type == library_type::dynamic_library) {
        this->config.toolchain.compile_format =
            this->config.toolchain.shared_library_compile_format;
        this->config.toolchain.link_format =
            this->config.toolchain.shared_library_link_format;
    }

    // If the given library name does not have a suffix, add the prefix and
    // suffix according to the operating system.
    if (this->name.find(".") == std::string::npos) {
        // Set the library name prefix and suffix according to the target
        // operating system. Windows
        if (this->config.toolchain.target_os == ccc::system_type::windows_os) {
            if (this->type == library_type::static_library) {
                this->name = "lib" + this->name + ".lib";
            } else if (this->type == library_type::dynamic_library ||
                       this->type == library_type::shared_library) {
                this->name = this->name + ".dll";
            }
        }
        // Linux
        else if (this->config.toolchain.target_os ==
                 ccc::system_type::linux_os) {
            if (this->type == library_type::static_library) {
                this->name = "lib" + this->name + ".a";
            } else if (this->type == library_type::dynamic_library ||
                       this->type == library_type::shared_library) {
                this->name = "lib" + this->name + ".so";
            }
        }
    }
}

void ccc::library::link(const ccc::config& project_cfg) {
    // Merge the project's link flags with the library's link flags.
    this->config.link_flags.insert(this->config.link_flags.begin(),
                                   project_cfg.link_flags.begin(),
                                   project_cfg.link_flags.end());

    // If the output_path doesn't exist, create it.
    std::string target_folder =
        extractPath(this->output_path + "/" + this->name);
    if (!(fs::exists(target_folder) && fs::is_directory(target_folder))) {
        fs::create_directories(target_folder);
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
             {(this->output_path.empty() ? "./build/lib" : this->output_path) +
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
bool ccc::library::check(const ccc::config& project_cfg) {
    if (project_cfg.toolchain.is_empty())
        return true;
    return true;
}