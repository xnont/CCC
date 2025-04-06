#include "ccc/command.h"

ccc::command::command(std::string name,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description)
    : run(run) {
    // Register the command.
    cmds[name] = this;
    ccc::descs[name] = description;
}

ccc::command::command(std::initializer_list<std::string> names,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description)
    : run(run) {
    // Register the command.
    for (auto name : names) {
        cmds[name] = this;
        ccc::descs[name] = description;
    }
}