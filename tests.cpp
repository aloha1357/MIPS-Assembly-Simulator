#include "lib.hpp"
#include "src/MipsSimulatorAPI.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Legacy library test")
{
    CHECK(foo() == 42);
}

TEST_CASE("MIPS Simulator - CPU Initialization")
{
    mips::CPU cpu;
    REQUIRE(cpu.isInitialized() == true);
}

TEST_CASE("MIPS Simulator - Memory Operations")
{
    mips::Memory memory(4096);  // 4KB memory

    SECTION("Word-aligned memory access")
    {
        uint32_t testValue = 0x12345678;
        memory.storeWord(0, testValue);
        uint32_t readValue = memory.loadWord(0);
        REQUIRE(readValue == testValue);
    }

    SECTION("Byte memory access")
    {
        uint8_t testByte = 0xAB;
        memory.storeByte(0, testByte);
        uint8_t readByte = memory.loadByte(0);
        REQUIRE(readByte == testByte);
    }
}

TEST_CASE("MIPS Simulator - Register File")
{
    mips::RegisterFile regFile;

    SECTION("Register read/write")
    {
        uint32_t testValue = 0xDEADBEEF;
        regFile.writeRegister(5, testValue);
        uint32_t readValue = regFile.readRegister(5);
        REQUIRE(readValue == testValue);
    }

    SECTION("Register $zero always returns 0")
    {
        regFile.writeRegister(0, 0xFFFFFFFF);
        uint32_t zeroValue = regFile.readRegister(0);
        REQUIRE(zeroValue == 0);
    }
}

TEST_CASE("MIPS Simulator - Basic Assembly")
{
    mips::Assembler assembler;

    SECTION("Simple ADD instruction")
    {
        std::string           assembly     = "add $t0, $t1, $t2";
        std::vector<uint32_t> instructions = assembler.assemble(assembly);
        REQUIRE(instructions.size() > 0);
    }
}

TEST_CASE("MIPS Simulator - API Integration")
{
    mips::MipsSimulatorAPI api;

    SECTION("API Initialization")
    {
        REQUIRE(api.isInitialized() == true);
    }

    SECTION("Simple program execution")
    {
        std::string program = "addi $t0, $zero, 42";
        REQUIRE(api.loadProgram(program) == true);

        // Execute program
        api.step();

        // Check result (register $t0 should be 42)
        // Note: $t0 is register 8 in MIPS
        uint32_t result = api.readRegister(8);
        REQUIRE(result == 42);
    }
}
