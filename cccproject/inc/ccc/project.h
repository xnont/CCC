#ifndef __CCC_PROJECT_H__
#define __CCC_PROJECT_H__

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
    project(auto (*init_func)(project*, std::string, std::vector<std::string>)
                ->void,
            auto (*exit_func)(project*, std::string, std::vector<std::string>)
                ->void);

    project(std::string name,
            auto (*init_func)(project*, std::string, std::vector<std::string>)
                ->void,
            auto (*exit_func)(project*, std::string, std::vector<std::string>)
                ->void,
            std::string description);

    /* The init_func is executed before processing the project. */
    void (*init_func)(project*, std::string, std::vector<std::string>);
    /* The exit_func is executed after processing the project. */
    void (*exit_func)(project*, std::string, std::vector<std::string>);

    /* Used to pass parameters between init_func and exit_func. */
    void* arg;

    std::vector<ccc::library> libs;
    std::vector<ccc::execution> exes;

    /* The configuration of the project. */
    ccc::config config;

    /* Process the project.  */
    void process();

    /* The convenient utility functions provided by ccc. */

    /**
     * @brief Set the config object
     *
     * @param compiler The compiler is used to compile the librarys and the
     *                 executions in the project.
     * @param compile_flags The compile_flags apply globally.
     * @param linker The linker is used to link the librarys and the executions
     *               in the project.
     * @param link_flags The link_flags apply globally.
     */
    void set_config(std::string compiler,
                    std::vector<std::string> compile_flags, std::string linker,
                    std::vector<std::string> link_flags);

    /**
     * @brief Add a execution need to be compile to the project.
     *
     * @param exe The execution need to be added to the project.
     */
    void add_exe(ccc::execution exe);

    /**
     * @brief Add a library need to be compile to the project.
     *
     * @param lib The library need to be added to the project.
     */
    void add_lib(ccc::library lib);

  private:
};

/* All initialized project variables will be automatically added to this
 * variable and handed over to cccmain for unified processing. */
extern std::vector<ccc::project*> projects;

#define CCC_PROJECT(project_name, init_func, exit_func)                        \
    ccc::project project_name(init_func, exit_func)

} // namespace ccc

#endif