#include "ccc/project.h"

namespace ccc {
std::vector<ccc::project*> projects;
}

ccc::project::project(
    auto (*init_func)(project*, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::vector<std::string>)->void)
    : init_func(init_func), exit_func(exit_func), arg(nullptr) {
    ccc::projects.push_back(this);
}
void ccc::project::process() {}