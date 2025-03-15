#ifndef __CCC_H__
#define __CCC_H__

#include "ccc/command.h"
#include "ccc/config.h"
#include "ccc/execution.h"
#include "ccc/library.h"
#include <iostream>
#include <vector>

namespace ccc {

enum func_status {};

class project {
  public:
    project(auto (*init_func)(project*, std::vector<std::string>)->void,
            auto (*exit_func)(project*, std::vector<std::string>)->void);

    std::string name;

    /* The init_func is executed before processing the project. */
    void (*init_func)(project*, std::vector<std::string>);
    /* The exit_func is executed after processing the project. */
    void (*exit_func)(project*, std::vector<std::string>);

    /* Used to pass parameters between init_func and exit_func. */
    void* arg;

    // std::vector<ccc::library> libs;
    std::vector<ccc::execution> exes;

    /* The configuration of the project. */
    ccc::config config;

    /* Process the project.  */
    void process();

  private:
    bool check();
};

/* All initialized project variables will be automatically added to this
 * variable and handed over to cccmain for unified processing. */
extern std::vector<ccc::project*> projects;

#define CCC_PROJECT(project_name, init_func, exit_func)                        \
    ccc::project project_name(init_func, exit_func)

} // namespace ccc

#endif