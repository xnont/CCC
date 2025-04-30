#include "util/io.h"
#include <cstdio>
#include <mutex>

static std::mutex out_mutex;

void ccc::io::print(const std::string& message) {
    std::lock_guard<std::mutex> lock(out_mutex);
    printf("%s", message.c_str());
}

void ccc::io::print(const char* message) {
    std::lock_guard<std::mutex> lock(out_mutex);
    printf("%s", message);
}

void ccc::io::println(const std::string& message) {
    std::lock_guard<std::mutex> lock(out_mutex);
    printf("%s\n", message.c_str());
}

void ccc::io::println(const char* message) {
    std::lock_guard<std::mutex> lock(out_mutex);
    printf("%s\n", message);
}

static std::mutex exec_out_mutex;

bool ccc::io::exec_command(const std::string& cmd, bool is_cmd_print,
                           bool is_res_print) {
    // Create the pipe
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(exec_out_mutex);

        // Print the command
        if (is_cmd_print) {
            ccc::io::println(cmd);
        }

        // Print the result
        if (is_res_print) {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                ccc::io::print(buffer);
            }
        }
        // Discard the result, and prevent pipeline blockage.
        else {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
                ;
        }
    }

    pclose(pipe);
    return true;
}