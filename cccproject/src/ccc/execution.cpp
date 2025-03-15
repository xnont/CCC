#include "ccc/execution.h"

void ccc::execution::process(ccc::config project_cfg) {
    this->handle(project_cfg);

    // If the build_dir_path doesn't exist, create it.
    if (!(fs::exists(this->build_dir_path) &&
          fs::is_directory(this->build_dir_path))) {
        fs::create_directories(this->build_dir_path);
    }

    std::string cmd = (
        // linker
        (this->config.linker.length() != 0 ? this->config.linker
                                           : project_cfg.linker) +
        " " +
        // Object files
        ccc::joinWithSpace(this->obj_files) + " -o " +
        // Output file
        this->build_dir_path + "/" + this->name + " " +
        // Linker flags from project
        ccc::joinWithSpace(project_cfg.link_flags) + " " +
        // Linker flags from execution
        ccc::joinWithSpace(this->config.link_flags));

    // Link
    std::cout << cmd << std::endl;
    std::system(cmd.c_str());
}
