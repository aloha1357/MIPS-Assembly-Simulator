/**
 * @file test_srav_instruction_bdd_minimal.cpp
 * @brief BDD Tests for SRAV (Shift Right Arithmetic Variable) instruction implementation
 *
 * Phase 5.3: SRAV instruction BDD tests (Red-Light Phase)
 *
 * This file implements Behavior-Driven Development (BDD) tests for the SRAV instruction.
 * SRAV performs variable arithmetic right shift where shift amount comes from a register.
 *
 * Key characteristics of SRAV:
 * - Function Code: 0x07 (R-type instruction)
 * - Format: srav $rd, $rt, $rs
 * - Function: rd = rt >>> (rs & 0x1F)
 * - Note: Only low 5 bits of rs register are used for shift amount (0-31)
 * - Arithmetic shift: sign extension from left (preserve sign bit)
 */

#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/RegisterFile.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>

/**
 * BDD Test Fixture for SRAV instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class SRAVInstructionBDDTest : public ::testing::Test
{
  protected:
    std::unique_ptr<mips::Cpu>                cpu;
    std::unique_ptr<mips::Assembler>          assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;

    void SetUp() override
    {
        cpu       = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder   = std::make_unique<mips::InstructionDecoder>();

        // Clear all registers
        for (int i = 0; i < 32; ++i)
        {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }

    void TearDown() override
    {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }
};

/**
 * Scenario 1: Basic Variable Arithmetic Right Shift (Negative Number)
 * Given: rt register contains 0x80000000 (negative), rs register contains 4
 * When: SRAV instruction shifts rt right arithmetically by rs amount
 * Then: rd register should contain 0xF8000000 (sign extended)
 */
TEST_F(SRAVInstructionBDDTest, BasicVariableArithmeticRightShiftNegative)
{
    // Given: rt = 0x80000000 (negative number), rs = 4
    const uint32_t rt_value    = 0x80000000;
    const uint32_t rs_value    = 4;
    const uint32_t rt_register = 1; // $t0 (source value)
    const uint32_t rs_register = 2; // $t1 (shift amount)
    const uint32_t rd_register = 3; // $t2 (destination)

    // For arithmetic right shift: sign-extend from left
    // 0x80000000 >> 4 = 0xF8000000 (arithmetic)
    const uint32_t expected_result = 0xF8000000;

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRAV instruction
    // Format: SRAV rd, rt, rs
    // Note: This will fail initially as SRAVInstruction class doesn't exist
    auto srav_instruction =
        std::make_unique<mips::SRAVInstruction>(rd_register, rt_register, rs_register);
    srav_instruction->execute(*cpu);

    // Then: rd register should contain the sign-extended result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 2: Arithmetic Right Shift with Positive Number
 * Given: rt register contains 0x70000000 (positive), rs register contains 4
 * When: SRAV instruction shifts rt right arithmetically by rs amount
 * Then: rd register should contain 0x07000000 (zero extended, same as logical)
 */
TEST_F(SRAVInstructionBDDTest, ArithmeticRightShiftPositive)
{
    // Given: rt = 0x70000000 (positive number), rs = 4
    const uint32_t rt_value    = 0x70000000;
    const uint32_t rs_value    = 4;
    const uint32_t rt_register = 4; // $t3
    const uint32_t rs_register = 5; // $t4
    const uint32_t rd_register = 6; // $t5

    // For positive numbers, arithmetic and logical shifts are the same
    const uint32_t expected_result = 0x07000000;

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRAV instruction
    auto srav_instruction =
        std::make_unique<mips::SRAVInstruction>(rd_register, rt_register, rs_register);
    srav_instruction->execute(*cpu);

    // Then: rd register should contain the shifted result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 3: Shift Amount Truncation with Sign Extension
 * Given: rt register contains 0xFFFFFFFF (all 1s), rs register contains 35 (0x23)
 * When: SRAV instruction uses only low 5 bits of rs (35 & 0x1F = 3)
 * Then: rd register should contain 0xFFFFFFFF (sign extended, all 1s remain)
 */
TEST_F(SRAVInstructionBDDTest, ShiftAmountTruncationSignExtended)
{
    // Given: rt = 0xFFFFFFFF (negative -1), rs = 35 (0x23)
    const uint32_t rt_value        = 0xFFFFFFFF;
    const uint32_t rs_value        = 35;              // 0x23, but only low 5 bits (3) will be used
    const uint32_t rt_register     = 7;               // $t6
    const uint32_t rs_register     = 8;               // $t7
    const uint32_t rd_register     = 9;               // $t8
    const uint32_t effective_shift = rs_value & 0x1F; // 35 & 0x1F = 3
    (void)effective_shift;                            // Suppress unused variable warning

    // Arithmetic right shift of -1 by any amount is still -1
    const uint32_t expected_result = 0xFFFFFFFF;

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRAV instruction
    auto srav_instruction =
        std::make_unique<mips::SRAVInstruction>(rd_register, rt_register, rs_register);
    srav_instruction->execute(*cpu);

    // Then: rd register should contain sign-extended result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 4: Maximum Shift Amount with Sign Extension
 * Given: rt register contains 0x80000001 (negative), rs register contains 31
 * When: SRAV instruction shifts by maximum amount (31) with sign extension
 * Then: rd register should contain 0xFFFFFFFF (sign bit propagated throughout)
 */
TEST_F(SRAVInstructionBDDTest, MaximumShiftSignExtension)
{
    // Given: rt = 0x80000001 (negative number), rs = 31
    const uint32_t rt_value    = 0x80000001;
    const uint32_t rs_value    = 31; // Maximum shift amount
    const uint32_t rt_register = 10; // $t9
    const uint32_t rs_register = 11; // $s0
    const uint32_t rd_register = 12; // $s1

    // Arithmetic right shift by 31 of negative number results in all 1s (0xFFFFFFFF)
    const uint32_t expected_result = 0xFFFFFFFF;

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRAV instruction with maximum shift
    auto srav_instruction =
        std::make_unique<mips::SRAVInstruction>(rd_register, rt_register, rs_register);
    srav_instruction->execute(*cpu);

    // Then: rd register should contain the sign-extended result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}
