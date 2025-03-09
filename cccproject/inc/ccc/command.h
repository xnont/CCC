#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <vector>

namespace ccc {
class command {
  public:
    /* Name of the command  */
    std::string name;

    /* Description of the command  */
    std::string description;

    /* Run the command  */
    void (*run)(int argc, char** argv);

    command(std::string name, auto (*run)(int, char**)->void,
            std::string description)
        : name(name), run(run), description(description) {}
};

extern std::vector<ccc::command> cmds;

} // namespace ccc

#endif