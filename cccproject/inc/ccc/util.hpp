#ifndef __UTIL_H__
#define __UTIL_H__

#include <filesystem>
#include <numeric>
#include <string>
#include <vector>

namespace fs = std::filesystem;

inline std::string joinWithSpace(const std::vector<std::string>& vec) {
    return std::accumulate(vec.begin(), vec.end(), std::string(),
                           [](const std::string& a, const std::string& b) {
                               return a.empty() ? b : a + " " + b;
                           });
}

// AI generated.
inline std::string replaceCppWithO(const std::string& filename) {
    std::string result = filename;
    size_t pos = result.find(".cpp");
    if (pos != std::string::npos) {
        result.replace(pos, 4, ".o");
    }
    return result;
}

// AI generated.
inline bool directoryExists(const std::string& path) {
    return fs::exists(path) && fs::is_directory(path);
}

// AI generated.
inline std::string extractPath(const std::string& fullPath) {
    size_t found = fullPath.find_last_of("/\\");
    if (found != std::string::npos) {
        return fullPath.substr(0, found);
    }
    return "";
}

// AI generated.
inline bool compareFileModificationTime(const std::string& first,
                                        const std::string& second) {
    auto firstTime = fs::last_write_time(first);
    auto secondTime = fs::last_write_time(second);
    return firstTime < secondTime;
}

#endif