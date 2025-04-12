#include "ccc/project.h"
#include <iostream>
#include <string>
#include <vector>

namespace ccc {
std::unordered_map<std::string, std::string> descs;
std::vector<ccc::project*> projects;
std::unordered_map<std::string, ccc::command*> cmds;
} // namespace ccc

void build(std::vector<std::string> args);
void describe(std::vector<std::string> args);
void clean(std::vector<std::string> args);
void version(std::vector<std::string> args);

#define RED "\033[31m"
#define RESET "\033[0m"

int main(int argc, char* argv[]) {

    ccc::command build_cmd({"", "build"}, build, "Builds all projects");
    ccc::command desc_cmd({"desc", "describe"}, describe,
                          "Describes all projects");
    ccc::command clean_cmd("clean", clean, "Cleans all projects");
    ccc::command version_cmd("--version", version, "Prints the version");

    std::string target_cmd = "";

    // Determine the target command
    if (argc > 1)
        target_cmd = argv[1];

    // Get the arguments
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    // Run the target command
    if (ccc::cmds.find(target_cmd) != ccc::cmds.end()) {
        try {
            ccc::cmds[target_cmd]->run(args);
        } catch (const std::exception& e) {
            std::cerr << "ccc: " << RED << "error: " << RESET << e.what()
                      << std::endl;
            return -1;
        }
        return 0;
    }

    std::cout << "Unknown command: " << target_cmd << std::endl;
    return -1;
}

void build(std::vector<std::string> args) {
    // Check if the project.cpp exists.
    if (!fs::exists("project.cpp") || !fs::is_regular_file("project.cpp")) {
        throw std::runtime_error(
            "The build command cannot run in a directory without project.cpp.");
        return;
    }
    for (auto project : ccc::projects) {
        project->init_func(project, "build", args);
        project->process();
        project->exit_func(project, "build", args);
    }
}

void describe(std::vector<std::string> args) {
    if (args.size() != 3) {
        std::cout << "Usage: ccc desc <command name>" << std::endl;
        return;
    }

    // Initialize all projects.
    for (auto project : ccc::projects) {
        project->init_func(project, "describe", args);
    }

    // Find the target description.
    if (ccc::descs.find(args[2]) != ccc::descs.end()) {
        std::cout << args[2] << ": " << ccc::descs[args[2]] << std::endl;
        // Exit all projects.
        for (auto project : ccc::projects) {
            project->exit_func(project, "describe", args);
        }
        return;
    }

    // If the target description is not found, throw an error.
    else {
        // Exit all projects.
        for (auto project : ccc::projects) {
            project->exit_func(project, "describe", args);
        }
        throw std::runtime_error("Not find description for " + args[2] + ".");
        return;
    }
}

void remove_directories(std::string path) {
    try {
        std::uintmax_t removed_count = fs::remove_all(path);
        std::cout << "Removed " << removed_count << " files/directories from "
                  << path << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << "when try to remove " << path
                  << std::endl;
    }
}

void clean(std::vector<std::string> args) {
    // Check if the project.cpp exists.
    if (!fs::exists("project.cpp") || !fs::is_regular_file("project.cpp")) {
        throw std::runtime_error(
            "The clean command cannot run in a directory without project.cpp.");
        return;
    }
    /* Traverse all projects. */
    for (auto project : ccc::projects) {
        project->init_func(project, "clean", args);

        /* Traverse all executables.(Delete the folders corresponding to output
         * path and obj path.) */
        for (auto exe : project->exes) {
            remove_directories(exe.output_path.length() != 0 ? exe.output_path
                                                             : "./build/bin");
            remove_directories(exe.obj_path.length() != 0 ? exe.obj_path
                                                          : "./build/obj");
        }
        /* Traverse all libraries.(Delete the folders corresponding to output
         * path and obj path.) */
        for (auto lib : project->libs) {
            remove_directories(lib.output_path.length() != 0 ? lib.output_path
                                                             : "./build/lib");
            remove_directories(lib.obj_path.length() != 0 ? lib.obj_path
                                                          : "./build/obj");
        }

        project->exit_func(project, "clean", args);
    }
}

void version(std::vector<std::string> args) {
    args.push_back("--version");
    std::cout << "ccc version: " << "0.0.0.20250328" << std::endl;
}
