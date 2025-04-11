#ifndef __DEPENDENCY_H__
#define __DEPENDENCY_H__

namespace ccc {
class dependency_description {
  public:
    bool is_transmit;
    bool is_compile;

    dependency_description(bool is_transmit = true, bool is_compile = false) {
        this->is_transmit = is_transmit;
        this->is_compile = is_compile;
    }
};
} // namespace ccc

#endif