#include "MipsSimulatorAPI.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void printUsage(const char* programName)
{
    std::cout << "MIPS Simulator - Artemis Compatible Version\n";
    std::cout << "Usage: " << programName << " [options] [file.asm]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help          Show this help message\n";
    std::cout << "  -i, --interactive   Run in interactive mode\n";
    std::cout << "  -s, --step          Step through execution\n";
    std::cout << "  -c, --cycles <n>    Run for maximum n cycles\n";
    std::cout << "  -v, --verbose       Verbose output\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " program.asm           # Run assembly file\n";
    std::cout << "  " << programName << " -i                    # Interactive mode\n";
    std::cout << "  " << programName << " -s program.asm        # Step through program\n";
}

bool runInteractiveMode(mips::MipsSimulatorAPI& api)
{
    std::cout << "MIPS Simulator Interactive Mode\n";
    std::cout << "Enter assembly code line by line. Type 'run' to execute, 'quit' to exit.\n\n";

    std::string       line;
    std::stringstream program;

    while (true)
    {
        std::cout << "mips> ";
        if (!std::getline(std::cin, line))
        {
            break;
        }

        if (line == "quit" || line == "exit")
        {
            break;
        }
        else if (line == "run")
        {
            std::string assemblyCode = program.str();
            if (assemblyCode.empty())
            {
                std::cout << "No program to run.\n";
                continue;
            }

            api.reset();
            if (!api.loadProgram(assemblyCode))
            {
                std::cout << "Error: " << api.getLastError() << "\n";
                continue;
            }

            std::cout << "Running program...\n";
            int cycles = api.run(1000); // Max 1000 cycles
            std::cout << "Program completed in " << cycles << " cycles.\n";

            // Show console output if any
            if (!api.getConsoleOutput().empty())
            {
                std::cout << "Program output:\n" << api.getConsoleOutput() << "\n";
            }
        }
        else if (line == "clear")
        {
            program.str("");
            program.clear();
            std::cout << "Program cleared.\n";
        }
        else if (line == "show")
        {
            std::cout << "Current program:\n" << program.str() << "\n";
        }
        else if (!line.empty())
        {
            program << line << "\n";
        }
    }

    return true;
}

bool runFile(mips::MipsSimulatorAPI& api, const std::string& filename, bool stepMode, int maxCycles,
             bool verbose)
{
    if (verbose)
    {
        std::cout << "Loading program from: " << filename << "\n";
    }

    if (!api.loadProgramFromFile(filename))
    {
        std::cerr << "Error loading file: " << api.getLastError() << "\n";
        return false;
    }

    if (verbose)
    {
        std::cout << "Program loaded successfully.\n";
    }

    if (stepMode)
    {
        std::cout << "Step mode - press Enter to execute each instruction, 'q' to quit:\n";
        std::string input;

        while (!api.isTerminated())
        {
            std::cout << "PC: 0x" << std::hex << api.getProgramCounter() << std::dec;
            std::cout << " | Cycle: " << api.getCycleCount() << " > ";

            if (!std::getline(std::cin, input) || input == "q")
            {
                break;
            }

            if (!api.step())
            {
                break;
            }

            if (verbose)
            {
                // Show some register values
                std::cout << "  $t0=" << api.readRegister(8) << " $t1=" << api.readRegister(9)
                          << " $t2=" << api.readRegister(10) << "\n";
            }
        }
    }
    else
    {
        if (verbose)
        {
            std::cout << "Running program...\n";
        }

        int cycles = api.run(maxCycles);

        if (verbose)
        {
            std::cout << "Program completed in " << cycles << " cycles.\n";
        }
    }

    // Show console output if any
    const std::string& output = api.getConsoleOutput();
    if (!output.empty())
    {
        std::cout << "Program output:\n" << output;
        if (output.back() != '\n')
        {
            std::cout << "\n";
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
    mips::MipsSimulatorAPI api;

    if (!api.isInitialized())
    {
        std::cerr << "Error: Failed to initialize MIPS simulator\n";
        return 1;
    }

    // Parse command line arguments
    bool        interactive = false;
    bool        stepMode    = false;
    bool        verbose     = false;
    int         maxCycles   = 10000;
    std::string filename;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (arg == "-i" || arg == "--interactive")
        {
            interactive = true;
        }
        else if (arg == "-s" || arg == "--step")
        {
            stepMode = true;
        }
        else if (arg == "-v" || arg == "--verbose")
        {
            verbose = true;
        }
        else if (arg == "-c" || arg == "--cycles")
        {
            if (i + 1 < argc)
            {
                maxCycles = std::atoi(argv[++i]);
            }
            else
            {
                std::cerr << "Error: --cycles requires a number\n";
                return 1;
            }
        }
        else if (arg[0] != '-')
        {
            filename = arg;
        }
        else
        {
            std::cerr << "Error: Unknown option " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    try
    {
        if (interactive)
        {
            return runInteractiveMode(api) ? 0 : 1;
        }
        else if (!filename.empty())
        {
            return runFile(api, filename, stepMode, maxCycles, verbose) ? 0 : 1;
        }
        else
        {
            std::cerr << "Error: No input file specified and not in interactive mode\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
