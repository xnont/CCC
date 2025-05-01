#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <unordered_map>
#include <vector>

#include "ccc/description.h"

namespace ccc {
/* This class defines how to register a command. */
class command {
  public:
    /* Run the command  */
    void (*run)(std::vector<std::string> args);

    /**
     * @brief Construct a new command object.
     *
     * @param name The name of the command(ccc determines the command to be
     *             executed through name matching).
     * @param run The function to run command.
     * @param description The description of the command.
     */
    command(std::string name, auto (*run)(std::vector<std::string> args)->void,
            std::string description);

    /**
     * @brief Construct a new command object
     *
     * @param names The names of the command(ccc determines the command to be
     *              executed through name matching).
     * @param run The function to run command.
     * @param description The description of the command.
     */
    command(std::initializer_list<std::string> names,
            auto (*run)(std::vector<std::string> args)->void,
            std::string description);
};

} // namespace ccc

#endif