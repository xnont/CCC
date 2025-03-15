#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>

namespace ccc {
class config {
  public:
    std::string compiler;
    std::string linker;
};
} // namespace ccc

#endif