/**
 * @file test_jr_instruction_integration.cpp
 * @brief Integration Tests for JR (Jump Register) instruction
 *
 * Phase 6.1: JR instruction Integration tests
 *
 * This file implements Integration tests for the JR instruction,
 * testing the interaction between Decoder, Assembler, and Execution.
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
 * Integration Test Fixture for JR instruction
 * Tests end-to-end functionality including Assembly → Decoding → Execution
 */
class JRInstructionIntegrationTest : public ::testing::Test
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
 * Test JR instruction decoding functionality
 * Verify that machine code is correctly decoded to JRInstruction
 */
TEST_F(JRInstructionIntegrationTest, JRInstruction_DecoderIntegration_ShouldDecodeCorrectly)
{
    // Given: JR machine code (R-type, function=0x08, rs=$t0)
    // Format: [opcode:6][rs:5][rt:5][rd:5][shamt:5][function:6]
    // JR $t0:  [000000][01000][00000][00000][00000][001000] = 0x01000008
    uint32_t machineCode = 0x01000008;

    // When: Decode the instruction
    auto instruction = decoder->decode(machineCode);

    // Then: Should decode to JRInstruction
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "jr");
}

/**
 * Test JR instruction with different register variants
 */
TEST_F(JRInstructionIntegrationTest, JRInstruction_DecoderVariants_ShouldDecodeCorrectly)
{
    // Test JR $ra (rs=$ra=31)
    // JR $ra:  [000000][11111][00000][00000][00000][001000] = 0x03E00008
    uint32_t machineCode_ra = 0x03E00008;

    auto instruction_ra = decoder->decode(machineCode_ra);
    ASSERT_NE(instruction_ra, nullptr);
    EXPECT_EQ(instruction_ra->getName(), "jr");

    // Test JR $s0 (rs=$s0=16)
    // JR $s0:  [000000][10000][00000][00000][00000][001000] = 0x02000008
    uint32_t machineCode_s0 = 0x02000008;

    auto instruction_s0 = decoder->decode(machineCode_s0);
    ASSERT_NE(instruction_s0, nullptr);
    EXPECT_EQ(instruction_s0->getName(), "jr");
}

/**
 * Test JR instruction assembler integration
 * Verify that assembly code is correctly parsed to JRInstruction
 */
TEST_F(JRInstructionIntegrationTest, JRInstruction_AssemblerIntegration_ShouldParseCorrectly)
{
    // Given: JR assembly code
    std::string assembly = "jr $t0";

    // When: Assemble the instruction
    auto instructions = assembler->assemble(assembly);

    // Then: Should parse to one JRInstruction
    ASSERT_EQ(instructions.size(), 1);
    EXPECT_EQ(instructions[0]->getName(), "jr");
}

/**
 * Test JR instruction assembler with different register variants
 */
TEST_F(JRInstructionIntegrationTest, JRInstruction_AssemblerVariants_ShouldParseCorrectly)
{
    // Test various register formats
    std::vector<std::string> variants = {"jr $ra", "jr $s0", "jr $t9", "jr $v0"};

    for (const auto& variant : variants)
    {
        auto instructions = assembler->assemble(variant);
        ASSERT_EQ(instructions.size(), 1) << "Failed for: " << variant;
        EXPECT_EQ(instructions[0]->getName(), "jr") << "Failed for: " << variant;
    }
}

/**
 * Test JR instruction assembler with invalid syntax
 */
TEST_F(JRInstructionIntegrationTest, JRInstruction_AssemblerInvalidSyntax_ShouldReturnEmpty)
{
    // Test invalid syntax variations
    std::vector<std::string> invalidSyntax = {
        "jr",          // Missing register
        "jr $t0, $t1", // Too many operands
        "jr 123",      // Invalid register format
        "jr $invalid"  // Non-existent register
    };

    for (const auto& invalid : invalidSyntax)
    {
        auto instructions = assembler->assemble(invalid);
        EXPECT_TRUE(instructions.empty()) << "Should fail for: " << invalid;
    }
}

/**
 * Test end-to-end JR instruction integration
 * Assembly → Decoding → Execution
 */
TEST_F(JRInstructionIntegrationTest, JRInstruction_EndToEndIntegration_ShouldWorkCorrectly)
{
    // Given: Assembly code and setup
    std::string    assembly       = "jr $t0";
    const uint32_t target_address = 0x00002000;
    const uint32_t expected_pc    = target_address / 4;

    // Setup target address in $t0
    cpu->getRegisterFile().write(8, target_address); // $t0 = 8

    // When: Assemble, decode and execute
    auto instructions = assembler->assemble(assembly);
    ASSERT_EQ(instructions.size(), 1);

    instructions[0]->execute(*cpu);

    // Then: PC should jump to target address
    uint32_t actual_pc = cpu->getProgramCounter();
    EXPECT_EQ(expected_pc, actual_pc);
}

/**
 * Test JR instruction execution behavior
 * Verify jump functionality works correctly
 */
TEST_F(JRInstructionIntegrationTest, JRInstruction_JumpBehaviorIntegration_ShouldBeAccurate)
{
    // Given: JR instruction and various target addresses
    std::vector<uint32_t> test_addresses = {
        0x00000000, // Zero address
        0x00001000, // Low address
        0x00010000, // Medium address
        0x7FFFF000  // High address
    };

    for (uint32_t target_address : test_addresses)
    {
        // Setup
        cpu->getRegisterFile().write(8, target_address); // $t0 = 8
        cpu->setProgramCounter(0x1000);                  // Reset PC

        // Execute JR
        auto jr_instruction = std::make_unique<mips::JRInstruction>(8);
        jr_instruction->execute(*cpu);

        // Verify jump
        uint32_t expected_pc = target_address / 4;
        uint32_t actual_pc   = cpu->getProgramCounter();
        EXPECT_EQ(expected_pc, actual_pc)
            << "Failed for target address: 0x" << std::hex << target_address;
    }
}
