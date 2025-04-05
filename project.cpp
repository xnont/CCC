#include "ccc/project.h"
#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace ccc;
namespace fs = filesystem;

void copy_directory(const fs::path& source, const fs::path& destination);

void ccc_init(project* self, string cmd, vector<string> args) {
    // Set compiler flags
    unordered_set<string> ccc_args(args.begin(), args.end());
    if (ccc_args.find("release") != ccc_args.end()) {
        self->config.compile_flags.push_back(
            "-Og -g -std=c++17 -W -Wall -Wextra");
    } else {
        self->config.compile_flags.push_back("-O2 -std=c++17 -W -Wall -Wextra");
    }

    execution ccc("ccc", "");
    ccc.obj_path = "./ccc/build/obj";
    ccc.output_path = "./build/bin";
    ccc.source_files = {"./ccc/src/ccc.cpp"};
    ccc.config.header_folder_paths = {"./cccproject/inc", "./ccc/inc"};
    self->exes.push_back(ccc);

    library cccmain("cccmain.a", static_library, "");
    cccmain.obj_path = "./cccmain/build/obj";
    cccmain.output_path = "./build/lib";
    cccmain.source_files = {"./cccmain/src/cccmain.cpp"};
    cccmain.config.header_folder_paths = {"./cccproject/inc"};
    self->libs.push_back(cccmain);

    library cccproject("cccproject.a", static_library, "");
    cccproject.obj_path = "./cccproject/build/obj";
    cccproject.output_path = "./build/lib";
    cccproject.add_source_files({"./cccproject/src/ccc"}, {".cpp"});
    cccproject.config.header_folder_paths = {"./cccproject/inc"};
    self->libs.push_back(cccproject);

    execution default_project("default_project", "");
    default_project.output_path = "./build/bin";
    default_project.obj_files = {
        "./build/lib/cccmain.a ./build/lib/cccproject.a"};
    self->exes.push_back(default_project);

    if (cmd == "build") {
        copy_directory("./cccproject/inc", "./build/inc");
    }

    if (cmd == "clean") {
        fs::remove_all("./build/inc");
    }
}

void ccc_exit(project* self, string cmd, vector<string> args) {}

project ccc_project("CCC", ccc_init, ccc_exit, "");

void copy_directory(const fs::path& source, const fs::path& destination) {
    try {
        // Check if the source path exists and is a directory
        if (!fs::exists(source) || !fs::is_directory(source)) {
            cerr << "Source directory does not exist or is not a directory."
                 << endl;
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
        cerr << "Filesystem error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "General error: " << e.what() << endl;
    }
}

#ifdef _WIN32
void debug(vector<string> args) {
    cout << "Compile the ccc in debug mode." << endl;
    system("ccc build debug");
}
#else
void debug(vector<string> args) {
    cout << "Compile the ccc in debug mode." << endl;
    system("bash -c 'ccc build debug'");
}
#endif
command debug_cmd("debug", debug, "Compile the ccc in debug mode.");

#ifdef _WIN32
void release(vector<string> args) {
    cout << "Compile the ccc in release mode." << endl;
    system("ccc build release");
}
#else
void release(vector<string> args) {
    cout << "Compile the ccc in release mode." << endl;
    system("bash -c 'ccc build release'");
}
#endif
command release_cmd("release", release, "Compile the ccc in release mode.");