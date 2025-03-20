#include "ccc/compile.h"

void ccc::compile::handle(ccc::config project_cfg) {
    // Iteratively compile all source files.
    for (auto source_file : source_files) {

        // Get the obj file path.
        std::string obj_file_path =
            this->obj_path + "/" + changeFileExtensionToO(source_file);
        // Add the obj file to the list.
        obj_files.push_back(obj_file_path);

        // If the obj file exists and is newer than the source file, skip it.
        if (fs::exists(obj_file_path) &&
            compareFileModificationTime(source_file, obj_file_path)) {
            continue;
        }

        // Get the target folder(The storage path of the obj file.)
        std::string target_folder = extractPath(obj_file_path);
        // If the folder doesn't exist, create it.
        if (!directoryExists(target_folder)) {
            fs::create_directories(target_folder);
        }

        std::string cmd = (
            // Compiler
            (this->config.compiler.length() != 0 ? this->config.compiler
                                                 : project_cfg.compiler) +
            // Only compile without linking
            " -c " +
            // Source file
            source_file + " -o " +
            // Output file
            obj_file_path + " " +
            // Compile flags from project
            joinWithSpace(project_cfg.compile_flags) + " " +
            // Compile flags from execution
            joinWithSpace(this->config.compile_flags));

        std::cout << cmd << std::endl;
        std::system(cmd.c_str());
    }
}