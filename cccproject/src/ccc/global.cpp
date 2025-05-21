#include "ccc/global.h"

#include "ccc/command.h"
#include "ccc/project.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace ccc {
/* Store all projects (automatically added to the global variable when the
 * project is initialized) */
std::vector<ccc::project*> projects;
/* Store all commands (automatically added to the global variable when the
 * command is initialized)*/
std::unordered_map<std::string, ccc::command*> cmds;
} // namespace ccc

std::unordered_map<std::string, std::vector<std::string>>
    ccc::global_var::descs;

void ccc::global_var::add_desc(std::string name, std::string desc) {
    descs[name].push_back(desc);
}

std::vector<std::string> ccc::global_var::get_descs(std::string name) {
    return descs[name];
}