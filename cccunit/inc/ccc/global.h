#ifndef __CCC_GLOBAL_H__
#define __CCC_GLOBAL_H__

#include "ccc/command.h"
#include "ccc/project.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace ccc {
class global_var {
  private:
    /* This static variable stores descriptions of
     * compile_tasks/projects/commands. */
    static std::unordered_map<std::string, std::vector<std::string>> descs;

    /* This static variable stores projects. */
    static std::vector<ccc::project*> projects;

    /* This static variable stores commands. */
    static std::unordered_map<
        std::string,
        std::unordered_map<ccc::command::priority, std::vector<ccc::command*>>>
        cmds;

  public:
    /**
     * @brief Add description to the global variable 'desc'. If the name already
     * has a corresponding description, these descriptions will be stored
     * uniformly.
     *
     * @param name Name of the compile_task/project/command.
     * @param desc Description of the compile_task/project/command.
     */
    static void add_desc(const std::string name, const std::string desc);

    /**
     * @brief Get descriptions from the global variable 'desc'.
     *
     * @param name Name of the compile_task/project/command.
     * @return std::vector<std::string>  Descriptions of the
     * compile_task/project/command. If the same name corresponds to more than
     * one description, all are returned.
     */
    static std::vector<std::string> get_descs(const std::string name);

    /**
     * @brief Add the commands based on name and priority.
     *
     * @param name The name of the command.
     * @param cmd The command to be added.
     * @param prio The priority of the command.
     */
    static void add_cmd(const std::string name, const ccc::command* cmd,
                        ccc::command::priority prio);

    /**
     * @brief Get the highest priority command by name. If there are multiple
     * commands with the highest priority of the target, a runtime exception
     * will be thrown. If the command corresponding to the target does not
     * exist, a runtime exception will be thrown.
     *
     * @param name Name of the command.
     * @return const ccc::command* The command.
     */
    static const ccc::command* get_cmd(const std::string name);

    /**
     * @brief Add a project to the global variable projects.
     *
     * @param project The project to be added.
     */
    static void add_project(ccc::project* project);

    /**
     * @brief Initialize all projects in the global variable projects.
     *
     * @param cmd The name of command to execute this method.
     * @param args The arguments passed to the project init function.
     */
    static void init_projects(std::string cmd, std::vector<std::string> args);

    /**
     * @brief Process all projects in the global variable projects.
     *
     */
    static void process_projects();

    /**
     * @brief Exit all projects in the global variable projects.
     *
     * @param cmd The name of command to execute this method.
     * @param args The arguments passed to the project exit function.
     */
    static void exit_projects(std::string cmd, std::vector<std::string> args);

    /**
     * @brief Get the copy of the global variable projects.
     *
     * @return std::vector<ccc::project*> The copy of the global variable
     * projects.
     */
    static const std::vector<ccc::project*> get_projects();
};

} // namespace ccc

#endif