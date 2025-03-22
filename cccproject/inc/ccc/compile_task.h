#ifndef __COMPILE_H__
#define __COMPILE_H__

#include "ThreadPool/ThreadPool.h"
#include "ccc/config.h"
#include "ccc/util.hpp"
#include <iostream>
#include <numeric>
#include <string>
#include <thread>

namespace ccc {
class compile_task {
  public:
    std::string name;
    std::string description;
    std::string output_path = "./build/";
    std::string obj_path = "./build/obj/";
    std::vector<std::string> source_files;
    std::vector<std::string> obj_files;

    ccc::config config;

    void handle(const ccc::config& project_cfg);

  private:
    void compile_source_file(const ccc::config& project_cfg,
                             const std::string& source_file);
};
} // namespace ccc

#endif