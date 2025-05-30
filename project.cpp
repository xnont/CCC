#include "ccc/project.h"
#include "ccc/command.h"
#include "ccc/info.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace ccc;
namespace fs = filesystem;

project ccc_project(
    "CCC",
    [](project* self, string cmd, vector<string> args) {
        // Set compiler flags
        unordered_set<string> ccc_args(args.begin(), args.end());
        if (ccc_args.find("release") != ccc_args.end()) {
            self->add_compile_flags(
                {"-O2", "-std=c++17", "-W", "-Wall", "-Wextra"});
        } else {
            self->add_compile_flags(
                {"-g", "-std=c++17", "-W", "-Wall", "-Wextra"});
        }

        if (ccc_args.count("--noprint")) {
            self->config.is_print = false;
        }

        /* Describe the executable program ccc. */
        execution ccc(
            "ccc",
            "The ccc does not rely on cccmain and cccunit, but its working "
            "principle is to link project.cpp, cccmain, and cccunit in a "
            "directory with project.cpp to generate and call project, and call "
            "defautl_project in a directory without project.cpp.");
        ccc.obj_path = "./ccc/build/obj";
        ccc.output_path = "./build/bin";
        ccc.add_source_files({"./ccc/src/ccc.cpp"});
        ccc.add_header_folder_paths({"./cccunit/inc", "./ccc/inc"});
        self->add_task(&ccc);

        /* Describe the library file cccmain. */
        library cccmain("cccmain", static_library,
                        "The cccmain relies on cccunit and defines the "
                        "built-in commands of "
                        "ccc using the interfaces provided in cccunit.");
        cccmain.obj_path = "./cccmain/build/obj";
        cccmain.output_path = "./build/lib";
        cccmain.add_source_files({"./cccmain/src/cccmain.cpp"});
        cccmain.add_header_folder_paths({"./cccunit/inc"});
        self->add_task(&cccmain);

        /* Describe the library file cccunit. */
        library cccunit("cccunit", shared_library,
                        "The ccc project is the main component of the project, "
                        "defining how to describe the project when using ccc.");
        cccunit.obj_path = "./cccunit/build/obj";
        cccunit.output_path = "./build/lib";
        cccunit.add_source_files({"./cccunit/src/"}, {".cpp"});
        cccunit.add_header_folder_paths({"./cccunit/inc"});
        self->add_task(&cccunit);
    },
    [](project* self, string cmd, vector<string> args) {
        // Copy the cccunit/inc directory to the build/inc directory when
        // running the build command.
        if (cmd == "build" && fs::exists("./cccunit/inc") &&
            fs::is_directory("./cccunit/inc")) {

            // AI generated
            function<void(const fs::path& source, const fs::path& destination)>
                copy_directory = [&](const fs::path& source,
                                     const fs::path& destination) -> void {
                try {
                    // Check if the source path exists and is a directory
                    if (!fs::exists(source) || !fs::is_directory(source)) {
                        cerr << "Source directory does not exist or is not a "
                                "directory."
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
                            // If it is a file, remove the existing file and
                            // copy the file
                            if (fs::exists(dest_path)) {
                                fs::remove(dest_path);
                            }
                            fs::copy_file(path, dest_path,
                                          fs::copy_options::overwrite_existing);
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    cerr << "Filesystem error: " << e.what() << endl;
                } catch (const exception& e) {
                    cerr << "General error: " << e.what() << endl;
                }
            };
            copy_directory("./cccunit/inc", "./build/inc");
        }

        // Clean the build/inc directory when running the clean command.
        if (cmd == "clean") {
            fs::remove_all("./build/inc");
        }
    },
    "\n" + info::help_msg +
        "Note: For the built-in commands provided above, you can "
        "use them directly in any project.\n" +

        "\nExtended commands:\n"
        "    debug                Compile the ccc in debug mode.\n"
        "    release              Compile the ccc in release mode.\n"
        "    help                 Print the help message about this "
        "project(ccc).\n"
        "    line                 Print the number of lines of code for the "
        "ccc project.\n"
        "Extended arguments:\n"
        "    --noprint            Don't generate any output when compile the "
        "ccc.\n"
        "Note: For the extended commands and arguments, you can use them in "
        "the this project. But if you want to use them in other projects, you "
        "need to achieve them by yourself.\n");

command debug_cmd(
    "debug",
    [](vector<string> args) {
        cout << "Compile the ccc in debug mode." << endl;
        std::string cmd = "ccc build debug";
        for (size_t i = 0; i < args.size(); i++) {
            cmd += " " + args[i];
        }
#ifdef _WIN32
        system(cmd.c_str());
#endif
#ifdef __linux__
        system(("bash -c '" + cmd + "'").c_str());
#endif
    },
    "Compile the ccc in debug mode.");

command release_cmd(
    "release",
    [](vector<string> args) {
        cout << "Compile the ccc in release mode." << endl;
        std::string cmd = "ccc build release";
        for (size_t i = 0; i < args.size(); i++) {
            cmd += " " + args[i];
        }
#ifdef _WIN32
        system(cmd.c_str());
#endif
#ifdef __linux__
        system(("bash -c '" + cmd + "'").c_str());
#endif
    },
    "Compile the ccc in release mode.");

command help_cmd(
    "help",
    [](vector<string> args) {
        std::string cmd = "ccc desc CCC";
        for (size_t i = 0; i < args.size(); i++) {
            cmd += " " + args[i];
        }
#ifdef _WIN32
        system(cmd.c_str());
#endif
#ifdef __linux__
        system(("bash -c '" + cmd + "'").c_str());
#endif
    },
    "Print the help message about this project(ccc).");

command line_cmd(
    "line",
    // AI generated
    [](vector<string> args) {
        int total_lines = 0;
        vector<string> directories = {"./ccc", "./cccmain", "./cccunit"};
        unordered_set<string> valid_ext = {".cpp", ".h", ".hpp", ".c"};

        auto count_lines = [&](const fs::path& path) {
            ifstream file(path);
            if (!file.is_open())
                return 0;

            int count = 0;
            string line;
            while (getline(file, line)) {
                line.erase(remove_if(line.begin(), line.end(), ::isspace),
                           line.end());
                if (!line.empty()) {
                    count++;
                }
            }
            return count;
        };

        for (const auto& dir : directories) {
            int dir_lines = 0;
            cout << "Counting lines in: " << dir << endl;

            for (const auto& entry : fs::recursive_directory_iterator(dir)) {
                if (entry.is_regular_file() &&
                    valid_ext.count(entry.path().extension().string())) {
                    int lines = count_lines(entry.path());
                    dir_lines += lines;
                    cout << "  " << entry.path().filename() << ": " << lines
                         << endl;
                }
            }

            cout << "Total for " << fs::path(dir).filename() << ": "
                 << dir_lines << "\n\n";
            total_lines += dir_lines;
        }

        cout << "================================\n";
        cout << "Grand total lines of code: " << total_lines << endl;
    },
    "Print the number of lines of code for the ccc project.");