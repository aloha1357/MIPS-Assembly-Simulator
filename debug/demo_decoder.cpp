#include "src/Cpu.h"
#include "src/Instruction.h"
#include "src/InstructionDecoder.h"
#include <iomanip>
#include <iostream>

using namespace mips;

int main()
{
    std::cout << "MIPS Instruction Decoder Demo\n";
    std::cout << "=============================\n\n";

    // Test various instruction encodings
    struct TestCase
    {
        uint32_t    word;
        std::string description;
    };

    TestCase tests[] = {{0x01094020, "add $t0, $t0, $t1"}, {0x01095022, "sub $t2, $t0, $t1"},
                        {0x2109000A, "addi $t1, $t0, 10"}, {0x8C080004, "lw $t0, 4($zero)"},
                        {0xAC090008, "sw $t1, 8($zero)"},  {0x08000010, "j 0x40"},
                        {0x0000000C, "syscall"},           {0x2109FFFF, "addi $t1, $t0, -1"}};

    Cpu cpu;

    for (const auto& test : tests)
    {
        std::cout << "Instruction: 0x" << std::hex << std::uppercase << std::setfill('0')
                  << std::setw(8) << test.word;
        std::cout << " (" << test.description << ")\n";

        auto instruction = InstructionDecoder::decode(test.word);
        if (instruction)
        {
            std::cout << "  Decoded as: " << instruction->getName() << "\n";

            // Test execution for some instructions
            if (instruction->getName() == "addi")
            {
                cpu.getRegisterFile().write(8, 5); // Set $t0 = 5
                instruction->execute(cpu);
                std::cout << "  Execution result: $t1 = " << std::dec
                          << cpu.getRegisterFile().read(9) << "\n";
                cpu.reset();
            }
        }
        else
        {
            std::cout << "  Failed to decode!\n";
        }
        std::cout << std::endl;
    }

    std::cout << "Demo completed successfully!\n";
    return 0;
}
