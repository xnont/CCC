#include "ccc/global.h"

#include "ccc/command.h"
#include "ccc/project.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace ccc {
/* Descriptions for projects, executions and libraries. */
std::unordered_map<std::string, std::string> descs;
/* Store all projects (automatically added to the global variable when the
 * project is initialized) */
std::vector<ccc::project*> projects;
/* Store all commands (automatically added to the global variable when the
 * command is initialized)*/
std::unordered_map<std::string, ccc::command*> cmds;
} // namespace ccc