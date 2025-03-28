#include "ccc/library.h"

void ccc::library::process(ccc::config project_cfg) {

    // If the library is a shared library or a dynamic library, add -fPIC to the
    // compile flags and add -shared to the link flags.
    if (this->type == library_type::shared_library ||
        this->type == library_type::dynamic_library) {
        this->config.compile_flags.push_back("-fPIC");
        this->config.link_flags.push_back("-shared");
    }

    this->handle(project_cfg);

    // If the output_path doesn't exist, create it.
    if (!(fs::exists(this->output_path) &&
          fs::is_directory(this->output_path))) {
        fs::create_directories(this->output_path);
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
    std::system(cmd.c_str());
}