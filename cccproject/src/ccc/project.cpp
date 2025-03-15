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
void ccc::project::process() {

    /*  Check config */
    if (!this->check()) {
        return;
    }

    // for (auto& lib : libs) {
    //     lib.process();
    // }

    for (auto& exe : exes) {
        exe.process(this->config);
    }
}
bool ccc::project::check() {
    bool status = true;

    /* Check project name */
    if (this->name.length() == 0) {
        std::cout << "Warnning: No project name set." << std::endl;
    }

    /* Check compiler */
    if (this->config.compiler.length() == 0) {
        std::cout << "Error: No compiler set for project "
                  << (this->name.length() != 0 ? this->name : "unnamed")
                  << std::endl;
        status = false;
    }

    /* Check linker(If the linker is not set, we will set it to be the same as
     * the compiler.) */
    if (this->config.linker.length() == 0 &&
        this->config.compiler.length() != 0) {
        std::cout << "Warnning: No linker set for project "
                  << (this->name.length() != 0 ? this->name : "unnamed")
                  << std::endl;
        std::cout << "We will set the linker to be the same as the compiler."
                  << std::endl;
        this->config.linker = this->config.compiler;
    }

    return status;
}