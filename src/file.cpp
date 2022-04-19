#include <filesystem>
#include <iostream>

#include "file.h"

std::string absolute_path_to_exec(const std::string& program_name) {
    if (program_name.at(0) == '/') {
        // Absolute path
        return program_name;
    } else if (program_name.find('/') != std::string::npos) {
        // Relative path
        std::string cwd = std::filesystem::current_path();
        return std::filesystem::canonical(cwd + '/' + program_name);
    } else {
        std::cout << "WARNING: Could not determine absolute path to executable, using relative path instead.\n";
        return ".";
    }
}

