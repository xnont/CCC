#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <unordered_map>
#include <vector>

#include "ccc/description.h"

namespace ccc {
class command {
  public:
    /* Run the command  */
    void (*run)(std::vector<std::string> args);

    command(std::string name, auto (*run)(std::vector<std::string> args)->void,
            std::string description);

    command(std::initializer_list<std::string> names,
            auto (*run)(std::vector<std::string> args)->void,
            std::string description);
};

extern std::unordered_map<std::string, ccc::command*> cmds;

} // namespace ccc

#endif