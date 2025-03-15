#include "ccc/compile.h"

// AI generated.
static std::string replaceCppWithO(const std::string& filename) {
    std::string result = filename;
    size_t pos = result.find(".cpp");
    if (pos != std::string::npos) {
        result.replace(pos, 4, ".o");
    }
    return result;
}

// AI generated.
static bool directoryExists(const std::string& path) {
    return fs::exists(path) && fs::is_directory(path);
}

// AI generated.
static std::string extractPath(const std::string& fullPath) {
    size_t found = fullPath.find_last_of("/\\");
    if (found != std::string::npos) {
        return fullPath.substr(0, found);
    }
    return "";
}

void ccc::compile::handle(ccc::config project_cfg) {
    // Iteratively compile all source files.
    for (auto source_file : source_files) {

        // Get the obj file path.
        std::string obj_file_path =
            this->build_dir_path + "/" + replaceCppWithO(source_file);

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
            std::accumulate(project_cfg.compile_flags.begin(),
                            project_cfg.compile_flags.end(), std::string(" ")) +
            " " +
            // Compile flags from execution
            std::accumulate(this->config.compile_flags.begin(),
                            this->config.compile_flags.end(),
                            std::string(" ")));

        std::cout << cmd << std::endl;
        std::system(cmd.c_str());

        // Add the obj file to the list.
        obj_files.push_back(obj_file_path);
    }
}