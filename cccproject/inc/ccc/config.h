#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#include <string>
#include <vector>

namespace ccc {
class config {
  public:
    std::string compiler = "g++";
    std::vector<std::string> compile_flags;
    std::string linker = "g++";
    std::vector<std::string> link_flags;

    uint32_t thread_num = 0;
};
} // namespace ccc

#endif