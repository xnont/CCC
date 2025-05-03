#include "ccc/execution.h"
#include "util/io.h"

ccc::execution::execution(std::string name, std::string description)
    : ccc::compile_task(name, description) {

    this->toolchain.compile_format = this->toolchain.execution_compile_format;
    this->toolchain.link_format = this->toolchain.execution_link_format;
};

void ccc::execution::link(const ccc::config& project_cfg) {
    // If the output_path doesn't exist, create it.
    std::string target_folder =
        extractPath(this->output_path + "/" + this->name);
    if (!(fs::exists(target_folder) && fs::is_directory(target_folder))) {
        fs::create_directories(target_folder);
    }

    auto replacements =
        std::unordered_map<std::string, std::vector<std::string>>{
            {"LINKER",
             {!this->config.linker.empty()  ? this->config.linker
              : !project_cfg.linker.empty() ? project_cfg.linker
                                            : "g++"}},
            {"OBJECT_FILES", {this->obj_files.begin(), this->obj_files.end()}},
            {"OUTPUT_FILE",
             {(this->output_path.empty() ? "./build/bin" : this->output_path) +
              "/" + this->name}},
            {"LINK_FLAGS",
             {this->config.link_flags.begin(), this->config.link_flags.end()}}};

    std::string cmd = this->toolchain.link_format.replace(replacements);

    // Link
    ccc::io::exec_command(cmd, project_cfg.is_print && this->config.is_print,
                          project_cfg.is_print && this->config.is_print);
}

#define RED "\033[31m"
#define ORANGE "\033[38;5;202m"
#define RESET "\033[0m"
bool ccc::execution::check(const ccc::config& project_cfg) {
    bool status = true;

    // Check if compiler is specified
    if (this->config.compiler.length() == 0 &&
        project_cfg.compiler.length() == 0) {
        std::cout << RED << "Error" << RESET
                  << ": No compiler specified for execution " << this->name
                  << std::endl;
        status = false;
    }

    // Check if linker is specified
    if (this->config.linker.length() == 0 && project_cfg.linker.length() == 0) {
        std::cout << RED << "Error" << RESET
                  << ": No linker specified for execution " << this->name
                  << std::endl;
        status = false;
    }

    // Check if there is any source file
    if (this->source_files.size() == 0) {
        std::cout << ORANGE << "Warning" << RESET
                  << ": There isn't any source file in the execution "
                  << this->name << std::endl;
    }

    return status;
}