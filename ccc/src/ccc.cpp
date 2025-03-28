#include "util/file.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char** argv) {

    /* Set the default project configuration file. */
    string project_config_file = "project.cpp";

    /* Set the default project binary file. */
#ifdef _WIN32
    string project_bin_file = "project.exe";
#else
    string project_bin_file = "project";
#endif

    /* Check if the project binary file is out of date. */
    if (!fs::exists(project_bin_file) ||
        !compareFileModificationTime(project_config_file, project_bin_file)) {

        const char* CCC_COMPILER = std::getenv("CCC_COMPILER");
        const char* CCC_INCLUDE_PATH = std::getenv("CCC_INCLUDE_PATH");
        const char* CCC_LIBRARY_PATH = std::getenv("CCC_LIBRARY_PATH");

        string cmd = CCC_COMPILER + string(" ") + project_config_file + " " +
                     CCC_LIBRARY_PATH + "/cccmain.a" + " " + CCC_LIBRARY_PATH +
                     "/cccproject.a" + " -o " + project_bin_file + " -I " +
                     CCC_INCLUDE_PATH;

        std::system(cmd.c_str());
    }

    /* Run the project binary file. */
    string cmd = project_bin_file;
    for (int i = 1; i < argc; i++) {
        cmd += " " + string(argv[i]);
    }
    std::system(cmd.c_str());

    return 0;
}