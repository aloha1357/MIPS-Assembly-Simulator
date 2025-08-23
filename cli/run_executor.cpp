#include "run_executor.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>

namespace cli {

bool load_file_content(const std::string& filename, std::string& content) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    content.clear();
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    return true;
}

int execute_run_command(const RunConfig& config) {
    // Check if file exists
    if (!std::filesystem::exists(config.program)) {
        std::cerr << "mipsim: file not found: " << config.program << std::endl;
        return EXIT_IO_ERROR;
    }

    // Load the program file
    std::string program_content;
    if (!load_file_content(config.program, program_content)) {
        std::cerr << "mipsim: failed to read file: " << config.program << std::endl;
        return EXIT_IO_ERROR;
    }

    // Create simulator instance
    mips::MipsSimulatorAPI simulator;
    
    // Load the program
    if (!simulator.loadProgram(program_content)) {
        std::cerr << "mipsim: assembly error: " << simulator.getLastError() << std::endl;
        return EXIT_RUNTIME_ERROR;
    }

    // Debug: Print program loaded successfully
    // std::cerr << "DEBUG: Program loaded successfully" << std::endl;

    // Execute the program
    try {
        int cycles_executed;
        auto start_time = std::chrono::steady_clock::now();
        
        if (config.limit > 0 && config.timeout > 0) {
            // Both limits specified - check both
            cycles_executed = 0;
            while (!simulator.isTerminated()) {
                simulator.step();
                cycles_executed++;
                
                // Check cycle limit
                if (cycles_executed >= config.limit) {
                    std::cerr << "mipsim: step limit exceeded (limit: " << config.limit << ")" << std::endl;
                    return EXIT_RUNTIME_ERROR;
                }
                
                // Check timeout
                auto current_time = std::chrono::steady_clock::now();
                auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
                if (elapsed_seconds >= config.timeout) {
                    std::cerr << "mipsim: timeout exceeded (timeout: " << config.timeout << " seconds)" << std::endl;
                    return EXIT_RUNTIME_ERROR;
                }
            }
        } else if (config.limit > 0) {
            // Only cycle limit
            cycles_executed = simulator.run(static_cast<int>(config.limit));
            
            // Check if we hit the limit
            if (!simulator.isTerminated() && cycles_executed >= config.limit) {
                std::cerr << "mipsim: step limit exceeded (limit: " << config.limit << ")" << std::endl;
                return EXIT_RUNTIME_ERROR;
            }
        } else if (config.timeout > 0) {
            // Only timeout
            cycles_executed = 0;
            while (!simulator.isTerminated()) {
                simulator.step();
                cycles_executed++;
                
                // Check timeout every 100 cycles for efficiency
                if (cycles_executed % 100 == 0) {
                    auto current_time = std::chrono::steady_clock::now();
                    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
                    if (elapsed_seconds >= config.timeout) {
                        std::cerr << "mipsim: timeout exceeded (timeout: " << config.timeout << " seconds)" << std::endl;
                        return EXIT_RUNTIME_ERROR;
                    }
                }
            }
        } else {
            // No limits
            cycles_executed = simulator.run(0);  // No limit
        }

        // Print any console output from the program
        const std::string& console_output = simulator.getConsoleOutput();
        if (!console_output.empty()) {
            std::cout << console_output;
        }

        // Debug output
        // std::cerr << "DEBUG: Executed " << cycles_executed << " cycles" << std::endl;
        // std::cerr << "DEBUG: Console output length: " << console_output.length() << std::endl;

        // Verbose output if requested
        if (cycles_executed > 0) {
            // Optional: print execution summary in verbose mode
            // std::cerr << "Executed " << cycles_executed << " cycles" << std::endl;
        }

        return EXIT_OK;
        
    } catch (const std::exception& e) {
        std::cerr << "mipsim: runtime error: " << e.what() << std::endl;
        return EXIT_RUNTIME_ERROR;
    }
}

}  // namespace cli
