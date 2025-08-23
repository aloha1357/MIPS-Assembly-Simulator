#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

/**
 * LLO Instruction Tests - Following Linus's "write tests first" philosophy
 *
 * BDD: Given/When/Then pattern for behavior-driven development
 * TDD: Write failing tests first, then make them pass
 */
class LLOInstructionTest : public ::testing::Test
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
 *      When LLO instruction is executed
 *      Then bottom 16 bits are replaced, top 16 bits preserved
 */
TEST_F(LLOInstructionTest, Given_RegisterWithValue_When_LLOExecuted_Then_LowerBitsReplaced)
{
    // Given: Register $t1 (register 9) with existing value
    cpu->getRegisterFile().write(9, 0x12345678);

    // When: Execute LLO instruction - llo $t1, 0xABCD
    LLOInstruction llo(9, 0xABCD);
    llo.execute(*cpu);

    // Then: Bottom 16 bits replaced (0x5678 -> 0xABCD), top preserved (0x1234)
    EXPECT_EQ(0x1234ABCD, cpu->getRegisterFile().read(9));
}

TEST_F(LLOInstructionTest, Given_RegisterWithMaxValue_When_LLOWithZero_Then_LowerBitsCleared)
{
    // Given: Register with all bits set
    cpu->getRegisterFile().write(8, 0xFFFFFFFF);  // $t0

    // When: Execute llo $t0, 0x0000
    LLOInstruction llo(8, 0x0000);
    llo.execute(*cpu);

    // Then: Lower 16 bits should be zero, upper preserved
    EXPECT_EQ(0xFFFF0000, cpu->getRegisterFile().read(8));
}

TEST_F(LLOInstructionTest, Given_ZeroRegister_When_LLOWithMaxImmediate_Then_LowerBitsSet)
{
    // Given: Register with zero value
    cpu->getRegisterFile().write(10, 0x00000000);  // $t2

    // When: Execute llo $t2, 0xFFFF
    LLOInstruction llo(10, 0xFFFF);
    llo.execute(*cpu);

    // Then: Lower 16 bits should be max, upper remain zero
    EXPECT_EQ(0x0000FFFF, cpu->getRegisterFile().read(10));
}

/**
 * Test instruction decoding - make sure decoder creates correct instruction
 */
TEST_F(LLOInstructionTest, Given_LLOOpcode_When_Decoded_Then_LLOInstructionCreated)
{
    // Given: LLO instruction word - opcode 0x18, rt=9, immediate=0x1234
    // Format: 011000 00000 01001 0001001000110100
    uint32_t word = 0x60091234;  // opcode=0x18, rs=0, rt=9, imm=0x1234

    // When: Decode instruction
    auto instruction = InstructionDecoder::decode(word);

    // Then: Should create LLO instruction (will fail initially)
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "llo");
}

/**
 * Test getName method
 */
TEST_F(LLOInstructionTest, LLOInstruction_ShouldReturnCorrectName)
{
    LLOInstruction llo(1, 0x1234);
    EXPECT_EQ("llo", llo.getName());
}
