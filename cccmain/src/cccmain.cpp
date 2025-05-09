#include "ccc/command.h"
#include "ccc/global.h"
#include "ccc/info.hpp"
#include "ccc/project.h"
#include "util/io.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

/* Declarations of the working function of commands built into ccc. */
void build(std::vector<std::string> args);
void describe(std::vector<std::string> args);
void clean(std::vector<std::string> args);
void version(std::vector<std::string> args);

#define RED "\033[31m"
#define RESET "\033[0m"

int main(int argc, char* argv[]) {
    // If there is no argument, print the help message.
    if (argc == 1) {
        ccc::io::print(ccc::info::help_msg);
        return -1;
    }

    /* The definition of commands built into ccc. */
    ccc::command build_cmd("build", build,
                           "Build the projects based on project.cpp.");
    ccc::command desc_cmd({"desc", "describe"}, describe,
                          "Get a description of what you want to know.");
    ccc::command clean_cmd("clean", clean,
                           "Remove products from the projects.");
    ccc::command version_cmd("cccver", version, "Print the version of ccc.");
    ccc::command help_cmd(
        "ccchelp",
        [](std::vector<std::string> args) {
            if (args.size() == 0 && args.size() != 0)
                return;
            ccc::io::print(ccc::info::help_msg);
        },
        "Print the help message about ccc.");
    ccc::command project_cmd(
        "project",
        [](std::vector<std::string> args) {
            if (args.size() == 0)
                return;
        },
        "Only generate the executable project file without performing any "
        "other operations.");

    std::string target_cmd = "";

    // Determine the target command
    target_cmd = argv[1];

    // Get the arguments
    std::vector<std::string> args;
    for (int i = 2; i < argc; i++)
        args.push_back(argv[i]);

    // Run the target command
    if (ccc::cmds.find(target_cmd) != ccc::cmds.end()) {
        try {
            ccc::cmds[target_cmd]->run(args);
        }
        // Catch the exception and print the error message.
        catch (const std::exception& e) {
            std::cerr << "ccc: " << RED << "error: " << RESET << e.what()
                      << std::endl;
            return -1;
        }
        return 0;
    }

    // If the target command is not found, print an error message.
    std::cerr << "ccc: " << RED << "error: " << RESET << "Unknown command "
              << target_cmd << "." << std::endl;
    return -1;
}

void build(std::vector<std::string> args) {
    // Check if the project.cpp exists.
    if (!fs::exists("project.cpp") || !fs::is_regular_file("project.cpp")) {
        throw std::runtime_error(
            "The build command cannot run in a directory without project.cpp.");
        return;
    }

    // Traverse and process all projects.
    for (auto project : ccc::projects) {
        project->init_func(project, "build", args);
        project->process();
        project->exit_func(project, "build", args);
    }
}

void describe(std::vector<std::string> args) {
    if (args.size() != 1) {
        throw std::runtime_error("Usage: ccc desc <name>");
        return;
    }

    // Initialize all projects.
    for (auto project : ccc::projects) {
        project->init_func(project, "describe", args);
    }

    // Find the target description.
    if (ccc::descs.find(args[0]) != ccc::descs.end()) {
        std::cout << args[0] << ": " << ccc::descs[args[0]] << std::endl;
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
        throw std::runtime_error("Not find description for " + args[0] + ".");
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

        /* Traverse all tasks.(Delete the folders corresponding to output path
         * and obj path.) */
        for (auto& task : project->tasks) {
            remove_directories(task->output_path);
            remove_directories(task->obj_path);
        }

        project->exit_func(project, "clean", args);
    }
}

void version(std::vector<std::string> args) {
    args.push_back("--version");
    std::cout << "ccc version: " << ccc::info::version << std::endl;
}
