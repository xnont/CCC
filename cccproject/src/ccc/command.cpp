#include "ccc/command.h"

namespace ccc {
std::vector<ccc::command*> cmds;
}

ccc::command::command(std::string name,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description)
    : run(run), description(description) {
    this->names.push_back(name);
    cmds.push_back(this);
}

ccc::command::command(std::initializer_list<std::string> names,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description)
    : run(run), description(description) {
    this->names = names;
    cmds.push_back(this);
}
#include "ccc/project.h"
void build(std::vector<std::string> args) {
    for (auto project : ccc::projects) {
        project->init_func(project, args);
        project->process();
        project->exit_func(project, args);
    }
}
ccc::command build_cmd({"", "build"}, build, "Builds all projects");

#include <iostream>
void describe(std::vector<std::string> args) {
    if (args.size() != 3) {
        std::cout << "Usage: ccc desc <command name>" << std::endl;
        return;
    }

    for (auto cmd : ccc::cmds) {
        for (auto name : cmd->names) {
            if (name == args[2]) {
                std::cout << name << ": " << cmd->description << std::endl;
                return;
            }
        }
    }
}
ccc::command desc_cmd({"desc", "describe"}, describe, "Describes all projects");

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
    /* Traverse all projects. */
    for (auto project : ccc::projects) {
        project->init_func(project, args);

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

        project->exit_func(project, args);
    }
}
ccc::command clean_cmd("clean", clean, "Cleans all projects");

void version(std::vector<std::string> args) {
    args.push_back("--version");
    std::cout << "ccc version: " << "0.0.0.20250328" << std::endl;
}
ccc::command version_cmd("--version", version, "Prints the version");