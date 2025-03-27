#ifndef __EXECUTION_H__
#define __EXECUTION_H__

#include "ccc/compile_task.h"
#include "util/file.hpp"
#include <iostream>

namespace ccc {
class execution : public ccc::compile_task {
  public:
    void process(ccc::config project_cfg);
};
} // namespace ccc

#endif