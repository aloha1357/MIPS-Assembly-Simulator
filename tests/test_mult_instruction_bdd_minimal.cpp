/**
 * @file test_mult_instruction_bdd_minimal.cpp
 * @brief BDD tests for MULT instruction (Phase 8.2: Red Light)
 */

#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>

using namespace mips;

class MULTInstructionBDD : public ::testing::Test
{
  protected:
    std::unique_ptr<Cpu>                cpu;
    std::unique_ptr<Assembler>          assembler;
    std::unique_ptr<InstructionDecoder> decoder;

    void SetUp() override
    {
        cpu       = std::make_unique<Cpu>();
        assembler = std::make_unique<Assembler>();
        decoder   = std::make_unique<InstructionDecoder>();

        // Clear all registers
        for (int i = 0; i < 32; ++i)
        {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
        cpu->getMemory().reset();
    }

    void TearDown() override
    {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }
};

/**
 * @brief BDD Scenario: Basic signed multiplication test
 *
 * Given: Two positive numbers in registers
 * When: MULT instruction is executed
 * Then: HI:LO should contain the 64-bit signed multiplication result
 */
TEST_F(MULTInstructionBDD, BasicSignedMultiplication)
{
    // Given: Two positive numbers
    cpu->getRegisterFile().write(8, 123); // $t0 = 123
    cpu->getRegisterFile().write(9, 456); // $t1 = 456

    // When: MULT $t0, $t1
    uint32_t mult_instruction = 0x01094818; // MULT opcode, rs=$t0(8), rt=$t1(9), function=0x18
    auto     instruction      = decoder->decode(mult_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should contain 123 * 456 = 56088
    uint64_t expected_result = 123ULL * 456ULL;
    uint32_t expected_hi     = static_cast<uint32_t>(expected_result >> 32);
    uint32_t expected_lo     = static_cast<uint32_t>(expected_result & 0xFFFFFFFF);

    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_hi);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_lo);
}

/**
 * @brief BDD Scenario: 64-bit result test (HI:LO verification)
 *
 * Given: Two large numbers that result in a 64-bit product
 * When: MULT instruction is executed
 * Then: HI register should contain upper 32 bits, LO register lower 32 bits
 */
TEST_F(MULTInstructionBDD, SixtyFourBitResult)
{
    // Given: Two large numbers
    cpu->getRegisterFile().write(8, 0x7FFFFFFF); // $t0 = max positive int32
    cpu->getRegisterFile().write(9, 2);          // $t1 = 2

    // When: MULT $t0, $t1
    uint32_t mult_instruction = 0x01094818; // MULT opcode, rs=$t0(8), rt=$t1(9), function=0x18
    auto     instruction      = decoder->decode(mult_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should contain 0x7FFFFFFF * 2 = 0xFFFFFFFE
    uint64_t expected_result = 0x7FFFFFFFULL * 2ULL;
    uint32_t expected_hi     = static_cast<uint32_t>(expected_result >> 32);
    uint32_t expected_lo     = static_cast<uint32_t>(expected_result & 0xFFFFFFFF);

    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_hi);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_lo);
}

/**
 * @brief BDD Scenario: Negative number multiplication test
 *
 * Given: One positive and one negative number
 * When: MULT instruction is executed
 * Then: Result should be correctly sign-extended negative value
 */
TEST_F(MULTInstructionBDD, NegativeMultiplication)
{
    // Given: One positive, one negative number
    cpu->getRegisterFile().write(8, 100);        // $t0 = 100
    cpu->getRegisterFile().write(9, 0xFFFFFFFF); // $t1 = -1 (signed)

    // When: MULT $t0, $t1
    uint32_t mult_instruction = 0x01094818; // MULT opcode, rs=$t0(8), rt=$t1(9), function=0x18
    auto     instruction      = decoder->decode(mult_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should contain 100 * (-1) = -100
    int64_t  expected_result = 100LL * (-1LL);
    uint32_t expected_hi     = static_cast<uint32_t>((expected_result >> 32) & 0xFFFFFFFF);
    uint32_t expected_lo     = static_cast<uint32_t>(expected_result & 0xFFFFFFFF);

    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_hi);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_lo);
}

/**
 * @brief BDD Scenario: Zero multiplication test
 *
 * Given: One operand is zero
 * When: MULT instruction is executed
 * Then: Result should be zero in both HI and LO registers
 */
TEST_F(MULTInstructionBDD, ZeroMultiplication)
{
    // Given: One operand is zero
    cpu->getRegisterFile().write(8, 0);     // $t0 = 0
    cpu->getRegisterFile().write(9, 12345); // $t1 = 12345

    // When: MULT $t0, $t1
    uint32_t mult_instruction = 0x01094818; // MULT opcode, rs=$t0(8), rt=$t1(9), function=0x18
    auto     instruction      = decoder->decode(mult_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should both be zero
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0);
}
