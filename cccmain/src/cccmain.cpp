#include "ccc/project.h"
#include <iostream>
#include <string>
#include <vector>

namespace ccc {
std::unordered_map<std::string, std::string> descs;
std::vector<ccc::project*> projects;
std::unordered_map<std::string, ccc::command*> cmds;
} // namespace ccc

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
    if (ccc::cmds.find(target_cmd) != ccc::cmds.end()) {
        ccc::cmds[target_cmd]->run(args);
        return 0;
    }

    std::cout << "Unknown command: " << target_cmd << std::endl;
    return -1;
}