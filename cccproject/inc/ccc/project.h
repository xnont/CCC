#ifndef __CCC_PROJECT_H__
#define __CCC_PROJECT_H__

#include "ccc/command.h"
#include "ccc/config.h"
#include "ccc/execution.h"
#include "ccc/library.h"
#include <iostream>
#include <vector>

namespace ccc {
class project : public ccc::config_manager {
  public:
    /**
     * @brief Construct a new project object.
     *
     * @param init_func The init_func is executed before processing the project.
     * @param exit_func The exit_func is executed after processing the project.
     */
    project(auto (*init_func)(project*, std::string, std::vector<std::string>)
                ->void,
            auto (*exit_func)(project*, std::string, std::vector<std::string>)
                ->void);
    /**
     * @brief Construct a new project object.
     *
     * @param name The name of the project.
     * @param init_func The init_func is executed before processing the project.
     * @param exit_func The exit_func is executed after processing the project.
     * @param description The description of the project.
     */
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

    /* Process the project.  */
    void process();

    /* The convenient utility functions provided by ccc. */

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

} // namespace ccc

#endif