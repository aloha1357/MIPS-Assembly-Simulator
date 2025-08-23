/**
 * @file test_multu_instruction_bdd_minimal.cpp
 * @brief BDD tests for MULTU instruction (Phase 8.4: Red Light)
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

using namespace mips;

class MULTUInstructionBDD : public ::testing::Test
{
  protected:
    std::unique_ptr<Cpu>                cpu;
    std::unique_ptr<Assembler>          assembler;
    std::unique_ptr<InstructionDecoder> decoder;

    void SetUp() override
    {
        cpu       = std::make_unique<Cpu>();
        assembler = std::make_unique<Assembler>();
        decoder   = std::make_unique<InstructionDecoder>();

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
 * @brief BDD Scenario: Basic unsigned multiplication test
 *
 * Given: Two positive numbers in registers
 * When: MULTU instruction is executed
 * Then: HI:LO should contain the 64-bit unsigned multiplication result
 */
TEST_F(MULTUInstructionBDD, BasicUnsignedMultiplication)
{
    // Given: Two positive numbers
    cpu->getRegisterFile().write(8, 123);  // $t0 = 123
    cpu->getRegisterFile().write(9, 456);  // $t1 = 456

    // When: MULTU $t0, $t1
    uint32_t multu_instruction = 0x01094819;  // MULTU opcode, rs=$t0(8), rt=$t1(9), function=0x19
    auto     instruction       = decoder->decode(multu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should contain 123 * 456 = 56088
    uint64_t expected_result = 123ULL * 456ULL;
    uint32_t expected_hi     = static_cast<uint32_t>(expected_result >> 32);
    uint32_t expected_lo     = static_cast<uint32_t>(expected_result & 0xFFFFFFFF);

    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_hi);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_lo);
}

/**
 * @brief BDD Scenario: Large unsigned multiplication test
 *
 * Given: Two large unsigned numbers
 * When: MULTU instruction is executed
 * Then: Result should be correctly calculated as unsigned values
 */
TEST_F(MULTUInstructionBDD, LargeUnsignedMultiplication)
{
    // Given: Two large unsigned numbers
    cpu->getRegisterFile().write(8, 0xFFFFFFFF);  // $t0 = max uint32 (4294967295)
    cpu->getRegisterFile().write(9, 2);           // $t1 = 2

    // When: MULTU $t0, $t1
    uint32_t multu_instruction = 0x01094819;  // MULTU opcode, rs=$t0(8), rt=$t1(9), function=0x19
    auto     instruction       = decoder->decode(multu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should contain 0xFFFFFFFF * 2 = 0x1FFFFFFFE
    uint64_t expected_result = 0xFFFFFFFFULL * 2ULL;
    uint32_t expected_hi     = static_cast<uint32_t>(expected_result >> 32);
    uint32_t expected_lo     = static_cast<uint32_t>(expected_result & 0xFFFFFFFF);

    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_hi);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_lo);
}

/**
 * @brief BDD Scenario: Maximum value multiplication test
 *
 * Given: Two maximum unsigned values
 * When: MULTU instruction is executed
 * Then: Result should overflow correctly into HI register
 */
TEST_F(MULTUInstructionBDD, MaxValueMultiplication)
{
    // Given: Two max unsigned values
    cpu->getRegisterFile().write(8, 0xFFFFFFFF);  // $t0 = max uint32
    cpu->getRegisterFile().write(9, 0xFFFFFFFF);  // $t1 = max uint32

    // When: MULTU $t0, $t1
    uint32_t multu_instruction = 0x01094819;  // MULTU opcode, rs=$t0(8), rt=$t1(9), function=0x19
    auto     instruction       = decoder->decode(multu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should contain 0xFFFFFFFF * 0xFFFFFFFF = 0xFFFFFFFE00000001
    uint64_t expected_result = 0xFFFFFFFFULL * 0xFFFFFFFFULL;
    uint32_t expected_hi     = static_cast<uint32_t>(expected_result >> 32);
    uint32_t expected_lo     = static_cast<uint32_t>(expected_result & 0xFFFFFFFF);

    EXPECT_EQ(cpu->getRegisterFile().readHI(), expected_hi);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), expected_lo);
}

/**
 * @brief BDD Scenario: Zero multiplication test
 *
 * Given: One operand is zero
 * When: MULTU instruction is executed
 * Then: Result should be zero in both HI and LO registers
 */
TEST_F(MULTUInstructionBDD, ZeroMultiplication)
{
    // Given: One operand is zero
    cpu->getRegisterFile().write(8, 0);           // $t0 = 0
    cpu->getRegisterFile().write(9, 0xFFFFFFFF);  // $t1 = max uint32

    // When: MULTU $t0, $t1
    uint32_t multu_instruction = 0x01094819;  // MULTU opcode, rs=$t0(8), rt=$t1(9), function=0x19
    auto     instruction       = decoder->decode(multu_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);

    // Then: HI:LO should both be zero
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0);
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0);
}
