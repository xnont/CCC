#ifndef __UTIL_H__
#define __UTIL_H__

#include <numeric>
#include <string>
#include <vector>

inline std::string joinWithSpace(const std::vector<std::string>& vec) {
    return std::accumulate(vec.begin(), vec.end(), std::string(),
                           [](const std::string& a, const std::string& b) {
                               return a.empty() ? b : a + " " + b;
                           });
}

#endif