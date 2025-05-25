#include "ccc/command.h"

#include "ccc/global.h"

ccc::command::command(std::string name,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description, ccc::command::priority prio)
    : run(run) {
    // Add description.
    ccc::global_var::add_desc(name, description);

    // Register the command.
    ccc::global_var::add_cmd(name, this, prio);
}

ccc::command::command(std::initializer_list<std::string> names,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description, ccc::command::priority prio)
    : run(run) {
    // Register the command.
    for (auto name : names) {
        // Add description.
        ccc::global_var::add_desc(name, description);

        // Register the command.
        ccc::global_var::add_cmd(name, this, prio);
    }
}