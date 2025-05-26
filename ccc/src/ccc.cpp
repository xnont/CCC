#include "util/file.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char** argv) {

    /* Parse the command line arguments. */
    std::string cmd;
    for (int i = 1; i < argc; i++) {
        cmd += " " + std::string(argv[i]);
    }

    /* Set the default project configuration file. */
    std::string project_config_file = "project.cpp";

    /* If there is no project.cpp file in the current directory, run the default
     * cccpoject. */
    if (!fs::exists(project_config_file)) {
#ifdef _WIN32
        cmd = "default_project " + cmd;
#endif
#ifdef __linux__
        cmd = "bash -c 'default_project " + cmd + "'";
#endif
        if (std::system(cmd.c_str()) != 0)
            return -1;
        return 0;
    }

    /* Set the default project binary file. */
#ifdef _WIN32
    std::string project_bin_file = "project.exe";
#endif
#ifdef __linux__
    std::string project_bin_file = "./project";
#endif

    // Check if the required environment variables are set.
    const char* CCC_COMPILER = std::getenv("CCC_COMPILER");
    const char* CCC_INCLUDE_PATH = std::getenv("CCC_INCLUDE_PATH");
    const char* CCC_LIBRARY_PATH = std::getenv("CCC_LIBRARY_PATH");
    if (CCC_COMPILER == nullptr || CCC_INCLUDE_PATH == nullptr ||
        CCC_LIBRARY_PATH == nullptr) {
        std::cerr << "The required environment variables("
                  << (CCC_COMPILER == nullptr ? "CCC_COMPILER " : "")
                  << (CCC_INCLUDE_PATH == nullptr ? "CCC_INCLUDE_PATH " : "")
                  << (CCC_LIBRARY_PATH == nullptr ? "CCC_LIBRARY_PATH" : "")
                  << ") for ccc are missing. "
                     "Please try reinstalling ccc."
                  << std::endl;
        return -1;
    }

    std::string cccmain_path = std::string(CCC_LIBRARY_PATH) +
#ifdef _WIN32
                               "/libcccmain.lib";
#endif
#ifdef __linux__
    "/libcccmain.a";
#endif

    std::string cccunit_path = std::string(CCC_LIBRARY_PATH) +
#ifdef _WIN32
                               "/cccunit.dll";
#endif
#ifdef __linux__
    "/libcccunit.so";
#endif

    // For the 'project' command, delete the 'project_bin_file' file to update
    // it.
    if (argc >= 2 && std::string(argv[1]) == "project" &&
        fs::exists(project_bin_file)) {
        fs::remove(project_bin_file);
    }

    /* Check if the project binary file is out of date. */
    if (!fs::exists(project_bin_file) ||
        !compareFileModificationTime(project_config_file, project_bin_file) ||
        !compareFileModificationTime(cccmain_path, project_bin_file) ||
        !compareFileModificationTime(cccunit_path, project_bin_file)) {

#ifdef _WIN32
        std::string compile_cmd = CCC_COMPILER + std::string(" ") +
                                  // project.cpp
                                  project_config_file + " " +
                                  // cccmain
                                  cccmain_path + " " +
                                  // cccunit
                                  cccunit_path + " " +
                                  // project bin
                                  " -o " + project_bin_file + " -I " +
                                  CCC_INCLUDE_PATH;
#endif
#ifdef __linux__
        std::string compile_cmd = CCC_COMPILER + std::string(" ") +
                                  // cccmain
                                  cccmain_path + " " +
                                  // project.cpp
                                  project_config_file + " " +
                                  // cccunit
                                  cccunit_path + " " +
                                  // project bin
                                  " -o " + project_bin_file + " -I " +
                                  CCC_INCLUDE_PATH;
#endif
        // Determine whether additional operations are required for the
        // generated project.
        for (int i = 0; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-g") {
                compile_cmd += " -g";
            }
            if (arg == "-O0") {
                compile_cmd += " -O0";
            }
            if (arg == "-O1") {
                compile_cmd += " -O1";
            }
            if (arg == "-O2") {
                compile_cmd += " -O2";
            }
            if (arg == "-O3") {
                compile_cmd += " -O3";
            }
        }

        /* Compile the project binary file. */
        if (std::system(compile_cmd.c_str()) != 0)
            return -1;
    }

    /* Run the project binary file. */
    cmd = project_bin_file + " " + cmd;
    if (std::system(cmd.c_str()) != 0)
        return -1;

    return 0;
}