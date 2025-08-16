/**
 * @file test_div_instruction_bdd_minimal.cpp
 * @brief BDD tests for DIV instruction (Phase 8.5: Red Light)
 */

#include <gtest/gtest.h>
#include <cstdint>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Memory.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include <memory>

using namespace mips;

class DIVInstructionBDD : public ::testing::Test {
protected:
    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<Assembler> assembler;
    std::unique_ptr<InstructionDecoder> decoder;

    void SetUp() override {
        cpu = std::make_unique<Cpu>();
        assembler = std::make_unique<Assembler>();
        decoder = std::make_unique<InstructionDecoder>();
        
        // Clear all registers
        for (int i = 0; i < 32; ++i) {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
        cpu->getMemory().reset();
    }

    void TearDown() override {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }
};

/**
 * @brief BDD Scenario: Basic signed division test
 * 
 * Given: Two positive numbers in registers
 * When: DIV instruction is executed
 * Then: LO should contain quotient, HI should contain remainder
 */
TEST_F(DIVInstructionBDD, BasicSignedDivision) {
    // Given: Two positive numbers
    cpu->getRegisterFile().write(8, 100);     // $t0 = 100
    cpu->getRegisterFile().write(9, 7);       // $t1 = 7
    
    // When: DIV $t0, $t1
    uint32_t div_instruction = 0x0109481A;    // DIV opcode, rs=$t0(8), rt=$t1(9), function=0x1A
    auto instruction = decoder->decode(div_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);
    
    // Then: LO should contain quotient (100 / 7 = 14), HI should contain remainder (100 % 7 = 2)
    int32_t expected_quotient = 100 / 7;   // 14
    int32_t expected_remainder = 100 % 7;  // 2
    
    EXPECT_EQ(cpu->getRegisterFile().readLO(), static_cast<uint32_t>(expected_quotient));
    EXPECT_EQ(cpu->getRegisterFile().readHI(), static_cast<uint32_t>(expected_remainder));
}

/**
 * @brief BDD Scenario: Divide by zero handling test
 * 
 * Given: Dividend is non-zero, divisor is zero
 * When: DIV instruction is executed
 * Then: Both HI and LO should be set to zero (undefined behavior, but safe)
 */
TEST_F(DIVInstructionBDD, DivideByZeroHandling) {
    // Given: Non-zero dividend, zero divisor
    cpu->getRegisterFile().write(8, 100);     // $t0 = 100
    cpu->getRegisterFile().write(9, 0);       // $t1 = 0
    
    // When: DIV $t0, $t1
    uint32_t div_instruction = 0x0109481A;    // DIV opcode, rs=$t0(8), rt=$t1(9), function=0x1A
    auto instruction = decoder->decode(div_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);
    
    // Then: Both HI and LO should be zero (safe handling of divide by zero)
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0);
}

/**
 * @brief BDD Scenario: Negative number division test
 * 
 * Given: One positive and one negative number
 * When: DIV instruction is executed
 * Then: Result should be correctly calculated with proper signs
 */
TEST_F(DIVInstructionBDD, NegativeDivision) {
    // Given: Positive dividend, negative divisor
    cpu->getRegisterFile().write(8, 100);             // $t0 = 100
    cpu->getRegisterFile().write(9, 0xFFFFFFF9);      // $t1 = -7 (signed)
    
    // When: DIV $t0, $t1
    uint32_t div_instruction = 0x0109481A;    // DIV opcode, rs=$t0(8), rt=$t1(9), function=0x1A
    auto instruction = decoder->decode(div_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);
    
    // Then: LO should contain quotient (100 / -7 = -14), HI should contain remainder (100 % -7 = 2)
    int32_t expected_quotient = 100 / (-7);   // -14
    int32_t expected_remainder = 100 % (-7);  // 2
    
    EXPECT_EQ(cpu->getRegisterFile().readLO(), static_cast<uint32_t>(expected_quotient));
    EXPECT_EQ(cpu->getRegisterFile().readHI(), static_cast<uint32_t>(expected_remainder));
}

/**
 * @brief BDD Scenario: Remainder verification test
 * 
 * Given: Numbers that don't divide evenly
 * When: DIV instruction is executed
 * Then: Quotient and remainder should satisfy dividend = quotient * divisor + remainder
 */
TEST_F(DIVInstructionBDD, RemainderVerification) {
    // Given: Numbers that don't divide evenly
    cpu->getRegisterFile().write(8, 123);     // $t0 = 123
    cpu->getRegisterFile().write(9, 10);      // $t1 = 10
    
    // When: DIV $t0, $t1
    uint32_t div_instruction = 0x0109481A;    // DIV opcode, rs=$t0(8), rt=$t1(9), function=0x1A
    auto instruction = decoder->decode(div_instruction);
    ASSERT_NE(instruction, nullptr);
    instruction->execute(*cpu);
    
    // Then: Verify the division identity: dividend = quotient * divisor + remainder
    uint32_t quotient = cpu->getRegisterFile().readLO();
    uint32_t remainder = cpu->getRegisterFile().readHI();
    
    // For signed division: 123 / 10 = 12, 123 % 10 = 3
    EXPECT_EQ(quotient, 12);
    EXPECT_EQ(remainder, 3);
    
    // Verify: 123 = 12 * 10 + 3
    EXPECT_EQ(123, quotient * 10 + remainder);
}
