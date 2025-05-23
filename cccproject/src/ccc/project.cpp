#include "ccc/project.h"

#include "ccc/global.h"

ccc::project::project(
    auto (*init_func)(project*, std::string, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::string, std::vector<std::string>)->void)
    : init_func(init_func), exit_func(exit_func) {
    // Add project
    ccc::global_var::add_project(this);
}

ccc::project::project(
    std::string name,
    auto (*init_func)(project*, std::string, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::string, std::vector<std::string>)->void,
    std::string description)
    : init_func(init_func), exit_func(exit_func) {
    // Add project
    ccc::global_var::add_project(this);
    // Add description
    ccc::global_var::add_desc(name, description);
}
void ccc::project::process() {
    for (const auto& task : tasks) {
        task->set_toolchain(this->config);
        task->compile(this->config);
        task->link(this->config);
    }
}

void ccc::project::add_task(ccc::compile_task* task) {
    tasks.push_back(std::shared_ptr<compile_task>(task->clone()));
}