/**
 * @file test_srlv_instruction_bdd_minimal.cpp
 * @brief BDD Tests for SRLV (Shift Right Logical Variable) instruction implementation
 * 
 * Phase 5.2: SRLV instruction BDD tests (Red-Light Phase)
 * 
 * This file implements Behavior-Driven Development (BDD) tests for the SRLV instruction.
 * SRLV performs variable logical right shift where shift amount comes from a register.
 * 
 * Key characteristics of SRLV:
 * - Function Code: 0x06 (R-type instruction)
 * - Format: srlv $rd, $rt, $rs
 * - Function: rd = rt >> (rs & 0x1F)
 * - Note: Only low 5 bits of rs register are used for shift amount (0-31)
 * - Logical shift: zero extension from left
 */

#include <gtest/gtest.h>
#include <cstdint>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Instruction.h"
#include "../src/Assembler.h"
#include "../src/InstructionDecoder.h"
#include <memory>

/**
 * BDD Test Fixture for SRLV instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class SRLVInstructionBDDTest : public ::testing::Test {
protected:
    std::unique_ptr<mips::Cpu> cpu;
    std::unique_ptr<mips::Assembler> assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;
    
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder = std::make_unique<mips::InstructionDecoder>();
        
        // Clear all registers
        for (int i = 0; i < 32; ++i) {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }

    void TearDown() override {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }
};

/**
 * Scenario 1: Basic Variable Logical Right Shift
 * Given: rt register contains 0x80000000, rs register contains 4
 * When: SRLV instruction shifts rt right logically by rs amount
 * Then: rd register should contain 0x08000000
 */
TEST_F(SRLVInstructionBDDTest, BasicVariableLogicalRightShift) {
    // Given: rt = 0x80000000, rs = 4
    const uint32_t rt_value = 0x80000000;
    const uint32_t rs_value = 4;
    const uint32_t rt_register = 1;  // $t0 (source value)
    const uint32_t rs_register = 2;  // $t1 (shift amount)
    const uint32_t rd_register = 3;  // $t2 (destination)
    const uint32_t expected_result = rt_value >> rs_value;  // 0x08000000

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRLV instruction
    // Format: SRLV rd, rt, rs
    // Note: This will fail initially as SRLVInstruction class doesn't exist
    auto srlv_instruction = std::make_unique<mips::SRLVInstruction>(
        rd_register, rt_register, rs_register
    );
    srlv_instruction->execute(*cpu);

    // Then: rd register should contain the logically shifted result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 2: Shift Amount Truncation with Logical Shift
 * Given: rt register contains 0xFFFFFFFF, rs register contains 35 (0x23)
 * When: SRLV instruction uses only low 5 bits of rs (35 & 0x1F = 3)
 * Then: rd register should contain 0x1FFFFFFF (shifted by 3, not 35)
 */
TEST_F(SRLVInstructionBDDTest, ShiftAmountTruncationLogical) {
    // Given: rt = 0xFFFFFFFF, rs = 35 (0x23)
    const uint32_t rt_value = 0xFFFFFFFF;
    const uint32_t rs_value = 35;  // 0x23, but only low 5 bits (3) will be used
    const uint32_t rt_register = 4;  // $t3
    const uint32_t rs_register = 5;  // $t4
    const uint32_t rd_register = 6;  // $t5
    const uint32_t effective_shift = rs_value & 0x1F;  // 35 & 0x1F = 3
    const uint32_t expected_result = rt_value >> effective_shift;  // 0x1FFFFFFF

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRLV instruction
    auto srlv_instruction = std::make_unique<mips::SRLVInstruction>(
        rd_register, rt_register, rs_register
    );
    srlv_instruction->execute(*cpu);

    // Then: rd register should contain result using truncated shift amount
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 3: Zero Shift Identity Operation
 * Given: rt register contains 0xABCDEF12, rs register contains 0
 * When: SRLV instruction shifts by 0
 * Then: rd register should contain 0xABCDEF12 (identity operation)
 */
TEST_F(SRLVInstructionBDDTest, ZeroShiftIdentity) {
    // Given: rt = 0xABCDEF12, rs = 0
    const uint32_t rt_value = 0xABCDEF12;
    const uint32_t rs_value = 0;
    const uint32_t rt_register = 7;  // $t6
    const uint32_t rs_register = 8;  // $t7
    const uint32_t rd_register = 9;  // $t8
    const uint32_t expected_result = rt_value;  // No shift, identity

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRLV instruction with zero shift
    auto srlv_instruction = std::make_unique<mips::SRLVInstruction>(
        rd_register, rt_register, rs_register
    );
    srlv_instruction->execute(*cpu);

    // Then: rd register should contain the same value as rt
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 4: Maximum Shift Amount with Zero Extension
 * Given: rt register contains 0x80000001, rs register contains 31
 * When: SRLV instruction shifts by maximum amount (31) with zero extension
 * Then: rd register should contain 0x00000001 (zero extended from left)
 */
TEST_F(SRLVInstructionBDDTest, MaximumShiftZeroExtension) {
    // Given: rt = 0x80000001, rs = 31
    const uint32_t rt_value = 0x80000001;
    const uint32_t rs_value = 31;  // Maximum shift amount
    const uint32_t rt_register = 10; // $t9
    const uint32_t rs_register = 11; // $s0
    const uint32_t rd_register = 12; // $s1
    const uint32_t expected_result = rt_value >> rs_value;  // 0x00000001 (logical shift)

    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute SRLV instruction with maximum shift
    auto srlv_instruction = std::make_unique<mips::SRLVInstruction>(
        rd_register, rt_register, rs_register
    );
    srlv_instruction->execute(*cpu);

    // Then: rd register should contain the zero-extended result
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result);
}
