/**
 * @file test_lb_instruction_bdd_minimal.cpp
 * @brief BDD Tests for LB (Load Byte) instruction implementation
 *
 * Phase 7.1: LB instruction BDD tests (Red-Light Phase)
 *
 * This file implements Behavior-Driven Development (BDD) tests for the LB instruction.
 * LB loads a byte from memory and sign-extends it to 32 bits.
 *
 * Key characteristics of LB:
 * - Opcode: 0x20 (I-type instruction)
 * - Format: lb $rt, offset($rs)
 * - Function: $rt = sign_extend(memory[rs + offset][7:0])
 * - Note: No alignment requirement, can access any byte address
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

/**
 * BDD Test Fixture for LB instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class LBInstructionBDD : public ::testing::Test
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
 * Scenario 1: Basic Byte Load - Positive Value
 * Given: Memory address 0x1000 contains byte value 0x42
 * When: LB instruction loads byte from memory with zero offset
 * Then: Target register should contain 0x00000042 (sign-extended)
 */
TEST_F(LBInstructionBDD, BasicByteLoad_PositiveValue)
{
    // Given: Memory contains positive byte value
    const uint32_t base_address    = 0x1000;
    const uint32_t rs_register     = 8; // $t0 (base address register)
    const uint32_t rt_register     = 9; // $t1 (target register)
    const int16_t  offset          = 0;
    const uint8_t  byte_value      = 0x42;       // Positive byte
    const uint32_t expected_result = 0x00000042; // Sign-extended positive

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getMemory().writeByte(base_address + offset, byte_value);

    // When: Execute LB instruction
    // Note: This will fail initially as LBInstruction class doesn't exist
    auto lb_instruction = std::make_unique<mips::LBInstruction>(rt_register, rs_register, offset);
    lb_instruction->execute(*cpu);

    // Then: Register should contain sign-extended byte value
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}

/**
 * Scenario 2: Sign Extension - Negative Byte Value
 * Given: Memory address 0x2000 contains byte value 0x80 (negative in signed representation)
 * When: LB instruction loads byte from memory
 * Then: Target register should contain 0xFFFFFF80 (sign-extended negative)
 */
TEST_F(LBInstructionBDD, SignExtension_NegativeByteValue)
{
    // Given: Memory contains negative byte value
    const uint32_t base_address    = 0x2000;
    const uint32_t rs_register     = 10; // $t2 (base address register)
    const uint32_t rt_register     = 11; // $t3 (target register)
    const int16_t  offset          = 0;
    const uint8_t  byte_value      = 0x80;       // Negative byte (MSB = 1)
    const uint32_t expected_result = 0xFFFFFF80; // Sign-extended negative

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getMemory().writeByte(base_address + offset, byte_value);

    // When: Execute LB instruction
    auto lb_instruction = std::make_unique<mips::LBInstruction>(rt_register, rs_register, offset);
    lb_instruction->execute(*cpu);

    // Then: Register should contain sign-extended negative value
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}

/**
 * Scenario 3: Zero Offset Load
 * Given: Base register contains address 0x3000, offset is 0
 * When: LB instruction loads byte from direct base address
 * Then: Byte should be loaded from exactly base address
 */
TEST_F(LBInstructionBDD, ZeroOffset_DirectBaseAddress)
{
    // Given: Zero offset, direct address access
    const uint32_t base_address    = 0x3000;
    const uint32_t rs_register     = 12; // $t4 (base address register)
    const uint32_t rt_register     = 13; // $t5 (target register)
    const int16_t  offset          = 0;
    const uint8_t  byte_value      = 0x55; // Arbitrary positive byte
    const uint32_t expected_result = 0x00000055;

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getMemory().writeByte(base_address, byte_value);

    // When: Execute LB instruction with zero offset
    auto lb_instruction = std::make_unique<mips::LBInstruction>(rt_register, rs_register, offset);
    lb_instruction->execute(*cpu);

    // Then: Register should contain the byte value from exact address
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}

/**
 * Scenario 4: Negative Offset Load
 * Given: Base register contains address 0x4000, offset is -4
 * When: LB instruction calculates effective address (base + offset)
 * Then: Byte should be loaded from address 0x3FFC
 */
TEST_F(LBInstructionBDD, NegativeOffset_CalculateEffectiveAddress)
{
    // Given: Negative offset for address calculation
    const uint32_t base_address      = 0x4000;
    const uint32_t rs_register       = 14; // $t6 (base address register)
    const uint32_t rt_register       = 15; // $t7 (target register)
    const int16_t  offset            = -4;
    const uint32_t effective_address = base_address + offset; // 0x3FFC
    const uint8_t  byte_value        = 0xAA;                  // Negative byte value
    const uint32_t expected_result   = 0xFFFFFFAA;            // Sign-extended

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getMemory().writeByte(effective_address, byte_value);

    // When: Execute LB instruction with negative offset
    auto lb_instruction = std::make_unique<mips::LBInstruction>(rt_register, rs_register, offset);
    lb_instruction->execute(*cpu);

    // Then: Register should contain sign-extended byte from calculated address
    uint32_t actual_result = cpu->getRegisterFile().read(rt_register);
    EXPECT_EQ(expected_result, actual_result);

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}
