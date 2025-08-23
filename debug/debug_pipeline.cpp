#include "src/Cpu.h"
#include <iostream>

using namespace mips;

int main()
{
    std::cout << "Pipeline Debug Test\n";

    Cpu         cpu;
    std::string program = R"(
        addi $t0, $zero, 10
        addi $t1, $zero, 20
        add $t2, $t0, $t1
        syscall
    )";

    cpu.loadProgramFromString(program);
    cpu.setPipelineMode(true);

    std::cout << "Starting pipeline execution...\n";

    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Cycle " << i << ": ";
        std::cout << "$t0=" << cpu.getRegisterFile().read(8)
                  << " $t1=" << cpu.getRegisterFile().read(9)
                  << " $t2=" << cpu.getRegisterFile().read(10) << " PC=" << cpu.getProgramCounter()
                  << " Terminated=" << (cpu.shouldTerminate() ? "Yes" : "No") << std::endl;

        if (cpu.shouldTerminate())
        {
            std::cout << "Program terminated\n";
            break;
        }

        cpu.tick();
    }

    std::cout << "Final state: ";
    std::cout << "$t0=" << cpu.getRegisterFile().read(8) << " $t1=" << cpu.getRegisterFile().read(9)
              << " $t2=" << cpu.getRegisterFile().read(10) << std::endl;

    return 0;
}
