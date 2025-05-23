#include "ccc/command.h"

#include "ccc/global.h"
#include <iostream>

ccc::command::command(std::string name,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description)
    : run(run) {
    // Add description.
    ccc::global_var::add_desc(name, description);

    // Check if the command is defined.
    if (ccc::global_var::get_cmd(name) != nullptr)
        std::cout << "ccc: \033[33mwarn:\033[0m Command " << name
                  << " is defined repeatedly." << std::endl;

    // Register the command.
    ccc::global_var::add_cmd(name, this);
}

ccc::command::command(std::initializer_list<std::string> names,
                      auto (*run)(std::vector<std::string> args)->void,
                      std::string description)
    : run(run) {
    // Register the command.
    for (auto name : names) {
        // Add description.
        ccc::global_var::add_desc(name, description);

        // Check if the command is defined.
        if (ccc::global_var::get_cmd(name) != nullptr)
            std::cout << "ccc: \033[33mwarn:\033[0m Command " << name
                      << " is defined repeatedly." << std::endl;

        // Register the command.
        ccc::global_var::add_cmd(name, this);
    }
}