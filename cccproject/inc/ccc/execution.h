#ifndef __EXECUTION_H__
#define __EXECUTION_H__

#include "ccc/compile_task.h"
#include "util/file.hpp"
#include <iostream>

namespace ccc {
class execution : public ccc::compile_task {
  public:
    /**
     * @brief Construct a new execution object.
     *
     * @param name The name of the execution task.
     * @param description The description of the execution task.
     */
    execution(std::string name, std::string description);

    /**
     * @brief Construct a new execution object by copy.
     *
     * @param other The execution task to be copied.
     */
    execution(const execution& other) : ccc::compile_task(other) {}

    /**
     * @brief Clone the execution task.
     *
     * @return ccc::execution* The cloned execution task.
     */
    ccc::execution* clone() const override { return new execution(*this); }

    /**
     * @brief Set the toolchain object.
     *
     * @param project_cfg The project configuration.
     */
    void set_toolchain(const ccc::config& project_cfg) override;

    /**
     * @brief Rewrite the link method of the compile_task class, call the
     *        compile method in it, and link the intermediate products.
     *
     * @param project_cfg The configuration of the project.
     */
    void link(const ccc::config& project_cfg) override;

    /**
     * @brief Check if the execution task is valid.
     *
     * @param project_cfg The configuration of the project.
     * @return true The execution task is valid.
     * @return false The execution task is invalid.
     */
    bool check(const ccc::config& project_cfg) override;
};
} // namespace ccc

#endif