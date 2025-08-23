#include "src/MipsSimulatorAPI.h"
#include <iostream>
#include <string>

int main()
{
    std::cout << "Testing MIPS Simulator API..." << std::endl;

    try
    {
        // Test 1: API Initialization
        mips::MipsSimulatorAPI api;
        if (!api.isInitialized())
        {
            std::cerr << "Error: API not initialized" << std::endl;
            return 1;
        }
        std::cout << "✓ API initialization successful" << std::endl;

        // Test 2: Legacy compatibility classes
        mips::CPU cpu;
        if (!cpu.isInitialized())
        {
            std::cerr << "Error: CPU not initialized" << std::endl;
            return 1;
        }
        std::cout << "✓ Legacy CPU class working" << std::endl;

        // Test 3: Memory operations
        mips::Memory memory(4096);
        memory.storeWord(0, 0x12345678);
        uint32_t value = memory.loadWord(0);
        if (value != 0x12345678)
        {
            std::cerr << "Error: Memory test failed, got " << std::hex << value << std::endl;
            return 1;
        }
        std::cout << "✓ Memory operations working" << std::endl;

        // Test 4: Register operations
        mips::RegisterFile regFile;
        regFile.writeRegister(5, 0xDEADBEEF);
        uint32_t regValue = regFile.readRegister(5);
        if (regValue != 0xDEADBEEF)
        {
            std::cerr << "Error: Register test failed, got " << std::hex << regValue << std::endl;
            return 1;
        }
        std::cout << "✓ Register operations working" << std::endl;

        // Test 5: Basic assembler test
        mips::Assembler assembler;
        auto            instructions = assembler.assemble("add $t0, $t1, $t2");
        if (instructions.empty())
        {
            std::cerr << "Error: Assembler test failed" << std::endl;
            return 1;
        }
        std::cout << "✓ Assembler basic test working" << std::endl;

        std::cout << "\nAll basic tests passed! ✓" << std::endl;
        std::cout << "The API interface is working correctly." << std::endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception during testing: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception during testing" << std::endl;
        return 1;
    }
}
