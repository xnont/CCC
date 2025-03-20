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

// AI generated.
static bool compareFileModificationTime(const std::string& first,
                                        const std::string& second) {
    auto firstTime = fs::last_write_time(first);
    auto secondTime = fs::last_write_time(second);
    return firstTime < secondTime;
}

void ccc::compile::handle(ccc::config project_cfg) {
    // Iteratively compile all source files.
    for (auto source_file : source_files) {

        // Get the obj file path.
        std::string obj_file_path =
            this->obj_path + "/" + replaceCppWithO(source_file);
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