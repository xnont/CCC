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
        std::accumulate(this->obj_files.begin(), this->obj_files.end(),
                        std::string(" ")) +
        " -o " +
        // Output file
        this->build_dir_path + "/" + this->name + " " +
        // Linker flags from project
        std::accumulate(project_cfg.link_flags.begin(),
                        project_cfg.link_flags.end(), std::string(" ")) +

        // Linker flags from execution
        std::accumulate(this->config.link_flags.begin(),
                        this->config.link_flags.end(), std::string(" ")));

    // Link
    std::cout << cmd << std::endl;
    std::system(cmd.c_str());
}
