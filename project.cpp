#include "ccc/project.h"
#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace ccc;
namespace fs = std::filesystem;

void copy_directory(const fs::path& source, const fs::path& destination);

void ccc_init(project* self, vector<string> args) {
    // Project settings
    self->config.compiler = "g++";
    self->config.linker = "g++";
    self->config.thread_num = 0; /*  0 means auto */

    // Set compiler flags
    unordered_set<string> ccc_args(args.begin(), args.end());
    if (ccc_args.find("release") != ccc_args.end()) {
        self->config.compile_flags.push_back(
            "-Og -g -std=c++17 -W -Wall -Wextra -Werror");
    } else {
        self->config.compile_flags.push_back(
            "-O2 -std=c++17 -W -Wall -Wextra -Werror");
    }

    execution ccc("ccc", "");
    ccc.obj_path = "./ccc/build/obj";
    ccc.output_path = "./build/bin";
    ccc.source_files = {"./ccc/src/ccc.cpp"};
    ccc.config.compile_flags.push_back("-I ./cccproject/inc -I./ccc/inc");
    self->exes.push_back(ccc);

    library cccmain("cccmain.a", "");
    cccmain.config.linker = "ar rcs";
    cccmain.type = static_library;
    cccmain.obj_path = "./cccmain/build/obj";
    cccmain.output_path = "./build/lib";
    cccmain.source_files = {"./cccmain/src/cccmain.cpp"};
    cccmain.config.compile_flags.push_back("-I ./cccproject/inc");
    self->libs.push_back(cccmain);

    library cccproject("cccproject.a", "");
    cccproject.config.linker = "ar rcs";
    cccproject.type = static_library;
    cccproject.obj_path = "./cccproject/build/obj";
    cccproject.output_path = "./build/lib";
    cccproject.source_files = {"./cccproject/src/ccc/command.cpp",
                               "./cccproject/src/ccc/compile_task.cpp",
                               "./cccproject/src/ccc/description.cpp",
                               "./cccproject/src/ccc/execution.cpp",
                               "./cccproject/src/ccc/library.cpp",
                               "./cccproject/src/ccc/project.cpp"};
    cccproject.config.compile_flags.push_back("-I ./cccproject/inc");
    self->libs.push_back(cccproject);
}

void ccc_exit(project* self, vector<string> args) {}

project ccc_project("CCC", ccc_init, ccc_exit, "");

void copy_directory(const fs::path& source, const fs::path& destination) {
    try {
        // Check if the source path exists and is a directory
        if (!fs::exists(source) || !fs::is_directory(source)) {
            std::cerr
                << "Source directory does not exist or is not a directory."
                << std::endl;
            return;
        }

        // Create target directory
        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }

        // Traverse all entries in the source directory
        for (const auto& entry : fs::directory_iterator(source)) {
            const auto& path = entry.path();
            const auto& filename = path.filename();
            const auto dest_path = destination / filename;

            if (fs::is_directory(path)) {
                // If it is a directory, recursively copy
                copy_directory(path, dest_path);
            } else if (fs::is_regular_file(path)) {
                // If it is a file, copy the file
                fs::copy_file(path, dest_path,
                              fs::copy_options::overwrite_existing);
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
}
