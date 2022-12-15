#include <string>

/**
 * Try to find the absoute path to the executable.
 *
 * Only works on Unix systems.
 *
 * @param program_name the name by which the program was called. Should be argv[0]
 *
 * @return path to the executable as a string
 */
std::string absolute_path_to_exec(const std::string& program_name);

