#include "Assembler.h"
#include "Cpu.h"
#include "Instruction.h"
#include "RegisterFile.h"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: jal_debug_runner <asm-file> [steps]\n";
        return 1;
    }

    std::string path     = argv[1];
    int         maxSteps = 1000;
    if (argc >= 3)
        maxSteps = std::stoi(argv[2]);

    std::ifstream in(path);
    if (!in)
    {
        std::cerr << "Failed to open " << path << "\n";
        return 1;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    std::string assembly = ss.str();

    mips::Assembler                  assembler;
    std::map<std::string, uint32_t>  labelMap;
    std::vector<mips::DataDirective> dataDirectives;
    auto instructions = assembler.assembleWithLabels(assembly, labelMap, dataDirectives);

    std::cout << "Assembled instructions=" << instructions.size()
              << " dataDirectives=" << dataDirectives.size() << " labels=" << labelMap.size()
              << "\n";
    for (const auto& p : labelMap)
    {
        std::cout << "  label: '" << p.first << "' -> " << p.second << "\n";
    }

    mips::Cpu cpu;
    // Use direct instructions by moving them into cpu internals via loadProgramFromString for now
    cpu.loadProgramFromString(assembly);

    std::cout << "Starting jal_debug_runner for: " << path << " maxSteps=" << maxSteps << "\n";

    for (int step = 0; step < maxSteps; ++step)
    {
        uint32_t pc = cpu.getProgramCounter();
        uint32_t ra = cpu.getRegisterFile().read(31);
        std::cout << "step=" << step << " pc=" << pc << " $ra=" << ra << "\n";

        // If program terminated, break
        if (cpu.shouldTerminate())
        {
            std::cout << "Program terminated at step=" << step << "\n";
            break;
        }

        // If PC is out of range, stop
        if (pc >= 1000000)  // safety
        {
            std::cout << "PC out of range: " << pc << "\n";
            break;
        }

        cpu.tick();
    }

    std::cout << "Final PC=" << cpu.getProgramCounter() << " $ra=" << cpu.getRegisterFile().read(31)
              << "\n";
    std::cout << "Console output: " << cpu.getConsoleOutput() << "\n";
    return 0;
}
