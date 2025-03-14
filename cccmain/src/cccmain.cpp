#include "ccc/project.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string target_cmd = "";

    // Determine the target command
    if (argc > 1)
        target_cmd = argv[1];

    // Get the arguments
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    // Run the target command
    for (auto cmd : ccc::cmds) {
        for (auto name : cmd->names) {
            if (name == target_cmd) {
                cmd->run(args);
                return 0;
            }
        }
    }

    std::cout << "Unknown command: " << target_cmd << std::endl;

    return 0;
}