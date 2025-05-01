#ifndef __CCC_GLOBAL_H__
#define __CCC_GLOBAL_H__

#include "ccc/command.h"
#include "ccc/project.h"
#include <vector>

namespace ccc {
/* All initialized project variables will be automatically added to this
 * variable and handed over to cccmain for unified processing. */
extern std::vector<ccc::project*> projects;

extern std::unordered_map<std::string, ccc::command*> cmds;

extern std::unordered_map<std::string, std::string> descs;
} // namespace ccc

#endif