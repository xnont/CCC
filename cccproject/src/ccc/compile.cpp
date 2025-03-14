#include "ccc/compile.h"

std::string replaceCppWithO(const std::string& filename) {
    std::string result = filename;
    size_t pos = result.find(".cpp");
    if (pos != std::string::npos) {
        result.replace(pos, 4, ".o");
    }
    return result;
}

void ccc::compile::handle(ccc::config project_cfg) {
    // Compile
    for (auto source_file : source_files) {
        std::string cmd = (
            // Compiler
            (this->cfg.compiler.length() != 0 ? this->cfg.compiler
                                              : project_cfg.compiler) +
            // Only compile
            " -c " +
            // Source file
            source_file + " -o " +
            // Output file
            this->build_dir_path + "/" + replaceCppWithO(source_file));

        std::system(cmd.c_str());
        obj_files.push_back(this->build_dir_path + "/" +
                            replaceCppWithO(source_file));
    }
}