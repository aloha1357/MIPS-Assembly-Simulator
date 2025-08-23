/**
 * @file test_divu_instruction_bdd_minimal.cpp
 * @brief BDD tests for DIVU instruction (Phase 8.6: Red Light)
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

class DIVUInstructionBDD : public ::testing::Test
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
 * @brief BDD Scenario: Basic unsigned division test
 *
 * Given: Two positive numbers in registers
 * When: DIVU instruction is executed
 * Then: LO should contain quotient, HI should contain remainder (unsigned)
 */
TEST_F(DIVUInstructionBDD, BasicUnsignedDivision)
{
    // Given: Two positive numbers
    cpu->getRegisterFile().write(8, 100); // $t0 = 100
    cpu->getRegisterFile().write(9, 7);   // $t1 = 7

    // When: DIVU $t0, $t1
    uint32_t divu_instruction = 0x0109481B; // DIVU opcode, rs=$t0(8), rt=$t1(9), function=0x1B
    auto     instruction      = decoder->decode(divu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: LO should contain quotient (100 / 7 = 14), HI should contain remainder (100 % 7 = 2)
    uint32_t expected_quotient  = 100U / 7U; // 14
    uint32_t expected_remainder = 100U % 7U; // 2

    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_quotient);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_remainder);
}

/**
 * @brief BDD Scenario: Divide by zero handling test
 *
 * Given: Dividend is non-zero, divisor is zero
 * When: DIVU instruction is executed
 * Then: Both HI and LO should be set to zero (undefined behavior, but safe)
 */
TEST_F(DIVUInstructionBDD, DivideByZeroHandling)
{
    // Given: Non-zero dividend, zero divisor
    cpu->getRegisterFile().write(8, 100); // $t0 = 100
    cpu->getRegisterFile().write(9, 0);   // $t1 = 0

    // When: DIVU $t0, $t1
    uint32_t divu_instruction = 0x0109481B; // DIVU opcode, rs=$t0(8), rt=$t1(9), function=0x1B
    auto     instruction      = decoder->decode(divu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: Both HI and LO should be zero (safe handling of divide by zero)
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0);
}

/**
 * @brief BDD Scenario: Large unsigned division test
 *
 * Given: Large unsigned numbers (that would be negative if treated as signed)
 * When: DIVU instruction is executed
 * Then: Result should be correctly calculated as unsigned values
 */
TEST_F(DIVUInstructionBDD, LargeUnsignedDivision)
{
    // Given: Large unsigned numbers
    cpu->getRegisterFile().write(8, 0xFFFFFFFF); // $t0 = 4294967295 (max uint32)
    cpu->getRegisterFile().write(9, 2);          // $t1 = 2

    // When: DIVU $t0, $t1
    uint32_t divu_instruction = 0x0109481B; // DIVU opcode, rs=$t0(8), rt=$t1(9), function=0x1B
    auto     instruction      = decoder->decode(divu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: LO should contain quotient, HI should contain remainder (unsigned arithmetic)
    uint32_t expected_quotient  = 0xFFFFFFFFU / 2U; // 2147483647
    uint32_t expected_remainder = 0xFFFFFFFFU % 2U; // 1

    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_quotient);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_remainder);
}

/**
 * @brief BDD Scenario: Unsigned remainder verification test
 *
 * Given: Numbers that don't divide evenly
 * When: DIVU instruction is executed
 * Then: Quotient and remainder should satisfy dividend = quotient * divisor + remainder
 */
TEST_F(DIVUInstructionBDD, UnsignedRemainderVerification)
{
    // Given: Numbers that don't divide evenly
    cpu->getRegisterFile().write(8, 0x80000000); // $t0 = 2147483648 (would be negative in signed)
    cpu->getRegisterFile().write(9, 10);         // $t1 = 10

    // When: DIVU $t0, $t1
    uint32_t divu_instruction = 0x0109481B; // DIVU opcode, rs=$t0(8), rt=$t1(9), function=0x1B
    auto     instruction      = decoder->decode(divu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: Verify the division identity: dividend = quotient * divisor + remainder
    uint32_t quotient  = cpu->getRegisterFile().readLO();
    uint32_t remainder = cpu->getRegisterFile().readHI();

    // For unsigned division: 2147483648 / 10 = 214748364, 2147483648 % 10 = 8
    uint32_t expected_quotient  = 0x80000000U / 10U; // 214748364
    uint32_t expected_remainder = 0x80000000U % 10U; // 8

    EXPECT_EQ(quotient, expected_quotient);
    EXPECT_EQ(remainder, expected_remainder);

    // Verify: dividend = quotient * divisor + remainder
    EXPECT_EQ(0x80000000U, quotient * 10U + remainder);
}
