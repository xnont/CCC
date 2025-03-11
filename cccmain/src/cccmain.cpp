#include "ccc/project.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string target_cmd = "";

    // Determine the target command
    if (argc > 1)
        target_cmd = argv[1];

    // Run the target command
    for (auto cmd : ccc::cmds) {
        for (auto name : cmd->names) {
            if (name == target_cmd) {
                cmd->run(argc, argv);
                return 0;
            }
        }
    }

    std::cout << "Unknown command: " << target_cmd << std::endl;

    return 0;
}