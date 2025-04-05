#include "ccc/library.h"

ccc::library::library(std::string name, ccc::library_type type,
                      std::string description)
    : ccc::compile_task(name, description) {
    this->type = type;

    if (this->type == library_type::static_library) {
        this->config.linker = "ar rcs";
    } else if (this->type == library_type::dynamic_library ||
               this->type == library_type::shared_library) {
        this->config.linker = "g++";
    }
}

void ccc::library::process(const ccc::config& project_cfg) {

    // If the library is a shared library or a dynamic library, add -fPIC to the
    // compile flags and add -shared to the link flags.
    if (this->type == library_type::shared_library ||
        this->type == library_type::dynamic_library) {
        this->config.compile_flags.push_back("-fPIC");
        this->config.link_flags.push_back("-shared");
    }

    this->compile(project_cfg);

    // If the output_path doesn't exist, create it.
    std::string target_folder =
        extractPath(this->output_path + "/" + this->name);
    if (!(fs::exists(target_folder) && fs::is_directory(target_folder))) {
        fs::create_directories(target_folder);
    }

    std::string cmd;

    if (this->type == library_type::static_library) {
        cmd = (
            // linker
            (this->config.linker.length() != 0 ? this->config.linker
                                               : project_cfg.linker) +
            " " +
            // Output file
            (this->output_path.length() != 0 ? this->output_path
                                             : "./build/lib") +
            "/" + this->name + " " +
            // Object files
            joinWithSpace(this->obj_files));
    }

    else if (this->type == library_type::dynamic_library ||
             this->type == library_type::shared_library) {

        cmd = (
            // linker
            (this->config.linker.length() != 0 ? this->config.linker
                                               : project_cfg.linker) +
            " " +
            // Object files
            joinWithSpace(this->obj_files) + " -o " +
            // Output file
            (this->output_path.length() != 0 ? this->output_path
                                             : "./build/lib") +
            "/" + this->name + " " +
            // Linker flags from project
            joinWithSpace(project_cfg.link_flags) + " " +
            // Linker flags from execution
            joinWithSpace(this->config.link_flags));
    }

    // Link
    std::cout << cmd << std::endl;
    if (std::system(cmd.c_str()) != 0)
        return;
}

#define RED "\033[31m"
#define ORANGE "\033[38;5;202m"
#define RESET "\033[0m"
bool ccc::library::check(const ccc::config& project_cfg) {
    bool status = true;

    // Check if compiler is specified
    if (this->config.compiler.length() == 0 &&
        project_cfg.compiler.length() == 0) {
        std::cout << RED << "Error" << RESET
                  << ": No compiler specified for library " << this->name
                  << std::endl;
        status = false;
    }

    // Check if linker is specified
    if (this->config.linker.length() == 0 && project_cfg.linker.length() == 0) {
        std::cout << RED << "Error" << RESET
                  << ": No linker specified for library " << this->name
                  << std::endl;
        status = false;
    }

    // Check if there is any source file
    if (this->source_files.size() == 0) {
        std::cout << ORANGE << "Warning" << RESET
                  << ": There isn't any source file in the library "
                  << this->name << std::endl;
    }

    return status;
}