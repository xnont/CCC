#ifndef __COMPILE_H__
#define __COMPILE_H__

#include "ccc/config.h"
#include <string>

namespace ccc {
class compile {
  public:
    std::string name;
    std::string description;
    std::string build_dir_path;
    std::vector<std::string> source_files;
    std::vector<std::string> obj_files;

    ccc::config cfg;

    void handle(ccc::config project_cfg);
};
} // namespace ccc

#endif