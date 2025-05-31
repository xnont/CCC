#include "util/file.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <string>
#include <vector>

namespace fs = std::filesystem;

/* The home directory of the user. */
std::string user_home =
#ifdef __linux__
    std::getenv("HOME");
#elif _WIN32
    std::getenv("USERPROFILE");
#endif

/* The name of the cccmain library. */
std::string cccmain_name =
#ifdef _WIN32
    "libcccmain.lib";
#endif
#ifdef __linux__
"libcccmain.a";
#endif

/* The name of the cccunit library. */
std::string cccunit_name =
#ifdef _WIN32
    "cccunit.dll";
#endif
#ifdef __linux__
"libcccunit.so";
#endif

void process_project(std::vector<std::string> source_list, std::string target,
                     std::string compile_flags, std::string run_args) {
    // Obtain the source files.
    std::string source;
    for (auto& source_file : source_list) {
        source += source_file + " ";
    }

    // Compile the project.cpp
    std::string compiler = std::getenv("CCC_COMPILER");
    std::string compile_cmd = compiler + " -o " +
                              // Target
                              target + " " +
                              // cccmain library
                              user_home + "/.ccc/lib/" + cccmain_name + " " +
                              // Source
                              source + " " +
                              // cccunit library
                              "-lcccunit -L" + user_home + "/.ccc/lib " +
                              // Include path
                              "-I" + user_home + "/.ccc/inc " +
                              // Compile flags
                              compile_flags;
    for (auto& source_file : source_list) {
        if (!fs::exists(target) ||
            !compareFileModificationTime(source_file, target) ||
            !compareFileModificationTime(
                user_home + "/.ccc/lib/" + cccmain_name, target) ||
            !compareFileModificationTime(
                user_home + "/.ccc/lib/" + cccunit_name, target)) {
            if (std::system(compile_cmd.c_str()) != 0) {
                std::exit(1);
            }
            break;
        }
    }
    // Run the project.exe
#ifdef __linux__
    std::string run_cmd = "bash -c '" + target + " " + run_args + "'";
#elif _WIN32
    std::string run_cmd = target + " " + run_args;
#endif
    if (std::system(run_cmd.c_str()) != 0) {
        std::exit(1);
    }
}

int main(int argc, char** argv) {
    // Parse the command line arguments.
    std::string compile_flags;
    std::string run_args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        // Obtain the compile flags(Used to compile the project.cpp file).
        if (arg.find("flags=") == 0) {
            size_t eq_pos = arg.find('=');
            std::string flags_value = arg.substr(eq_pos + 1);
            compile_flags += " " + flags_value;
        }
        // Obtain the run arguments(Used to run the project.exe).
        else {
            // Special case for 'project' command
            if (run_args.size() == 0 && arg == "project") {
#ifdef __linux__
                std::string bin = "project";
#elif _WIN32
                std::string bin = "project.exe";
#endif
                if (fs::exists(bin))
                    fs::remove(bin);
                else if (fs::exists(user_home + "/.ccc/" + bin))
                    fs::remove(user_home + "/.ccc/" + bin);
            }

            run_args += " " + arg;
        }
    }

    // If there is not a project.cpp file in the ~/.ccc directory, create it.
    if (!fs::exists(user_home + "/.ccc/project.cpp"))
        std::ofstream(user_home + "/.ccc/project.cpp").close();

    std::vector<std::string> source_list{user_home + "/.ccc/project.cpp"};

    // If there is a project.cpp file, compile and run it.
    if (fs::exists("project.cpp")) {
        source_list.push_back("project.cpp");
#ifdef __linux__
        process_project(source_list, "./project", compile_flags, run_args);
#elif _WIN32
        process_project(source_list, "project.exe", compile_flags, run_args);
#endif
    }

    // If there is no project.cpp file, use the default project.cpp file(Located
    // in the ~/.ccc directory).
    else {
#ifdef __linux__
        process_project(source_list, user_home + "/.ccc/project", compile_flags,
                        run_args);
#elif _WIN32
        process_project(source_list, user_home + "/.ccc/project.exe",
                        compile_flags, run_args);
#endif
    }

    return 0;
}