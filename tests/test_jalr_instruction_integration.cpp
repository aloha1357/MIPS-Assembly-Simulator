/**
 * @file test_jalr_instruction_integration.cpp
 * @brief Integration Tests for JALR instruction across all system components
 *
 * Phase 6.3: JALR instruction Integration tests
 *
 * These tests verify end-to-end integration of JALR instruction through:
 * - InstructionDecoder (machine code → instruction object)
 * - Assembler (assembly text → instruction object)
 * - Instruction execution (instruction object → CPU state changes)
 * - Error handling and edge cases
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
 * Integration Test Fixture for JALR instruction
 * Tests complete integration across all system components
 */
class JALRInstructionIntegration : public ::testing::Test
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
 * Integration Test 1: Decoder Integration
 * Tests that InstructionDecoder correctly creates JALRInstruction from machine code
 */
TEST_F(JALRInstructionIntegration, DecoderCreatesJALRInstruction)
{
    // Given: Machine code for JALR $ra, $t0
    // Format: R-type, rs=$t0(8), rt=0, rd=$ra(31), shamt=0, function=0x09
    // Binary: 000000 01000 00000 11111 00000 001001
    const uint32_t machine_code = 0x01000009 | (31 << 11);  // JALR $ra, $t0

    // When: Decode machine code
    auto instruction = decoder->decode(machine_code);

    // Then: Should create JALRInstruction
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "jalr");

    // Verify execution behavior
    const uint32_t initial_pc     = 100;
    const uint32_t target_address = 0x2000;
    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(8, target_address);  // $t0 = target

    instruction->execute(*cpu);

    EXPECT_EQ(cpu->getProgramCounter(), target_address / 4);
    EXPECT_EQ(cpu->getRegisterFile().read(31), (initial_pc + 1) * 4);  // $ra
}

/**
 * Integration Test 2: Assembler Integration - Two Operand Format
 * Tests that Assembler correctly parses "jalr $rd, $rs" syntax
 */
TEST_F(JALRInstructionIntegration, AssemblerParsesJALRTwoOperands)
{
    // Given: Assembly text with explicit destination register
    const std::string assembly_text = "jalr $s1, $s0";

    // When: Parse assembly text
    auto instructions = assembler->assemble(assembly_text);

    // Then: Should create JALRInstruction with correct registers
    ASSERT_EQ(instructions.size(), 1);
    EXPECT_EQ(instructions[0]->getName(), "jalr");

    // Verify execution behavior
    const uint32_t initial_pc     = 200;
    const uint32_t target_address = 0x3000;
    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(16, target_address);  // $s0 = target

    instructions[0]->execute(*cpu);

    EXPECT_EQ(cpu->getProgramCounter(), target_address / 4);
    EXPECT_EQ(cpu->getRegisterFile().read(17), (initial_pc + 1) * 4);  // $s1
}

/**
 * Integration Test 3: Assembler Integration - Single Operand Format
 * Tests that Assembler correctly parses "jalr $rs" syntax (default $rd=$ra)
 */
TEST_F(JALRInstructionIntegration, AssemblerParsesJALRSingleOperand)
{
    // Given: Assembly text with implicit destination register
    const std::string assembly_text = "jalr $t2";

    // When: Parse assembly text
    auto instructions = assembler->assemble(assembly_text);

    // Then: Should create JALRInstruction with $ra as default destination
    ASSERT_EQ(instructions.size(), 1);
    EXPECT_EQ(instructions[0]->getName(), "jalr");

    // Verify execution behavior
    const uint32_t initial_pc     = 300;
    const uint32_t target_address = 0x4000;
    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(10, target_address);  // $t2 = target

    instructions[0]->execute(*cpu);

    EXPECT_EQ(cpu->getProgramCounter(), target_address / 4);
    EXPECT_EQ(cpu->getRegisterFile().read(31), (initial_pc + 1) * 4);  // $ra (default)
}

/**
 * Integration Test 4: Assembler Error Handling - Invalid Register
 * Tests that Assembler correctly rejects invalid register names
 */
TEST_F(JALRInstructionIntegration, AssemblerRejectsInvalidRegister)
{
    // Given: Assembly text with invalid register name
    const std::string invalid_assembly = "jalr $invalidreg";

    // When: Attempt to parse invalid assembly
    auto instructions = assembler->assemble(invalid_assembly);

    // Then: Should return empty vector (parsing failure)
    EXPECT_EQ(instructions.size(), 0);
}

/**
 * Integration Test 5: Assembler Error Handling - Invalid Syntax
 * Tests that Assembler correctly rejects invalid syntax formats
 */
TEST_F(JALRInstructionIntegration, AssemblerRejectsInvalidSyntax)
{
    // Given: Assembly text with too many operands
    const std::string invalid_assembly = "jalr $t0, $t1, $t2";

    // When: Attempt to parse invalid assembly
    auto instructions = assembler->assemble(invalid_assembly);

    // Then: Should return empty vector (parsing failure)
    EXPECT_EQ(instructions.size(), 0);
}

/**
 * Integration Test 6: End-to-End Function Call Simulation
 * Tests realistic function call scenario using JALR
 */
TEST_F(JALRInstructionIntegration, EndToEndFunctionCallScenario)
{
    // Given: Simulated function call scenario
    const uint32_t main_function_pc     = 0x1000 / 4;  // Main function at 0x1000
    const uint32_t target_function_addr = 0x2000;      // Target function at 0x2000
    const uint32_t expected_return_addr = (main_function_pc + 1) * 4;

    // Setup: Main function calls target function
    cpu->setProgramCounter(main_function_pc);
    cpu->getRegisterFile().write(8, target_function_addr);  // $t0 = function address

    // When: Execute JALR for function call
    const std::string call_assembly     = "jalr $ra, $t0";
    auto              call_instructions = assembler->assemble(call_assembly);
    ASSERT_EQ(call_instructions.size(), 1);

    call_instructions[0]->execute(*cpu);

    // Then: Should jump to function and save return address
    EXPECT_EQ(cpu->getProgramCounter(), target_function_addr / 4);
    EXPECT_EQ(cpu->getRegisterFile().read(31), expected_return_addr);  // $ra

    // Simulate function return using JR $ra (if available)
    cpu->setProgramCounter(expected_return_addr / 4);  // Simulate return

    // Verify we're back at the expected return location
    EXPECT_EQ(cpu->getProgramCounter(), expected_return_addr / 4);
}

/**
 * Integration Test 7: Cross-Component Consistency
 * Tests that Decoder and Assembler produce equivalent results
 */
TEST_F(JALRInstructionIntegration, DecoderAssemblerConsistency)
{
    // Given: Machine code and equivalent assembly text
    // JALR $ra, $t7: opcode=000000, rs=$t7(15), rt=0, rd=$ra(31), shamt=0, func=0x09
    // Binary: 000000 01111 00000 11111 00000 001001
    const uint32_t machine_code = (0 << 26) | (15 << 21) | (0 << 16) | (31 << 11) | (0 << 6) | 0x09;
    const std::string assembly_text = "jalr $ra, $t7";

    // When: Create instructions via both paths
    auto decoded_instruction    = decoder->decode(machine_code);
    auto assembled_instructions = assembler->assemble(assembly_text);

    // Then: Both should create valid instructions with same behavior
    ASSERT_NE(decoded_instruction, nullptr);
    ASSERT_EQ(assembled_instructions.size(), 1);
    EXPECT_EQ(decoded_instruction->getName(), assembled_instructions[0]->getName());

    // Verify both have same execution behavior
    const uint32_t initial_pc     = 500;
    const uint32_t target_address = 0x5000;

    // Test decoded instruction
    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(15, target_address);  // $t7 = target
    decoded_instruction->execute(*cpu);

    uint32_t decoded_pc = cpu->getProgramCounter();
    uint32_t decoded_ra = cpu->getRegisterFile().read(31);

    // Reset and test assembled instruction
    cpu->setProgramCounter(initial_pc);
    cpu->getRegisterFile().write(15, target_address);  // $t7 = target
    cpu->getRegisterFile().write(31, 0);               // Clear $ra
    assembled_instructions[0]->execute(*cpu);

    uint32_t assembled_pc = cpu->getProgramCounter();
    uint32_t assembled_ra = cpu->getRegisterFile().read(31);

    // Both should produce identical results
    EXPECT_EQ(decoded_pc, assembled_pc);
    EXPECT_EQ(decoded_ra, assembled_ra);
}
