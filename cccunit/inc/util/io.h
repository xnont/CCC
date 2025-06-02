#ifndef __CCC_IO_H__
#define __CCC_IO_H__

#include <string>

namespace ccc {

namespace io {

/**
 * @brief Print message to console.
 *
 * @param message The message to print.
 */
void print(const std::string& message);

/**
 * @brief Print message to console.
 *
 * @param message The message to print.
 */
void print(const char* message);

/**
 * @brief Print message to console and add a new line.
 *
 * @param message The message to print.
 */
void println(const std::string& message);

/**
 * @brief Print message to console and add a new line.
 *
 * @param message The message to print.
 */
void println(const char* message);

/**
 * @brief Execute command and determine whether to output command string and
 *        execution result strings on the console based on parameters.
 *
 * @param cmd The command to execute.
 * @param is_cmd_print Whether to print the command.
 * @param is_res_print Whether to print the result.
 * @return true If the command executed successfully.
 * @return false If the command executed failed.
 */
bool exec_command(const std::string& cmd, bool is_cmd_print, bool is_res_print);

/**
 * @brief Get the terminal width.
 *
 * @return int The terminal width.
 */
int get_terminal_width();

} // namespace io

} // namespace ccc

#endif