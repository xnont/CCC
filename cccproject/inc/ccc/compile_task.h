#ifndef __COMPILE_TASK_H__
#define __COMPILE_TASK_H__

#include "ccc/config.h"
#include "ccc/description.h"
#include "util/file.hpp"
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <thread>

namespace ccc {
class compile_task {
  public:
    compile_task() {};
    compile_task(std::string name, std::string description);

    std::string name;
    std::string output_path = "./build/";
    std::string obj_path = "./build/obj/";
    std::vector<std::string> source_files;
    std::vector<std::string> obj_files;

    ccc::config config;

    void handle(const ccc::config& project_cfg);

    /* The convenient utility functions provided by ccc. */

    /**
     * @brief Add file_path to the source_files variable.
     *
     * @param file_path The file_path need to be added to the source_files
     *        variable.
     */
    void add_source_file(const std::string& file_path);

    /**
     * @brief Add these file_paths to the source_files variable.
     *
     * @param file_paths The file_paths need to be added to the source_files
     *        variable.
     */
    void add_source_files(const std::initializer_list<std::string>& file_paths);

    /**
     * @brief Add all files in the dir_paths which have the suffix in suffixs to
     *        the the source_files variable.(Default recursion into subfolders.)
     *
     * @param dir_paths The dir_paths which have the files need to be added to.
     * @param suffixs The suffixs of the files need to be added to the
     *        source_files variable.
     * @param recursive Decide whether to recursively enter subfolders.
     */
    void add_source_files(const std::initializer_list<std::string>& dir_paths,
                          const std::initializer_list<std::string>& suffixs,
                          bool recursive = true);

  private:
    void compile_source_file(const ccc::config& project_cfg,
                             const std::string& source_file);
};
} // namespace ccc

#endif