#include "ccc/project.h"

ccc::project::project(
    auto (*init_func)(project*, std::string, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::string, std::vector<std::string>)->void)
    : init_func(init_func), exit_func(exit_func) {
    // Default config
    this->config.compiler = "g++";
    this->config.linker = "g++";
    // Add project
    ccc::projects.push_back(this);
}

ccc::project::project(
    std::string name,
    auto (*init_func)(project*, std::string, std::vector<std::string>)->void,
    auto (*exit_func)(project*, std::string, std::vector<std::string>)->void,
    std::string description)
    : init_func(init_func), exit_func(exit_func) {
    // Default config
    this->config.compiler = "g++";
    this->config.linker = "g++";
    // Add project
    ccc::projects.push_back(this);
    // Add description
    ccc::descs[name] = description;
}
void ccc::project::process() {
    for (auto& lib : libs)
        if (lib.check(this->config))
            lib.process(this->config);

    for (auto& exe : exes)
        if (exe.check(this->config))
            exe.process(this->config);
}

void ccc::project::add_exe(ccc::execution exe) {
    exes.push_back(std::move(exe));
}

void ccc::project::add_lib(ccc::library lib) { libs.push_back(std::move(lib)); }