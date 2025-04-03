#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include "ccc/compile_task.h"

namespace ccc {
enum library_type { static_library, shared_library, dynamic_library };

class library : public ccc::compile_task {
  public:
    library(std::string name, ccc::library_type type, std::string description);

    library_type type = library_type::static_library;

    void process(const ccc::config& project_cfg) override;

    bool check(const ccc::config& project_cfg) override;
};
} // namespace ccc

#endif