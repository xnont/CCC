#include "ccc/command.h"

namespace ccc {
std::vector<ccc::command*> cmds;
}

ccc::command::command(std::string name, auto (*run)(int, char**)->void,
                      std::string description)
    : name(name), run(run), description(description) {
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
ccc::command build_cmd("build", build, "Builds all projects");

#include <iostream>
void describe(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: ccc desc <command name>" << std::endl;
        return;
    }

    for (auto cmd : ccc::cmds) {
        if (cmd->name == argv[2]) {
            std::cout << cmd->name << ": " << cmd->description << std::endl;
            return;
        }
    }
}
ccc::command desc_cmd("desc", describe, "Describes all projects");