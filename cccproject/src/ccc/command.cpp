#include "ccc/command.h"

namespace ccc {
std::vector<ccc::command*> cmds;
}

ccc::command::command(std::string name, auto (*run)(int, char**)->void,
                      std::string description)
    : run(run), description(description) {
    this->names.push_back(name);
    cmds.push_back(this);
}

ccc::command::command(std::vector<std::string> names,
                      auto (*run)(int, char**)->void, std::string description)
    : run(run), description(description) {
    this->names = names;
    cmds.push_back(this);
}
#include "ccc/project.h"
void build(int argc, char** argv) {
    for (auto project : ccc::projects) {
        project->init_func(project, argc, argv);
        project->process();
        project->exit_func(project, argc, argv);
    }
}
ccc::command build_cmd(std::vector<std::string>{"", "build"}, build,
                       "Builds all projects");

#include <iostream>
void describe(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: ccc desc <command name>" << std::endl;
        return;
    }

    for (auto cmd : ccc::cmds) {
        for (auto name : cmd->names) {
            if (name == argv[2]) {
                std::cout << name << ": " << cmd->description << std::endl;
                return;
            }
        }
    }
}
ccc::command desc_cmd(std::vector<std::string>{"desc", "describe"}, describe,
                      "Describes all projects");