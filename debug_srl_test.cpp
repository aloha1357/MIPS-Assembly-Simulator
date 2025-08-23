#include "src/InstructionDecoder.h"
#include <iomanip>
#include <iostream>

int main()
{
    mips::InstructionDecoder decoder;

    uint32_t instruction = 0x00084122; // srl $t1, $t0, 4

    std::cout << "Testing instruction: 0x" << std::hex << instruction << std::endl;

    // Extract fields manually
    uint32_t opcode = (instruction >> 26) & 0x3F;
    uint32_t rs     = (instruction >> 21) & 0x1F;
    uint32_t rt     = (instruction >> 16) & 0x1F;
    uint32_t rd     = (instruction >> 11) & 0x1F;
    uint32_t shamt  = (instruction >> 6) & 0x1F;
    uint32_t func   = instruction & 0x3F;

    std::cout << "Opcode: 0x" << std::hex << opcode << std::endl;
    std::cout << "Rs: " << std::dec << rs << std::endl;
    std::cout << "Rt: " << rt << std::endl;
    std::cout << "Rd: " << rd << std::endl;
    std::cout << "Shamt: " << shamt << std::endl;
    std::cout << "Function: 0x" << std::hex << func << std::endl;

    auto decoded = decoder.decode(instruction);
    if (decoded)
    {
        std::cout << "Decoded as: " << decoded->getName() << std::endl;
    }
    else
    {
        std::cout << "Failed to decode!" << std::endl;
    }

    return 0;
}
