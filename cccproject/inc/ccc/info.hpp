#ifndef __CCC_INFO_HPP__
#define __CCC_INFO_HPP__

#include <string>

namespace ccc {
namespace info {
inline std::string help_msg =
    "Usage: ccc <command> [option]...\n"
    "Built-in commands:\n"
    "    project              Only generate/update the executable project file "
    "without performing any other operations.(You can use this command with "
    "the '-g' option to generate the executable project file that can be "
    "debugged.)\n"
    "    build                Compile the project.\n"
    "    desc                 Get a description of what you want to know.\n"
    "    clean                Clean the product during the build process.\n"
    "    cccver               Get the version of ccc.\n"
    "    ccchelp              Get help information.\n"
    "Built-in options:\n"
    "    -g                   Generate debug information in the executable "
    "project file.\n"
    "    -O0/-O1/-O2/-O3/-Og  Select the optimized equivalent of the "
    "executable project file.\n";

inline std::string version = "0.0.1.20250507";

} // namespace info
} // namespace ccc

#endif
