#include "ccc/config.h"

void ccc::config_manager::set_config(const ccc::config& cfg) { return; }

void ccc::config_manager::set_config(
    const std::string& compiler, const std::vector<std::string>& compile_flags,
    const std::string& linker, const std::vector<std::string>& link_flags) {
    return;
}

void ccc::config_manager::set_compiler(const std::string& compiler) { return; }

void ccc::config_manager::set_compile_flags(
    const std::vector<std::string>& compile_flags) {
    return;
}

void ccc::config_manager::set_linker(const std::string& linker) { return; }

void ccc::config_manager::set_link_flags(
    const std::vector<std::string>& link_flags) {
    return;
}

void ccc::config_manager::set_thread_num(uint32_t num) { return; }