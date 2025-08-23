/**
 * @file test_jalr_instruction_bdd_minimal.cpp
 * @brief BDD Tests for JALR (Jump And Link Register) instruction implementation
 *
 * Phase 6.3: JALR instruction BDD tests (Red-Light Phase)
 *
 * This file implements Behavior-Driven Development (BDD) tests for the JALR instruction.
 * JALR performs unconditional jump to address stored in a register and saves return address.
 *
 * Key characteristics of JALR:
 * - Function Code: 0x09 (R-type instruction)
 * - Format: jalr $rd, $rs (or jalr $rs with $rd=$ra)
 * - Function: $rd = PC + 4, PC = $rs
 * - Note: Jump target address should be word-aligned
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
 * BDD Test Fixture for JALR instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class JALRInstructionBDD : public ::testing::Test
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
 * Scenario 1: Basic Jump And Link Register
 * Given: PC at 0x00000200, rs register contains 0x00003000
 * When: JALR instruction jumps to rs and saves return address in rd
 * Then: PC should be 0x00003000/4, rd should contain 0x00000204
 */
TEST_F(JALRInstructionBDD, BasicJumpAndLinkRegister)
{
    // Given: PC and target address in register
    const uint32_t initial_pc              = 0x00000200 / 4; // Word address
    const uint32_t rs_register             = 8;              // $t0 (source address)
    const uint32_t rd_register             = 31;             // $ra (return address)
    const uint32_t target_address          = 0x00003000;
    const uint32_t expected_pc             = target_address / 4;
    const uint32_t expected_return_address = (initial_pc + 1) * 4;

    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(rs_register, target_address);

    // When: Execute JALR instruction
    // Note: This will fail initially as JALRInstruction class doesn't exist
    auto jalr_instruction = std::make_unique<mips::JALRInstruction>(rd_register, rs_register);
    jalr_instruction->execute(*cpu);

    // Then: PC should jump and return address should be saved
    uint32_t actual_pc             = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(rd_register);

    EXPECT_EQ(expected_pc, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}

/**
 * Scenario 2: JALR with different destination register
 * Given: PC at 0x00000300, rs contains target, rd is different register
 * When: JALR instruction saves return address in specified rd
 * Then: PC should jump, specified rd should contain return address
 */
TEST_F(JALRInstructionBDD, DifferentDestinationRegister)
{
    // Given: different destination register
    const uint32_t initial_pc              = 0x00000300 / 4;
    const uint32_t rs_register             = 16; // $s0 (source address)
    const uint32_t rd_register             = 17; // $s1 (return address register)
    const uint32_t target_address          = 0x00005678;
    const uint32_t expected_pc             = target_address / 4;
    const uint32_t expected_return_address = (initial_pc + 1) * 4;

    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(rs_register, target_address);

    // When: Execute JALR instruction with custom destination
    auto jalr_instruction = std::make_unique<mips::JALRInstruction>(rd_register, rs_register);
    jalr_instruction->execute(*cpu);

    // Then: PC should jump and return address should be saved in specified register
    uint32_t actual_pc             = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(rd_register);

    EXPECT_EQ(expected_pc, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}

/**
 * Scenario 3: Zero Address Jump
 * Given: PC at 0x00000100, rs register contains 0x00000000
 * When: JALR instruction jumps to address 0
 * Then: PC should be 0, rd should contain return address
 */
TEST_F(JALRInstructionBDD, ZeroAddressJump)
{
    // Given: zero target address
    const uint32_t initial_pc              = 0x00000100 / 4;
    const uint32_t rs_register             = 9;  // $t1
    const uint32_t rd_register             = 31; // $ra
    const uint32_t target_address          = 0x00000000;
    const uint32_t expected_pc             = 0;
    const uint32_t expected_return_address = (initial_pc + 1) * 4;

    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(rs_register, target_address);

    // When: Execute JALR to address 0
    auto jalr_instruction = std::make_unique<mips::JALRInstruction>(rd_register, rs_register);
    jalr_instruction->execute(*cpu);

    // Then: PC should be 0 and return address should be saved
    uint32_t actual_pc             = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(rd_register);

    EXPECT_EQ(expected_pc, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}

/**
 * Scenario 4: High Address Jump
 * Given: PC at 0x00000400, rs register contains high memory address
 * When: JALR instruction jumps to high address
 * Then: PC should update correctly, rd should contain return address
 */
TEST_F(JALRInstructionBDD, HighAddressJump)
{
    // Given: high memory address
    const uint32_t initial_pc              = 0x00000400 / 4;
    const uint32_t rs_register             = 10; // $t2
    const uint32_t rd_register             = 31; // $ra
    const uint32_t target_address          = 0xFFFF0000;
    const uint32_t expected_pc             = target_address / 4;
    const uint32_t expected_return_address = (initial_pc + 1) * 4;

    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(rs_register, target_address);

    // When: Execute JALR to high address
    auto jalr_instruction = std::make_unique<mips::JALRInstruction>(rd_register, rs_register);
    jalr_instruction->execute(*cpu);

    // Then: PC should jump to high address and return address should be saved
    uint32_t actual_pc             = cpu->getProgramCounter();
    uint32_t actual_return_address = cpu->getRegisterFile().read(rd_register);

    EXPECT_EQ(expected_pc, actual_pc);
    EXPECT_EQ(expected_return_address, actual_return_address);
}
