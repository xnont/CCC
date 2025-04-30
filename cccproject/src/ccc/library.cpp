#include "ccc/library.h"
#include "util/io.h"

ccc::library::library(std::string name, ccc::library_type type,
                      std::string description)
    : ccc::compile_task(name, description) {
    // Set the type
    this->type = type;

    // If the given library name does not have a suffix, add the prefix and
    // suffix according to the operating system.
    if (this->name.find(".") == std::string::npos) {
        // Set the library name prefix and suffix according to the target
        // operating system. Windows
        if (this->target_os == ccc::system_type::windows_os) {
            if (this->type == library_type::static_library) {
                this->name = "lib" + this->name + ".lib";
            } else if (this->type == library_type::dynamic_library ||
                       this->type == library_type::shared_library) {
                this->name = this->name + ".dll";
            }
        }
        // Linux
        else if (this->target_os == ccc::system_type::linux_os) {
            if (this->type == library_type::static_library) {
                this->name = "lib" + this->name + ".a";
            } else if (this->type == library_type::dynamic_library ||
                       this->type == library_type::shared_library) {
                this->name = "lib" + this->name + ".so";
            }
        }
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
            ("ar rcs") + std::string(" ") +
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
            (this->config.linker.length() != 0  ? this->config.linker
             : project_cfg.linker.length() != 0 ? project_cfg.linker
                                                : "g++") +
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
    ccc::io::exec_command(cmd, project_cfg.is_print && this->config.is_print,
                          project_cfg.is_print && this->config.is_print);
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