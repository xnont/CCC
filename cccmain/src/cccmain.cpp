#include "ccc/command.h"
#include "ccc/global.h"
#include "ccc/info.hpp"
#include "util/io.h"

#include <iostream>
#include <string>
#include <vector>

#define RED "\033[31m"
#define RESET "\033[0m"

int main(int argc, char* argv[]) {
    // If there is no argument, print the help message.
    if (argc == 1) {
        ccc::io::print(ccc::info::help_msg);
        return -1;
    }

    std::string target_cmd = "";
    // Determine the target command
    target_cmd = argv[1];

    // Get the arguments
    std::vector<std::string> args;
    for (int i = 2; i < argc; i++)
        args.push_back(argv[i]);

    // Run the target command
    if (auto cmd = ccc::global_var::get_cmd(target_cmd); cmd != nullptr) {
        try {
            cmd->run(args);
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