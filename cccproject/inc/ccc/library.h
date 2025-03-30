#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include "ccc/compile_task.h"

namespace ccc {
enum library_type { static_library, shared_library, dynamic_library };

class library : public ccc::compile_task {
  public:
    library() : ccc::compile_task() {}

    library(std::string name, std::string description)
        : ccc::compile_task(name, description) {}

    library_type type;
    void process(ccc::config project_cfg);
};
} // namespace ccc

#endif