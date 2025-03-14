#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>

namespace ccc {
class config {
  public:
    std::string compiler;
    std::vector<std::string> compile_flags;
    std::string linker;
    std::vector<std::string> link_flags;
};
} // namespace ccc

#endif