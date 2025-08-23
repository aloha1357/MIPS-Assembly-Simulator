#pragma once

#include "cli.hpp"
#include "../src/MipsSimulatorAPI.h"
#include <string>

namespace cli {

/**
 * @brief Execute a MIPS program using the run command
 * @param config Run configuration
 * @return Exit code
 */
int execute_run_command(const RunConfig& config);

/**
 * @brief Load file content into string
 * @param filename Path to file
 * @param content Output content
 * @return true if successful
 */
bool load_file_content(const std::string& filename, std::string& content);

}  // namespace cli
