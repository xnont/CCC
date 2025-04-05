#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#include <string>
#include <vector>

namespace ccc {
class config {
  public:
    /* The compiler */
    std::string compiler = "g++";
    /* The compile flags */
    std::vector<std::string> compile_flags;
    /* The linker */
    std::string linker = "g++";
    /* The link flags */
    std::vector<std::string> link_flags;

    /* The header folder paths */
    std::vector<std::string> header_folder_paths;

    /* The macros */
    std::vector<std::string> macros;

    uint32_t thread_num = 0;
};

class config_manager {
  public:
    /* The configuration. */
    ccc::config config;

    /**
     * @brief Set the config object from another config object.
     *
     * @param cfg The configuration need to be copy.
     */
    virtual void set_config(const ccc::config& cfg) final;

    /**
     * @brief Set the config object
     *
     * @param compiler The compiler is used to compile.
     * @param compile_flags The compile_flags is used to compile.
     * @param linker The linker is used to link.
     * @param link_flags The link_flags is used to link.
     */
    virtual void set_config(const std::string& compiler,
                            const std::vector<std::string>& compile_flags,
                            const std::string& linker,
                            const std::vector<std::string>& link_flags) final;

    /**
     * @brief Set the compiler object
     *
     * @param compiler The compiler is used to compile.
     */
    virtual void set_compiler(const std::string& compiler) final;

    /**
     * @brief Set the compile flags object
     *
     * @param compile_flags The compile_flags is used to compile.
     */
    virtual void
    set_compile_flags(const std::vector<std::string>& compile_flags) final;

    /**
     * @brief Set the linker object
     *
     * @param linker The linker is used to link.
     */
    virtual void set_linker(const std::string& linker) final;

    /**
     * @brief Set the link flags object
     *
     * @param link_flags The link_flags is used to link.
     */
    virtual void
    set_link_flags(const std::vector<std::string>& link_flags) final;

    /**
     * @brief Set the thread num object
     *
     * @param num The thread num when compile.
     */
    virtual void set_thread_num(uint32_t num) final;
};

} // namespace ccc

#endif