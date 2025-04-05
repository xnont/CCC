#ifndef __DEPENDENCE_H__
#define __DEPENDENCE_H__

namespace ccc {
class dependence_description {
  public:
    bool is_transmit;
    bool is_compile;

    dependence_description(bool is_transmit = true, bool is_compile = false) {
        this->is_transmit = is_transmit;
        this->is_compile = is_compile;
    }
};
} // namespace ccc

#endif