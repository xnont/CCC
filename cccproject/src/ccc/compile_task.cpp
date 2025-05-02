#include "ccc/compile_task.h"
#include "ccc/format.h"
#include "ccc/global.h"
#include "util/io.h"

ccc::compile_task::compile_task(std::string name, std::string description) {
    this->name = name;
    // Add description
    ccc::descs[name] = description;
}
void ccc::compile_task::compile(const ccc::config& project_cfg) {
    // Process the dependencies.
    for (auto& [dep, dep_desc] : dependencies) {
        // If the dependency does not exist and the is_compile is true, process
        // it.
        if (!fs::exists(dep->output_path + "/" + dep->name) &&
            dep_desc.is_compile) {
            dep->compile(project_cfg);
            dep->link(project_cfg);
        }

        // Add header paths.
        for (auto& header_folder_path : dep->config.header_folder_paths) {
            this->config.header_folder_paths.push_back(header_folder_path);
        }
    }

    // Create a thread pool.
    unsigned int core_num = std::thread::hardware_concurrency();
    size_t thread_num =
        this->config.thread_num != 0
            ? this->config
                  .thread_num // Use the this.config thread num if it is not 0.

        : project_cfg.thread_num != 0
            ? project_cfg
                  .thread_num // Use the project.config thread num if it
                              // is not 0 and the this.config thread num is 0.

        : core_num != 0 ? core_num // If all the above variables are set, use
                                   // the kernel number as the parameter.

                        : 1; // If the number of cores cannot be obtained,
                             // set the parameter to 1.

    // Merge the project and task configurations.
    this->config.compile_flags.insert(this->config.compile_flags.end(),
                                      project_cfg.compile_flags.begin(),
                                      project_cfg.compile_flags.end());
    this->config.header_folder_paths.insert(
        this->config.header_folder_paths.end(),
        project_cfg.header_folder_paths.begin(),
        project_cfg.header_folder_paths.end());
    this->config.macros.insert(this->config.macros.end(),
                               project_cfg.macros.begin(),
                               project_cfg.macros.end());

    // Create a mutex and condition variable for thread synchronization.
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::atomic<size_t> active_threads(0);

    // Iteratively compile all source files.
    for (const auto& source_file : source_files) {
        // Wait until there is an available thread slot.
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return active_threads < thread_num; });

        // Increment the active thread count.
        active_threads++;

        // Launch a new thread to compile the source file.
        threads.emplace_back(
            [this, &project_cfg, source_file, &active_threads, &cv]() {
                compile_source_file(project_cfg, source_file);

                // Decrement the active thread count and notify the waiting
                // threads.
                active_threads--;
                cv.notify_one();
            });
    }

    // Wait for all threads to finish.
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    for (auto& [dep, dep_desc] : dependencies) {
        // If the is_transmit is true, add the dependency to the obj_files list.
        if (dep_desc.is_transmit) {
            this->obj_files.push_back(dep->output_path + "/" + dep->name);
        }
    }
}

static std::mutex compile_mtx;
void ccc::compile_task::compile_source_file(const ccc::config& project_cfg,
                                            const std::string& source_file) {

    // Get the obj file path.
    std::string obj_file_path =
        this->obj_path + "/" +
        ((target_os == windows_os) ? changeFileExtension(source_file, ".obj")
         : (target_os == linux_os) ? changeFileExtension(source_file, ".o")
                                   : changeFileExtension(source_file, ".o"));

    // Add the obj file to the list.
    {
        std::lock_guard<std::mutex> lock(compile_mtx);
        obj_files.push_back(obj_file_path);
    }

    // If the obj file exists and is newer than the source file, skip it.
    if (fs::exists(obj_file_path) &&
        compareFileModificationTime(source_file, obj_file_path)) {
        return;
    }

    // Get the target folder(The storage path of the obj file.)
    std::string target_folder = extractPath(obj_file_path);
    // If the folder doesn't exist, create it.
    if (!directoryExists(target_folder)) {
        fs::create_directories(target_folder);
    }

    // Replace the placeholders in the format string.
    auto replacements =
        std::unordered_map<std::string, std::vector<std::string>>{
            {"COMPILER",
             {this->config.compiler.length() != 0  ? this->config.compiler
              : project_cfg.compiler.length() != 0 ? project_cfg.compiler
                                                   : "g++"}},
            {"SOURCE_FILE", {source_file}},
            {"OBJECT_FILE", {obj_file_path}},
            {"COMPILER_FLAGS",
             {this->config.compile_flags.begin(),
              this->config.compile_flags.end()}},
            {"HEADER_FOLDER",
             {this->config.header_folder_paths.begin(),
              this->config.header_folder_paths.end()}},
            {"MACRO", {this->config.macros.begin(), this->config.macros.end()}},

            {"HEADER_FOLDERS",
             {this->config.header_folder_paths.begin(),
              this->config.header_folder_paths.end()}},
            {"MACROS",
             {this->config.macros.begin(), this->config.macros.end()}}};
    std::string cmd = this->compile_format.replace(replacements);

    // Execute the command.
    ccc::io::exec_command(cmd, project_cfg.is_print && this->config.is_print,
                          project_cfg.is_print && this->config.is_print);
}

void ccc::compile_task::add_source_file(const std::string& file_path) {
    this->source_files.push_back(file_path);
    return;
}

void ccc::compile_task::add_source_files(
    const std::initializer_list<std::string>& file_paths) {
    for (auto file_path : file_paths) {
        this->source_files.push_back(file_path);
    }
    return;
}

void ccc::compile_task::add_source_files(
    const std::initializer_list<std::string>& dir_paths,
    const std::initializer_list<std::string>& suffixs, bool recursive) {
    namespace fs = std::filesystem;

    // Convert suffix initializer_list to vector for easier processing
    std::vector<std::string> suffix_vector(suffixs);

    // Process each directory in the input list
    for (const auto& dir : dir_paths) {
        const fs::path dir_path(dir);

        // Skip invalid directories
        if (!fs::exists(dir_path))
            continue; // Path doesn't exist
        if (!fs::is_directory(dir_path))
            continue; // Path isn't a directory

        std::vector<fs::directory_entry> entries;

        // Collect directory entries (recursive or non-recursive)
        if (recursive) {
            for (const auto& entry :
                 fs::recursive_directory_iterator(dir_path)) {
                entries.emplace_back(entry);
            }
        } else {
            for (const auto& entry : fs::directory_iterator(dir_path)) {
                entries.emplace_back(entry);
            }
        }

        // Process collected directory entries
        for (const auto& entry : entries) {
            // Skip non-files and symlinks
            if (!entry.is_regular_file() || entry.is_symlink())
                continue;

            // Extract file extension (includes the dot, e.g., ".cpp")
            const std::string ext = entry.path().extension().string();

            // Skip files without extensions
            if (!ext.empty()) {
                // Check if extension matches any of the target suffixes
                for (const auto& suffix : suffix_vector) {
                    if (ext == suffix) {

                        // Add normalized path to source files list
                        source_files.emplace_back(
                            entry.path().lexically_normal().string());
                        break; // No need to check other suffixes
                    }
                }
            }
        }
    }
}
void ccc::compile_task::add_source_files(
    const std::initializer_list<std::string>& dir_paths,
    auto judge(const std::string&)->bool, bool recursive) {
    namespace fs = std::filesystem;

    // Iterate through each directory path provided in dir_paths
    for (const auto& dir : dir_paths) {
        const fs::path dir_path(dir);

        // Skip if the directory does not exist or is not a valid directory
        if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
            continue;

        // If recursive flag is true, use recursive_directory_iterator
        if (recursive) {
            for (fs::recursive_directory_iterator it(dir_path);
                 it != fs::recursive_directory_iterator(); ++it) {
                const auto& entry = *it;

                // Check if the entry is a regular file and not a symbolic link
                if (entry.is_regular_file() && !entry.is_symlink()) {
                    std::string path = entry.path().lexically_normal().string();

                    // If the judge function returns true for this file, add it
                    // to source_files
                    if (judge(path))
                        source_files.push_back(path);
                }
            }
        } else { // If recursive flag is false, use directory_iterator
            for (fs::directory_iterator it(dir_path);
                 it != fs::directory_iterator(); ++it) {
                const auto& entry = *it;

                // Check if the entry is a regular file and not a symbolic link
                if (entry.is_regular_file() && !entry.is_symlink()) {
                    std::string path = entry.path()
                                           .lexically_normal()
                                           .string(); // Normalize the file path

                    // If the judge function returns true for this file, add it
                    // to source_files
                    if (judge(path))
                        source_files.push_back(path);
                }
            }
        }
    }
}
void ccc::compile_task::remove_source_file(const std::string& file_path) {
    size_t index = 0;
    for (size_t i = 0; i < source_files.size(); ++i) {
        if (source_files[i] != file_path) {
            // Move non-matching elements to front
            if (index != i) {
                source_files[index] = std::move(source_files[i]);
            }
            ++index;
        }
    }
    // Truncate vector to new size
    source_files.resize(index);
}

void ccc::compile_task::remove_source_files(
    const std::initializer_list<std::string>& file_paths) {
    // Iterate through paths to remove
    for (const auto& path : file_paths) {
        // Reimplement single removal logic for each path
        size_t index = 0;
        for (size_t i = 0; i < source_files.size(); ++i) {
            if (source_files[i] != path) {
                // Compact array by moving kept elements
                if (index != i) {
                    source_files[index] = std::move(source_files[i]);
                }
                ++index;
            }
        }
        source_files.resize(index);
    }
}

int ccc::compile_task::remove_source_files(bool (*judge)(const std::string&)) {
    int removed_count = 0;
    size_t index = 0;

    // Process all elements
    for (size_t i = 0; i < source_files.size(); ++i) {
        if (!judge(
                source_files[i])) { // Keep elements that don't match condition
            // Shift elements to maintain order
            if (index != i) {
                source_files[index] = std::move(source_files[i]);
            }
            ++index;
        } else { // Count removed elements
            ++removed_count;
        }
    }
    // Resize container after removal
    source_files.resize(index);
    return removed_count;
}

bool ccc::compile_task::find_source_file(const std::string& file_path) {
    // Implement linear search manually
    for (const auto& path : source_files) {
        if (path == file_path) {
            return true; // Early return when found
        }
    }
    return false; // Return false if not found
}