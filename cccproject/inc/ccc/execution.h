#ifndef __EXECUTION_H__
#define __EXECUTION_H__

#include "ccc/compile.h"
#include "ccc/util.h"
#include <iostream>

namespace ccc {
class execution : public ccc::compile {
  public:
    void process(ccc::config project_cfg);
};
} // namespace ccc

#endif