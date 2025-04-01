#include "util/file.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv) {

    /* Parse the command line arguments. */
    string cmd;
    for (int i = 1; i < argc; i++) {
        cmd += " " + string(argv[i]);
    }

    /* Set the default project configuration file. */
    string project_config_file = "project.cpp";

    /* If there is no project.cpp file in the current directory, run the default
     * cccpoject. */
    if (!fs::exists(project_config_file)) {
        cmd = "cccproject " + cmd;
        std::system(cmd.c_str());
        return 0;
    }

    /* Set the default project binary file. */
#ifdef _WIN32
    string project_bin_file = "project.exe";
#else
    string project_bin_file = "project";
#endif

    const char* CCC_COMPILER = std::getenv("CCC_COMPILER");
    const char* CCC_INCLUDE_PATH = std::getenv("CCC_INCLUDE_PATH");
    const char* CCC_LIBRARY_PATH = std::getenv("CCC_LIBRARY_PATH");

    /* Check if the project binary file is out of date. */
    if (!fs::exists(project_bin_file) ||
        !compareFileModificationTime(project_config_file, project_bin_file) ||
        !compareFileModificationTime(string(CCC_LIBRARY_PATH) + "/cccmain.a",
                                     project_bin_file) ||
        !compareFileModificationTime(string(CCC_LIBRARY_PATH) + "/cccproject.a",
                                     project_bin_file)) {

        string cmd = CCC_COMPILER + string(" ") + project_config_file + " " +
                     CCC_LIBRARY_PATH + "/cccmain.a" + " " + CCC_LIBRARY_PATH +
                     "/cccproject.a" + " -o " + project_bin_file + " -I " +
                     CCC_INCLUDE_PATH;

        std::system(cmd.c_str());
    }

    /* Run the project binary file. */
    cmd = project_bin_file + " " + cmd;
    std::system(cmd.c_str());

    return 0;
}