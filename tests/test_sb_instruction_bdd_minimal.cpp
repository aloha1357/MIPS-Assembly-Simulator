/**
 * @file test_sb_instruction_bdd_minimal.cpp
 * @brief BDD Tests for SB (Store Byte) instruction implementation
 *
 * Phase 7.2: SB instruction BDD tests (Red-Light Phase)
 *
 * This file implements Behavior-Driven Development (BDD) tests for the SB instruction.
 * SB stores the low-order 8 bits of a register to memory at a byte address.
 *
 * Key characteristics of SB:
 * - Opcode: 0x28 (I-type instruction)
 * - Format: sb $rt, offset($rs)
 * - Function: memory[rs + offset][7:0] = rt[7:0]
 * - Note: No alignment requirement, can store to any byte address
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
 * BDD Test Fixture for SB instruction
 * Follows Given-When-Then pattern for behavior verification
 */
class SBInstructionBDD : public ::testing::Test
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
 * Scenario 1: Basic Byte Store - Low 8 Bits
 * Given: Register contains 0x12345678, base address 0x1000
 * When: SB instruction stores low byte to memory with zero offset
 * Then: Memory at 0x1000 should contain 0x78 (low 8 bits)
 */
TEST_F(SBInstructionBDD, BasicByteStore_Low8Bits)
{
    // Given: Register contains value with specific low byte
    const uint32_t base_address   = 0x1000;
    const uint32_t rs_register    = 8; // $t0 (base address register)
    const uint32_t rt_register    = 9; // $t1 (source register)
    const int16_t  offset         = 0;
    const uint32_t register_value = 0x12345678;
    const uint8_t  expected_byte  = 0x78; // Low 8 bits

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getRegisterFile().write(rt_register, register_value);

    // When: Execute SB instruction
    // Note: This will fail initially as SBInstruction class doesn't exist
    auto sb_instruction = std::make_unique<mips::SBInstruction>(rt_register, rs_register, offset);
    sb_instruction->execute(*cpu);

    // Then: Memory should contain the low 8 bits
    uint8_t actual_byte = cpu->getMemory().readByte(base_address + offset);
    EXPECT_EQ(expected_byte, actual_byte);

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}

/**
 * Scenario 2: Byte Truncation - High Bits Ignored
 * Given: Register contains 0xABCDEF42, base address 0x2000
 * When: SB instruction stores byte to memory
 * Then: Memory should contain 0x42, high bits should be ignored
 */
TEST_F(SBInstructionBDD, ByteTruncation_HighBitsIgnored)
{
    // Given: Register contains value with high bits set
    const uint32_t base_address   = 0x2000;
    const uint32_t rs_register    = 10; // $t2 (base address register)
    const uint32_t rt_register    = 11; // $t3 (source register)
    const int16_t  offset         = 0;
    const uint32_t register_value = 0xABCDEF42;
    const uint8_t  expected_byte  = 0x42; // Only low 8 bits matter

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getRegisterFile().write(rt_register, register_value);

    // When: Execute SB instruction
    auto sb_instruction = std::make_unique<mips::SBInstruction>(rt_register, rs_register, offset);
    sb_instruction->execute(*cpu);

    // Then: Memory should contain only the low byte
    uint8_t actual_byte = cpu->getMemory().readByte(base_address + offset);
    EXPECT_EQ(expected_byte, actual_byte);

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}

/**
 * Scenario 3: Positive Offset Store
 * Given: Base address 0x3000, offset +8
 * When: SB instruction stores byte with positive offset
 * Then: Byte should be stored at address 0x3008
 */
TEST_F(SBInstructionBDD, PositiveOffset_CalculateAddress)
{
    // Given: Positive offset for address calculation
    const uint32_t base_address      = 0x3000;
    const uint32_t rs_register       = 12; // $t4 (base address register)
    const uint32_t rt_register       = 13; // $t5 (source register)
    const int16_t  offset            = 8;
    const uint32_t effective_address = base_address + offset; // 0x3008
    const uint32_t register_value    = 0x000000AA;
    const uint8_t  expected_byte     = 0xAA;

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getRegisterFile().write(rt_register, register_value);

    // When: Execute SB instruction with positive offset
    auto sb_instruction = std::make_unique<mips::SBInstruction>(rt_register, rs_register, offset);
    sb_instruction->execute(*cpu);

    // Then: Byte should be stored at calculated address
    uint8_t actual_byte = cpu->getMemory().readByte(effective_address);
    EXPECT_EQ(expected_byte, actual_byte);

    // Verify byte was not stored at base address
    uint8_t base_byte = cpu->getMemory().readByte(base_address);
    EXPECT_EQ(0, base_byte); // Should remain zero

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}

/**
 * Scenario 4: Negative Offset Store
 * Given: Base address 0x4000, offset -4
 * When: SB instruction stores byte with negative offset
 * Then: Byte should be stored at address 0x3FFC
 */
TEST_F(SBInstructionBDD, NegativeOffset_CalculateAddress)
{
    // Given: Negative offset for address calculation
    const uint32_t base_address      = 0x4000;
    const uint32_t rs_register       = 14; // $t6 (base address register)
    const uint32_t rt_register       = 15; // $t7 (source register)
    const int16_t  offset            = -4;
    const uint32_t effective_address = base_address + offset; // 0x3FFC
    const uint32_t register_value    = 0xFFFFFF55;
    const uint8_t  expected_byte     = 0x55; // Low 8 bits

    cpu->getRegisterFile().write(rs_register, base_address);
    cpu->getRegisterFile().write(rt_register, register_value);

    // When: Execute SB instruction with negative offset
    auto sb_instruction = std::make_unique<mips::SBInstruction>(rt_register, rs_register, offset);
    sb_instruction->execute(*cpu);

    // Then: Byte should be stored at calculated address
    uint8_t actual_byte = cpu->getMemory().readByte(effective_address);
    EXPECT_EQ(expected_byte, actual_byte);

    // PC should increment
    EXPECT_EQ(1, cpu->getProgramCounter());
}
