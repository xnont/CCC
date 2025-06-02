#include "ccc/project.h"

#include "ccc/global.h"
#include "util/io.h"

#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

ccc::project::project(
    std::string name,
    auto (*init_func)(project*, std::string, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::string, std::vector<std::string>)->void,
    std::string description, std::source_location loc)
    : name(name), init_func(init_func), exit_func(exit_func), loc(loc) {
    // Add project
    ccc::global_var::add_project(this);
    // Add description
    ccc::global_var::add_desc(name, description);
}
void ccc::project::process() {
    for (size_t i = 0; i < tasks.size(); i++) {
        std::vector<std::string> path{this->name + " " + this->loc.file_name() +
                                      ":" + std::to_string(this->loc.line())};
        tasks[i]->set_toolchain(this->config);
        tasks[i]->compile(this->config, path);
        tasks[i]->link(this->config);

        if (i != tasks.size() - 1)
            ccc::io::println("");
    }
}

void ccc::project::add_task(ccc::compile_task* task) {
    tasks.push_back(std::shared_ptr<compile_task>(task->clone()));
}