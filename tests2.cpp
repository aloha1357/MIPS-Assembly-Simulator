#include "lib.hpp"
#include "src/MipsSimulatorAPI.h"

#include <gtest/gtest.h>

TEST(LegacyTest, Foo_Is42)
{
    ASSERT_EQ(foo(), 42);
}

class MipsSimulatorTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu     = std::make_unique<mips::CPU>();
        memory  = std::make_unique<mips::Memory>(4096);
        regFile = std::make_unique<mips::RegisterFile>();
        api     = std::make_unique<mips::MipsSimulatorAPI>();
    }

    std::unique_ptr<mips::CPU>              cpu;
    std::unique_ptr<mips::Memory>           memory;
    std::unique_ptr<mips::RegisterFile>     regFile;
    std::unique_ptr<mips::MipsSimulatorAPI> api;
};

TEST_F(MipsSimulatorTest, CPU_Initialization)
{
    EXPECT_TRUE(cpu->isInitialized());
}

TEST_F(MipsSimulatorTest, API_Initialization)
{
    EXPECT_TRUE(api->isInitialized());
}

TEST_F(MipsSimulatorTest, Memory_WordAccess)
{
    uint32_t testValue = 0x12345678;
    memory->storeWord(0, testValue);
    uint32_t readValue = memory->loadWord(0);
    EXPECT_EQ(readValue, testValue);
}

TEST_F(MipsSimulatorTest, Memory_ByteAccess)
{
    uint8_t testByte = 0xAB;
    memory->storeByte(4, testByte);
    uint8_t readByte = memory->loadByte(4);
    EXPECT_EQ(readByte, testByte);
}

TEST_F(MipsSimulatorTest, RegisterFile_ReadWrite)
{
    uint32_t testValue = 0xDEADBEEF;
    regFile->writeRegister(10, testValue);
    uint32_t readValue = regFile->readRegister(10);
    EXPECT_EQ(readValue, testValue);
}

TEST_F(MipsSimulatorTest, RegisterFile_ZeroRegister)
{
    regFile->writeRegister(0, 0xFFFFFFFF);
    uint32_t zeroValue = regFile->readRegister(0);
    EXPECT_EQ(zeroValue, 0);
}

TEST_F(MipsSimulatorTest, Assembler_BasicInstruction)
{
    mips::Assembler       assembler;
    std::string           assembly = "add $t0, $t1, $t2";
    std::vector<uint32_t> instructions = assembler.assemble(assembly);
    EXPECT_GT(instructions.size(), 0);
}

TEST_F(MipsSimulatorTest, API_ProgramExecution)
{
    // Test loading and executing a simple program
    std::string program = R"(
        addi $t0, $zero, 10
        addi $t1, $zero, 20
        add $t2, $t0, $t1
    )";

    EXPECT_TRUE(api->loadProgram(program));

    // Execute the program step by step
    EXPECT_TRUE(api->step());                // addi $t0, $zero, 10
    EXPECT_EQ(api->readRegister(8), 10);     // $t0 = register 8

    EXPECT_TRUE(api->step());                // addi $t1, $zero, 20
    EXPECT_EQ(api->readRegister(9), 20);     // $t1 = register 9

    EXPECT_TRUE(api->step());                // add $t2, $t0, $t1
    EXPECT_EQ(api->readRegister(10), 30);    // $t2 = register 10
}

TEST_F(MipsSimulatorTest, API_MemoryThroughAPI)
{
    // Test memory operations through API
    uint32_t testValue = 0x12345678;
    api->storeWord(100, testValue);
    uint32_t readValue = api->loadWord(100);
    EXPECT_EQ(readValue, testValue);
}

TEST_F(MipsSimulatorTest, API_RegisterThroughAPI)
{
    // Test register operations through API
    uint32_t testValue = 0xABCDEF00;
    api->writeRegister(15, testValue);
    uint32_t readValue = api->readRegister(15);
    EXPECT_EQ(readValue, testValue);
}
