/**
 * @file test_jal_instruction_bdd_minimal.cpp
 * @brief BDD Tests for JAL (Jump And Link) instruction implementation
 * 
 * Phase 6.2: JAL instruction BDD tests (Red-Light Phase)
 * 
 * This file implements Behavior-Driven Development (BDD) tests for the JAL instruction.
 * JAL performs unconditional jump to immediate address and saves return address.
 * 
 * Key characteristics of JAL:
 * - Opcode: 0x03 (J-type instruction)
 * - Format: jal target
 * - Function: $ra = PC+4; PC = target
 * - Note: Return address saved in $ra register (31)
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
 * BDD Test Fixture for JAL instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class JALInstructionBDD : public ::testing::Test {
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
 * Scenario 1: Basic Jump And Link
 * Given: PC at 0x00000100, target address 0x2000
 * When: JAL instruction jumps to target and saves return address
 * Then: PC should be 0x2000, $ra should contain 0x00000104
 */
TEST_F(JALInstructionBDD, BasicJumpAndLink) {
    // Given: PC at specific address
    const uint32_t initial_pc = 0x00000100 / 4;  // Word address
    const uint32_t target_address = 0x2000;      // Word address
    const uint32_t expected_return_address = (initial_pc + 1) * 4;  // Byte address
    
    cpu->setProgramCounter(initial_pc);
    
    // When: Execute JAL instruction
    auto jal_instruction = std::make_unique<mips::JALInstruction>(target_address);
    jal_instruction->execute(*cpu);
    
    // Then: PC should jump to target and $ra should contain return address
    uint32_t actual_pc = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(31);  // $ra = 31
    
    EXPECT_EQ(target_address, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}

/**
 * Scenario 2: Different Target Address
 * Given: PC at 0x00000200, target address 0x3000
 * When: JAL instruction jumps to different target
 * Then: PC should be 0x3000, $ra should contain 0x00000204
 */
TEST_F(JALInstructionBDD, DifferentTargetAddress) {
    // Given: different initial PC and target
    const uint32_t initial_pc = 0x00000200 / 4;
    const uint32_t target_address = 0x3000;
    const uint32_t expected_return_address = (initial_pc + 1) * 4;
    
    cpu->setProgramCounter(initial_pc);
    
    // When: Execute JAL instruction
    auto jal_instruction = std::make_unique<mips::JALInstruction>(target_address);
    jal_instruction->execute(*cpu);
    
    // Then: Verify jump and return address
    uint32_t actual_pc = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(31);
    
    EXPECT_EQ(target_address, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}

/**
 * Scenario 3: Zero Target Address
 * Given: PC at 0x00000300, target address 0x0000
 * When: JAL instruction jumps to zero address
 * Then: PC should be 0, $ra should contain 0x00000304
 */
TEST_F(JALInstructionBDD, ZeroTargetAddress) {
    // Given: initial PC and zero target
    const uint32_t initial_pc = 0x00000300 / 4;
    const uint32_t target_address = 0x0000;
    const uint32_t expected_return_address = (initial_pc + 1) * 4;
    
    cpu->setProgramCounter(initial_pc);
    
    // When: Execute JAL instruction
    auto jal_instruction = std::make_unique<mips::JALInstruction>(target_address);
    jal_instruction->execute(*cpu);
    
    // Then: Verify jump to zero and return address
    uint32_t actual_pc = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(31);
    
    EXPECT_EQ(target_address, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}

/**
 * Scenario 4: High Target Address
 * Given: PC at 0x00000400, target address 0x7FFF
 * When: JAL instruction jumps to high address
 * Then: PC should be 0x7FFF, $ra should contain 0x00000404
 */
TEST_F(JALInstructionBDD, HighTargetAddress) {
    // Given: initial PC and high target
    const uint32_t initial_pc = 0x00000400 / 4;
    const uint32_t target_address = 0x7FFF;
    const uint32_t expected_return_address = (initial_pc + 1) * 4;
    
    cpu->setProgramCounter(initial_pc);
    
    // When: Execute JAL instruction
    auto jal_instruction = std::make_unique<mips::JALInstruction>(target_address);
    jal_instruction->execute(*cpu);
    
    // Then: Verify jump to high address and return address
    uint32_t actual_pc = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(31);
    
    EXPECT_EQ(target_address, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}
