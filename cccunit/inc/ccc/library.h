#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include "ccc/compile_task.h"

namespace ccc {
enum library_type { static_library, shared_library };

class library : public ccc::compile_task {
  public:
    /**
     * @brief Construct a new library object.
     *
     * @param name The name of the library.
     * @param type The type of the library.
     * @param description The description of the library.
     */
    library(std::string name, ccc::library_type type, std::string description);

    /**
     * @brief Construct a new library object by copying another library object.
     *
     * @param other The library object to be copied.
     */
    library(const library& other)
        : ccc::compile_task(other), type(other.type) {}

    /**
     * @brief Clone the library object.
     *
     * @return ccc::library* The cloned library object.
     */
    ccc::library* clone() const override { return new ccc::library(*this); }

    /**
     * @brief Add information to the parent task based on different library
     * types.
     *
     * @param super The parent task.
     */
    void transmit(ccc::compile_task& super) override;

    library_type type = library_type::static_library;

    /**
     * @brief Set the toolchain object.
     *
     * @param project_cfg The configuration of the project.
     */
    void set_toolchain(const ccc::config& project_cfg) override;

    /**
     * @brief Rewrite the link method of the compile_task class, call the
     *        compile method in it, and link the intermediate products.
     *
     * @param project_cfg The configuration of the project.
     */
    void link(const ccc::config& project_cfg) override;
};
} // namespace ccc

#endif