#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

/**
 * TRAP Instruction Tests - Following TDD approach
 */
class TrapInstructionTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        memory = std::make_unique<Memory>();
        registerFile = std::make_unique<RegisterFile>();
        cpu = std::make_unique<Cpu>();
    }

    std::unique_ptr<Memory> memory;
    std::unique_ptr<RegisterFile> registerFile;
    std::unique_ptr<Cpu> cpu;
};

/**
 * BDD: Given a trap code
 *      When TRAP instruction is executed
 *      Then trap handler should be invoked with the code
 */
TEST_F(TrapInstructionTest, Given_TrapCode_When_TrapExecuted_Then_HandlerInvoked)
{
    // Given: Clear console output
    cpu->getConsoleOutput(); // This reads and potentially clears
    
    // When: Execute trap instruction with code 42
    TrapInstruction trap(42);
    trap.execute(*cpu);
    
    // Then: Should output trap information
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find("TRAP") != std::string::npos);
    EXPECT_TRUE(output.find("42") != std::string::npos);
}

TEST_F(TrapInstructionTest, Given_ZeroTrapCode_When_TrapExecuted_Then_HandlerInvokedWithZero)
{
    // When: Execute trap instruction with code 0
    TrapInstruction trap(0);
    trap.execute(*cpu);
    
    // Then: Should handle zero trap code
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find("TRAP") != std::string::npos);
    EXPECT_TRUE(output.find("0") != std::string::npos);
}

TEST_F(TrapInstructionTest, Given_MaxTrapCode_When_TrapExecuted_Then_HandlerInvokedWithMaxCode)
{
    // Given: Max 26-bit value (0x3FFFFFF)
    uint32_t maxTrapCode = 0x3FFFFFF;
    
    // When: Execute trap instruction with max code
    TrapInstruction trap(maxTrapCode);
    trap.execute(*cpu);
    
    // Then: Should handle max trap code
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find("TRAP") != std::string::npos);
}

/**
 * Test instruction decoding
 */
TEST_F(TrapInstructionTest, Given_TrapOpcode_When_Decoded_Then_TrapInstructionCreated)
{
    // Given: TRAP instruction word - opcode 0x1A, trapcode=42
    uint32_t word = 0x6800002A; // opcode=0x1A, trapcode=42
    
    // When: Decode instruction
    auto instruction = InstructionDecoder::decode(word);
    
    // Then: Should create TRAP instruction
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "trap");
}

/**
 * Test getName method
 */
TEST_F(TrapInstructionTest, TrapInstruction_ShouldReturnCorrectName)
{
    TrapInstruction trap(123);
    EXPECT_EQ("trap", trap.getName());
}
