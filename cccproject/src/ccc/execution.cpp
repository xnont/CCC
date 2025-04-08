#include "ccc/execution.h"

ccc::execution::execution(std::string name, std::string description)
    : ccc::compile_task(name, description) {};

void ccc::execution::process(const ccc::config& project_cfg) {
    this->compile(project_cfg);

    // If the output_path doesn't exist, create it.
    std::string target_folder =
        extractPath(this->output_path + "/" + this->name);
    if (!(fs::exists(target_folder) && fs::is_directory(target_folder))) {
        fs::create_directories(target_folder);
    }

    std::string cmd = (
        // linker
        (this->config.linker.length() != 0 ? this->config.linker
                                           : project_cfg.linker) +
        " " +
        // Object files
        joinWithSpace(this->obj_files) + " -o " +
        // Output file
        (this->output_path.length() != 0 ? this->output_path : "./build/bin") +
        "/" + this->name + " " +
        // Linker flags from project
        joinWithSpace(project_cfg.link_flags) + " " +
        // Linker flags from execution
        joinWithSpace(this->config.link_flags));

    // Link
    std::cout << cmd << std::endl;
    if (std::system(cmd.c_str()) != 0)
        return;
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