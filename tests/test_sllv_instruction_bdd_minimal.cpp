/**
 * @file test_sllv_instruction_bdd_minimal.cpp
 * @brief BDD Tests for SLLV (Shift Left Logical Variable) instruction implementation
 *
 * Phase 5.1: SLLV instruction BDD tests (Red-Light Phase)
 *
 * This file implements Behavior-Driven Development (BDD) tests for the SLLV instruction.
 * SLLV performs variable left shift where shift amount comes from a register.
 *
 * Key characteristics of SLLV:
 * - Function Code: 0x04 (R-type instruction)
 * - Format: sllv $rd, $rt, $rs
 * - Function: rd = rt << (rs & 0x1F)
 * - Note: Only low 5 bits of rs register are used for shift amount (0-31)
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
 * BDD Test Fixture for SLLV instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class SLLVInstructionBDDTest : public ::testing::Test
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
 * Scenario 1: Basic Variable Left Shift
 * Given: rt register contains 0x00000001, rs register contains 4
 * When: SLLV instruction shifts rt left by rs amount
 * Then: rd register should contain 0x00000010
 */
TEST_F(SLLVInstructionBDDTest, BasicVariableLeftShift)
{
    // Given: rt = 0x00000001, rs = 4
    const uint32_t rt_value        = 0x00000001;
    const uint32_t rs_value        = 4;
    const uint32_t rt_register     = 1;                    // $t0 (source value)
    const uint32_t rs_register     = 2;                    // $t1 (shift amount)
    const uint32_t rd_register     = 3;                    // $t2 (destination)
    const uint32_t expected_result = rt_value << rs_value; // 0x00000010

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SLLV instruction
    // Format: SLLV rd, rt, rs
    // Note: This will fail initially as SLLVInstruction class doesn't exist
    auto sllv_instruction =
        std::make_unique<mips::SLLVInstruction>(rd_register, rt_register, rs_register);
    sllv_instruction->execute(*cpu);

    // Then: rd register should contain the shifted result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 2: Shift Amount Truncation
 * Given: rt register contains 0x00000001, rs register contains 36 (0x24)
 * When: SLLV instruction uses only low 5 bits of rs (36 & 0x1F = 4)
 * Then: rd register should contain 0x00000010 (shifted by 4, not 36)
 */
TEST_F(SLLVInstructionBDDTest, ShiftAmountTruncation)
{
    // Given: rt = 0x00000001, rs = 36 (0x24)
    const uint32_t rt_value        = 0x00000001;
    const uint32_t rs_value        = 36;              // 0x24, but only low 5 bits (4) will be used
    const uint32_t rt_register     = 4;               // $t3
    const uint32_t rs_register     = 5;               // $t4
    const uint32_t rd_register     = 6;               // $t5
    const uint32_t effective_shift = rs_value & 0x1F; // 36 & 0x1F = 4
    const uint32_t expected_result = rt_value << effective_shift; // 0x00000010

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SLLV instruction
    auto sllv_instruction =
        std::make_unique<mips::SLLVInstruction>(rd_register, rt_register, rs_register);
    sllv_instruction->execute(*cpu);

    // Then: rd register should contain result using truncated shift amount
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 3: Zero Shift Identity Operation
 * Given: rt register contains 0x12345678, rs register contains 0
 * When: SLLV instruction shifts by 0
 * Then: rd register should contain 0x12345678 (identity operation)
 */
TEST_F(SLLVInstructionBDDTest, ZeroShiftIdentity)
{
    // Given: rt = 0x12345678, rs = 0
    const uint32_t rt_value        = 0x12345678;
    const uint32_t rs_value        = 0;
    const uint32_t rt_register     = 7;        // $t6
    const uint32_t rs_register     = 8;        // $t7
    const uint32_t rd_register     = 9;        // $t8
    const uint32_t expected_result = rt_value; // No shift, identity

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SLLV instruction with zero shift
    auto sllv_instruction =
        std::make_unique<mips::SLLVInstruction>(rd_register, rt_register, rs_register);
    sllv_instruction->execute(*cpu);

    // Then: rd register should contain the same value as rt
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 4: Maximum Shift Amount
 * Given: rt register contains 0x00000001, rs register contains 31
 * When: SLLV instruction shifts by maximum amount (31)
 * Then: rd register should contain 0x80000000
 */
TEST_F(SLLVInstructionBDDTest, MaximumShift)
{
    // Given: rt = 0x00000001, rs = 31
    const uint32_t rt_value        = 0x00000001;
    const uint32_t rs_value        = 31;                   // Maximum shift amount
    const uint32_t rt_register     = 10;                   // $t9
    const uint32_t rs_register     = 11;                   // $s0
    const uint32_t rd_register     = 12;                   // $s1
    const uint32_t expected_result = rt_value << rs_value; // 0x80000000

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SLLV instruction with maximum shift
    auto sllv_instruction =
        std::make_unique<mips::SLLVInstruction>(rd_register, rt_register, rs_register);
    sllv_instruction->execute(*cpu);

    // Then: rd register should contain the maximum shifted result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}
