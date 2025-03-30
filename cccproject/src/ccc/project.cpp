#include "ccc/project.h"

namespace ccc {
std::vector<ccc::project*> projects;
}

ccc::project::project(
    auto (*init_func)(project*, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::vector<std::string>)->void)
    : init_func(init_func), exit_func(exit_func), arg(nullptr) {
    // Add project
    ccc::projects.push_back(this);
}

ccc::project::project(
    std::string name,
    auto (*init_func)(project*, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::vector<std::string>)->void,
    std::string description)
    : init_func(init_func), exit_func(exit_func) {
    // Add project
    ccc::projects.push_back(this);
    // Add description
    ccc::descs[name] = description;
}
void ccc::project::process() {

    /*  Check config */
    if (!this->check()) {
        return;
    }

    for (auto& lib : libs) {
        lib.process(this->config);
    }

    for (auto& exe : exes) {
        exe.process(this->config);
    }
}
bool ccc::project::check() {
    bool status = true;

    return status;
}