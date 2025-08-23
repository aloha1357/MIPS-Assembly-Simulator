#pragma once

#include "cli.hpp"

namespace cli {

/**
 * @brief Execute the assemble command with the given configuration
 * @param config AssembleConfig containing input file, output file, and options
 * @return Exit code (EXIT_OK, EXIT_IO_ERROR, or EXIT_RUNTIME_ERROR)
 */
int execute_assemble_command(const AssembleConfig& config);

}  // namespace cli
