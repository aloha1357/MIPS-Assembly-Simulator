/**
 * @file test_jr_instruction_bdd_minimal.cpp
 * @brief BDD Tests for JR (Jump Register) instruction implementation
 * 
 * Phase 6.1: JR instruction BDD tests (Red-Light Phase)
 * 
 * This file implements Behavior-Driven Development (BDD) tests for the JR instruction.
 * JR performs unconditional jump to address stored in a register.
 * 
 * Key characteristics of JR:
 * - Function Code: 0x08 (R-type instruction)
 * - Format: jr $rs
 * - Function: PC = $rs
 * - Note: Jump target address should be word-aligned
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
 * BDD Test Fixture for JR instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class JRInstructionBDD : public ::testing::Test {
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
 * Scenario 1: Basic Register Jump
 * Given: rs register contains 0x00001000
 * When: JR instruction jumps to address in rs
 * Then: PC should be set to 0x00001000 / 4 = 0x400
 */
TEST_F(JRInstructionBDD, BasicRegisterJump) {
    // Given: rs register contains target address
    const uint32_t rs_register = 8;  // $t0
    const uint32_t target_address = 0x00001000;
    const uint32_t expected_pc = target_address / 4;  // Word address
    
    cpu->getRegisterFile().write(rs_register, target_address);
    
    // When: Execute JR instruction
    auto jr_instruction = std::make_unique<mips::JRInstruction>(rs_register);
    jr_instruction->execute(*cpu);
    
    // Then: PC should be set to target address
    uint32_t actual_pc = cpu->getProgramCounter();
    EXPECT_EQ(expected_pc, actual_pc);
}

/**
 * Scenario 2: Different Register Jump
 * Given: different rs register contains 0x00002000
 * When: JR instruction jumps to address in rs
 * Then: PC should be set to 0x00002000 / 4 = 0x800
 */
TEST_F(JRInstructionBDD, DifferentRegisterJump) {
    // Given: different source register with target address
    const uint32_t rs_register = 16;  // $s0
    const uint32_t target_address = 0x00002000;
    const uint32_t expected_pc = target_address / 4;
    
    cpu->getRegisterFile().write(rs_register, target_address);
    cpu->setProgramCounter(0x00000100 / 4);  // Set initial PC
    
    // When: Execute JR instruction
    auto jr_instruction = std::make_unique<mips::JRInstruction>(rs_register);
    jr_instruction->execute(*cpu);
    
    // Then: PC should be set to target address
    uint32_t actual_pc = cpu->getProgramCounter();
    EXPECT_EQ(expected_pc, actual_pc);
}

/**
 * Scenario 3: Zero Address Jump
 * Given: rs register contains 0x00000000
 * When: JR instruction jumps to zero address
 * Then: PC should be set to 0
 */
TEST_F(JRInstructionBDD, ZeroAddressJump) {
    // Given: rs register contains zero address
    const uint32_t rs_register = 8;  // $t0
    const uint32_t target_address = 0x00000000;
    const uint32_t expected_pc = 0;
    
    cpu->getRegisterFile().write(rs_register, target_address);
    
    // When: Execute JR instruction
    auto jr_instruction = std::make_unique<mips::JRInstruction>(rs_register);
    jr_instruction->execute(*cpu);
    
    // Then: PC should be set to zero
    uint32_t actual_pc = cpu->getProgramCounter();
    EXPECT_EQ(expected_pc, actual_pc);
}

/**
 * Scenario 4: High Address Jump
 * Given: rs register contains 0x7FFFF000
 * When: JR instruction jumps to high address
 * Then: PC should be set to 0x7FFFF000 / 4
 */
TEST_F(JRInstructionBDD, HighAddressJump) {
    // Given: rs register contains high address
    const uint32_t rs_register = 8;  // $t0
    const uint32_t target_address = 0x7FFFF000;
    const uint32_t expected_pc = target_address / 4;
    
    cpu->getRegisterFile().write(rs_register, target_address);
    
    // When: Execute JR instruction
    auto jr_instruction = std::make_unique<mips::JRInstruction>(rs_register);
    jr_instruction->execute(*cpu);
    
    // Then: PC should be set to high address
    uint32_t actual_pc = cpu->getProgramCounter();
    EXPECT_EQ(expected_pc, actual_pc);
}
