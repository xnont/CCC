#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <vector>

namespace ccc {
/* This class defines how to register a command. */
class command {
  public:
    /* Definition of Command Priority */
    enum class priority { low, normal, high };

    /* The running function of the command  */
    void (*run)(std::vector<std::string> args);

    /* The priority of the command */
    priority prio;

    /**
     * @brief Register a command. When there are multiple commands for a certain
     * name, the command with the highest priority will be executed. You can
     * register multiple commands for each name, but if there are multiple
     * commands in the highest priority of the corresponding name, it will cause
     * an exception to be thrown when running the command.
     *
     * @param name The name of the command(ccc determines the command to be
     *             executed through name matching).
     * @param run The function to run command.
     * @param description The description of the command.
     * @param prio The priority of the command.
     */
    command(std::string name, auto (*run)(std::vector<std::string> args)->void,
            std::string description,
            ccc::command::priority prio = ccc::command::priority::normal);

    /**
     * @brief Register a command. When there are multiple commands for a certain
     * name, the command with the highest priority will be executed. You can
     * register multiple commands for each name, but if there are multiple
     * commands in the highest priority of the corresponding name, it will cause
     * an exception to be thrown when running the command.
     *
     * @param names The names of the command(ccc determines the command to be
     *              executed through name matching).
     * @param run The function to run command.
     * @param description The description of the command.
     * @param prio The priority of the command.
     */
    command(std::initializer_list<std::string> names,
            auto (*run)(std::vector<std::string> args)->void,
            std::string description,
            ccc::command::priority prio = ccc::command::priority::normal);
};

} // namespace ccc

#endif