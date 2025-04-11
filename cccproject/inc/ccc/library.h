#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include "ccc/compile_task.h"

namespace ccc {
enum library_type { static_library, shared_library, dynamic_library };
enum system_type { windows_os, linux_os /*, macos */ };

class library : public ccc::compile_task {
  public:
    library(std::string name, ccc::library_type type, std::string description);

    library(const library& other)
        : ccc::compile_task(other), type(other.type) {}

    ccc::library* clone() const override { return new ccc::library(*this); }

    library_type type = library_type::static_library;

    /* The operating system on which the library runs defaults to the current
     * operating system. */
    system_type system =
#ifdef _WIN32
        system_type::windows_os;
#else
        system_type::linux_os;
#endif

    void process(const ccc::config& project_cfg) override;

    bool check(const ccc::config& project_cfg) override;
};
} // namespace ccc

#endif