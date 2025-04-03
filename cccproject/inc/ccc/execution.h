#ifndef __EXECUTION_H__
#define __EXECUTION_H__

#include "ccc/compile_task.h"
#include "util/file.hpp"
#include <iostream>

namespace ccc {
class execution : public ccc::compile_task {
  public:
    execution(std::string name, std::string description);

    void process(const ccc::config& project_cfg) override;

    bool check(const ccc::config& project_cfg) override;
};
} // namespace ccc

#endif