#include <string>

/**
 * Try to find the absoute path to the executable.
 *
 * Only works on Unix systems
 */
std::string absolute_path_to_exec(const std::string& program_name);

/**
 * Try to find the absoute path to the executable.
 *
 *  If successful, success is set to true. Only works on Unix systems.
 */
std::string absolute_path_to_exec(const std::string& program_name, bool& success);
