/**
 * @file test_addiu_instruction_bdd_minimal.cpp
 * @brief BDD Tests for ADDIU (Add Immediate Unsigned) instruction implementation
 * 
 * Phase 4.1: ADDIU instruction BDD tests (Red-Light Phase)
 * 
 * This file implements Behavior-Driven Development (BDD) tests for the ADDIU instruction.
 * ADDIU performs unsigned immediate addition without overflow checking.
 * 
 * Key characteristics of ADDIU:
 * - Opcode: 0x09 (I-type instruction)
 * - Format: addiu $rt, $rs, immediate
 * - Function: rt = rs + sign_extend(immediate) (no overflow exception)
 * - Note: Despite "unsigned" name, immediate is sign-extended
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
 * BDD Test Fixture for ADDIU instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class ADDIUInstructionBDDTest : public ::testing::Test {
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
 * Scenario 1: Basic Immediate Addition
 * Given: rs register contains a positive value
 * When: ADDIU instruction adds a positive immediate value
 * Then: rt register should contain the sum without overflow check
 */
TEST_F(ADDIUInstructionBDDTest, BasicImmediateAddition) {
    // Given: rs register contains 0x12345678
    const uint32_t rs_value = 0x12345678;
    const uint32_t rs_register = 1;  // $t0
    const uint32_t rt_register = 2;  // $t1
    const uint16_t immediate = 0x1000;  // Positive immediate
    const uint32_t expected_result = rs_value + immediate;

    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute ADDIU instruction
    // Format: ADDIU rt, rs, immediate
    // Encoding: 0x09 (opcode) | rs (5 bits) | rt (5 bits) | immediate (16 bits)
    uint32_t instruction_encoding = (0x09 << 26) | (rs_register << 21) | (rt_register << 16) | immediate;
    (void)instruction_encoding; // Suppress unused variable warning
    
    // Create ADDIU instruction and execute
    // Note: This will fail initially as ADDIUInstruction class doesn't exist
    auto addiu_instruction = std::make_unique<mips::ADDIUInstruction>(
        rt_register, rs_register, immediate
    );
    addiu_instruction->execute(*cpu);

    // Then: rt register should contain the expected sum
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 2: Negative Immediate Addition (Actually Subtraction)
 * Given: rs register contains a positive value
 * When: ADDIU instruction adds a negative immediate (sign-extended)
 * Then: rt register should contain rs + sign_extend(immediate)
 */
TEST_F(ADDIUInstructionBDDTest, NegativeImmediateAddition) {
    // Given: rs register contains 0x10000000
    const uint32_t rs_value = 0x10000000;
    const uint32_t rs_register = 3;  // $t2
    const uint32_t rt_register = 4;  // $t3
    const uint16_t immediate = 0xFFFF;  // -1 when sign-extended
    const uint32_t expected_result = rs_value + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(immediate)));

    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute ADDIU instruction with negative immediate
    auto addiu_instruction = std::make_unique<mips::ADDIUInstruction>(
        rt_register, rs_register, immediate
    );
    addiu_instruction->execute(*cpu);

    // Then: rt register should contain rs + sign_extend(immediate)
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 3: Overflow No Exception Test
 * Given: rs register contains a large positive value
 * When: ADDIU instruction causes arithmetic overflow
 * Then: Result should wrap around without generating exception
 */
TEST_F(ADDIUInstructionBDDTest, OverflowNoException) {
    // Given: rs register contains maximum positive value
    const uint32_t rs_value = 0xFFFFFFFF;  // Maximum 32-bit value
    const uint32_t rs_register = 5;  // $t4
    const uint32_t rt_register = 6;  // $t5
    const uint16_t immediate = 0x0001;  // Small positive immediate
    const uint32_t expected_result = rs_value + immediate;  // Should wrap to 0x00000000

    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute ADDIU instruction causing overflow
    auto addiu_instruction = std::make_unique<mips::ADDIUInstruction>(
        rt_register, rs_register, immediate
    );
    
    // Execute should not throw exception despite overflow
    EXPECT_NO_THROW(addiu_instruction->execute(*cpu));

    // Then: rt register should contain wrapped result
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);
}

/**
 * Scenario 4: Zero Immediate Identity Operation
 * Given: rs register contains any value
 * When: ADDIU instruction adds zero immediate
 * Then: rt register should contain the same value as rs (identity operation)
 */
TEST_F(ADDIUInstructionBDDTest, ZeroImmediateIdentity) {
    // Given: rs register contains test value
    const uint32_t rs_value = 0xABCDEF12;
    const uint32_t rs_register = 7;  // $t6
    const uint32_t rt_register = 8;  // $t7
    const uint16_t immediate = 0x0000;  // Zero immediate
    const uint32_t expected_result = rs_value;  // Should be identical

    cpu->getRegisterFile().write(rs_register, rs_value);

    // When: Execute ADDIU instruction with zero immediate
    auto addiu_instruction = std::make_unique<mips::ADDIUInstruction>(
        rt_register, rs_register, immediate
    );
    addiu_instruction->execute(*cpu);

    // Then: rt register should contain the same value as rs
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);
}
