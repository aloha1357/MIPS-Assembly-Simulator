#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

/**
 * LHI Instruction Tests - Following TDD approach
 *
 * BDD: Given/When/Then pattern for behavior-driven development
 */
class LHIInstructionTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        memory       = std::make_unique<Memory>();
        registerFile = std::make_unique<RegisterFile>();
        cpu          = std::make_unique<Cpu>();
    }

    std::unique_ptr<Memory>       memory;
    std::unique_ptr<RegisterFile> registerFile;
    std::unique_ptr<Cpu>          cpu;
};

/**
 * BDD: Given a register with existing value
 *      When LHI instruction is executed
 *      Then upper 16 bits are replaced, lower 16 bits preserved
 */
TEST_F(LHIInstructionTest, Given_RegisterWithValue_When_LHIExecuted_Then_UpperBitsReplaced)
{
    // Given: Register $t2 (register 10) with existing value
    cpu->getRegisterFile().write(10, 0x12345678);

    // When: Execute LHI instruction - lhi $t2, 0xABCD
    LHIInstruction lhi(10, 0xABCD);
    lhi.execute(*cpu);

    // Then: Upper 16 bits replaced (0x1234 -> 0xABCD), lower preserved (0x5678)
    EXPECT_EQ(0xABCD5678, cpu->getRegisterFile().read(10));
}

TEST_F(LHIInstructionTest, Given_RegisterWithMaxValue_When_LHIWithZero_Then_UpperBitsCleared)
{
    // Given: Register with all bits set
    cpu->getRegisterFile().write(11, 0xFFFFFFFF);  // $t3

    // When: Execute lhi $t3, 0x0000
    LHIInstruction lhi(11, 0x0000);
    lhi.execute(*cpu);

    // Then: Upper 16 bits should be zero, lower preserved
    EXPECT_EQ(0x0000FFFF, cpu->getRegisterFile().read(11));
}

TEST_F(LHIInstructionTest, Given_ZeroRegister_When_LHIWithMaxImmediate_Then_UpperBitsSet)
{
    // Given: Register with zero value
    cpu->getRegisterFile().write(12, 0x00000000);  // $t4

    // When: Execute lhi $t4, 0xFFFF
    LHIInstruction lhi(12, 0xFFFF);
    lhi.execute(*cpu);

    // Then: Upper 16 bits should be max, lower remain zero
    EXPECT_EQ(0xFFFF0000, cpu->getRegisterFile().read(12));
}

/**
 * BDD: Test combination with LLO instruction
 */
TEST_F(LHIInstructionTest, Given_Register_When_LHIThenLLO_Then_CombinationWorks)
{
    // Given: Register starts at zero
    cpu->getRegisterFile().write(9, 0x00000000);  // $t1

    // When: Execute lhi $t1, 0x1234 then llo $t1, 0x5678
    LHIInstruction lhi(9, 0x1234);
    lhi.execute(*cpu);
    LLOInstruction llo(9, 0x5678);
    llo.execute(*cpu);

    // Then: Register should contain combined value
    EXPECT_EQ(0x12345678, cpu->getRegisterFile().read(9));
}

/**
 * Test instruction decoding
 */
TEST_F(LHIInstructionTest, Given_LHIOpcode_When_Decoded_Then_LHIInstructionCreated)
{
    // Given: LHI instruction word - opcode 0x19, rt=10, immediate=0x5678
    uint32_t word = 0x640A5678;  // opcode=0x19, rs=0, rt=10, imm=0x5678

    // When: Decode instruction
    auto instruction = InstructionDecoder::decode(word);

    // Then: Should create LHI instruction
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "lhi");
}

/**
 * Test getName method
 */
TEST_F(LHIInstructionTest, LHIInstruction_ShouldReturnCorrectName)
{
    LHIInstruction lhi(1, 0x1234);
    EXPECT_EQ("lhi", lhi.getName());
}
