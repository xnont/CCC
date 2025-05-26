#include "ccc/global.h"

#include "ccc/command.h"
#include "ccc/info.hpp"
#include "ccc/project.h"
#include "util/io.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::vector<std::string>>
    ccc::global_var::descs;

std::vector<ccc::project*> ccc::global_var::projects;

std::unordered_map<std::string, std::unordered_map<ccc::command::priority,
                                                   std::vector<ccc::command*>>>
    ccc::global_var::cmds;
/* Built-in commands. */
static ccc::command build_cmd(
    "build",
    [](std::vector<std::string> args) {
        // Check if the project.cpp exists.
        if (!fs::exists("project.cpp") || !fs::is_regular_file("project.cpp")) {
            throw std::runtime_error("The build command cannot run in a "
                                     "directory without project.cpp.");
            return;
        }

        // Process all projects.
        ccc::global_var::init_projects("build", args);
        ccc::global_var::process_projects();
        ccc::global_var::exit_projects("build", args);
    },
    "Build the projects based on project.cpp.");
static ccc::command desc_cmd(
    {"desc", "describe"},
    [](std::vector<std::string> args) {
        if (args.size() != 1) {
            throw std::runtime_error("Usage: ccc desc <name>");
            return;
        }

        // Initialize all projects.
        ccc::global_var::init_projects("describe", args);

        // Find the target description.
        auto descs = ccc::global_var::get_descs(args[0]);
        if (descs.size() != 0) {
            for (size_t i = 0; i < descs.size(); i++) {
                std::cout << args[0] << ": " << descs[i] << std::endl;
                if (i != descs.size() - 1)
                    std::cout << std::endl;
            }
            // Exit all projects.
            ccc::global_var::exit_projects("describe", args);
            return;
        }

        // If the target description is not found, throw an error.
        else {
            // Exit all projects.
            ccc::global_var::exit_projects("describe", args);
            throw std::runtime_error("Not find description for " + args[0] +
                                     ".");
            return;
        }
    },
    "Get a description of what you want to know.");
static ccc::command clean_cmd(
    "clean",
    [](std::vector<std::string> args) {
        // Remove function.
        auto remove_directories = [](std::string path) {
            try {
                std::uintmax_t removed_count = fs::remove_all(path);
                std::cout << "Removed " << removed_count
                          << " files/directories from " << path << std::endl;
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Error: " << e.what() << "when try to remove "
                          << path << std::endl;
            }
        };

        // Check if the project.cpp exists.
        if (!fs::exists("project.cpp") || !fs::is_regular_file("project.cpp")) {
            throw std::runtime_error("The clean command cannot run in a "
                                     "directory without project.cpp.");
            return;
        }
        /* Traverse all projects. */
        for (auto project : ccc::global_var::get_projects()) {
            project->init_func(project, "clean", args);

            /* Traverse all tasks.(Delete the folders corresponding to output
             * path and obj path.) */
            for (auto& task : project->tasks) {
                remove_directories(task->output_path);
                remove_directories(task->obj_path);
            }

            project->exit_func(project, "clean", args);
        }
    },
    "Remove products from the projects.");
static ccc::command version_cmd(
    "cccver",
    [](std::vector<std::string> args) {
        args.push_back("--version");
        std::cout << "ccc version: " << ccc::info::version << std::endl;
    },
    "Print the version of ccc.");
static ccc::command help_cmd(
    "ccchelp",
    [](std::vector<std::string> args) {
        if (args.size() == 0 && args.size() != 0)
            return;
        ccc::io::print(ccc::info::help_msg);
    },
    "Print the help message about ccc.");
static ccc::command project_cmd(
    "project",
    [](std::vector<std::string> args) {
        if (args.size() == 0)
            return;
    },
    "Only generate the executable project file without performing any "
    "other operations.");
void ccc::global_var::add_desc(const std::string name, const std::string desc) {
    descs[name].push_back(desc);
}

std::vector<std::string> ccc::global_var::get_descs(const std::string name) {
    return descs[name];
}

void ccc::global_var::add_cmd(const std::string name, const ccc::command* cmd,
                              ccc::command::priority prio) {
    cmds[name][prio].push_back(const_cast<ccc::command*>(cmd));
}

const ccc::command* ccc::global_var::get_cmd(const std::string name) {
    // Check if the command exists
    if (cmds.find(name) == cmds.end()) {
        throw std::runtime_error("The command '" + name +
                                 "' is not registered.");
    }

    // Get priority map
    auto& priority_map = cmds[name];

    // Find the highest priority (high > normal > low)
    auto max_prio = ccc::command::priority::low;
    for (const auto& [prio, cmds] : priority_map) {
        if (!cmds.empty() && prio > max_prio) {
            max_prio = prio;
        }
    }

    // Determine the priority name
    std::string prio_name;
    switch (max_prio) {
    case ccc::command::priority::high:
        prio_name = "high";
        break;
    case ccc::command::priority::normal:
        prio_name = "normal";
        break;
    case ccc::command::priority::low:
        prio_name = "low";
        break;
    }

    // Get the command list for the highest priority
    auto& candidates = priority_map.at(max_prio);

    // Check if there are multiple candidates
    if (candidates.size() != 1) {
        throw std::runtime_error("The multiple commands are registered for '" +
                                 name + "' with the same(" + prio_name +
                                 ") priority.");
    }

    // Return the single command
    return candidates.front();
}

void ccc::global_var::add_project(ccc::project* project) {
    projects.push_back(project);
}

void ccc::global_var::init_projects(std::string cmd,
                                    std::vector<std::string> args) {
    for (auto project : ccc::global_var::projects) {
        project->init_func(project, cmd, args);
    }
}

void ccc::global_var::process_projects() {
    // Traverse and process all projects.
    for (auto project : ccc::global_var::projects) {
        project->process();
    }
}

void ccc::global_var::exit_projects(std::string cmd,
                                    std::vector<std::string> args) {
    for (auto project : ccc::global_var::projects) {
        project->exit_func(project, cmd, args);
    }
}

const std::vector<ccc::project*> ccc::global_var::get_projects() {
    return ccc::global_var::projects;
}