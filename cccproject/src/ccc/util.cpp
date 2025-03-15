#include "ccc/util.h"

std::string ccc::joinWithSpace(const std::vector<std::string>& vec) {
    return std::accumulate(vec.begin(), vec.end(), std::string(),
                           [](const std::string& a, const std::string& b) {
                               return a.empty() ? b : a + " " + b;
                           });
}