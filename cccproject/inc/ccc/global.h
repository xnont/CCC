#ifndef __CCC_GLOBAL_H__
#define __CCC_GLOBAL_H__

#include "ccc/command.h"
#include "ccc/project.h"
#include <string>
#include <vector>

namespace ccc {
/* All initialized project variables will be automatically added to this
 * variable and handed over to cccmain for unified processing. */
extern std::vector<ccc::project*> projects;

extern std::unordered_map<std::string, ccc::command*> cmds;

class global_var {
  private:
    /* This static variable stores descriptions of
     * compile_tasks/projects/commands. */
    static std::unordered_map<std::string, std::vector<std::string>> descs;

  public:
    /**
     * @brief Add description to the global variable 'desc'. If the name already
     * has a corresponding description, these descriptions will be stored
     * uniformly.
     *
     * @param name Name of the compile_task/project/command.
     * @param desc Description of the compile_task/project/command.
     */
    static void add_desc(std::string name, std::string desc);

    /**
     * @brief Get descriptions from the global variable 'desc'.
     *
     * @param name Name of the compile_task/project/command.
     * @return std::vector<std::string>  Descriptions of the
     * compile_task/project/command. If the same name corresponds to more than
     * one description, all are returned.
     */
    static std::vector<std::string> get_descs(std::string name);
};

} // namespace ccc

#endif