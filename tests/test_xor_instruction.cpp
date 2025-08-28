/**
 * @file test_xor_instruction.cpp
 * @brief XOR instruction integration tests
 *
 * XOR instruction integration tests implemented following BDD methodology, ensuring:
 * 1. InstructionDecoder correctly decodes XOR instructions
 * 2. Assembler correctly parses "xor" syntax
 */

#include "../src/Assembler.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include <gtest/gtest.h>

class XorInstructionIntegrationTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        decoder   = std::make_unique<mips::InstructionDecoder>();
        assembler = std::make_unique<mips::Assembler>();
    }

    void TearDown() override
    {
        decoder.reset();
        assembler.reset();
    }

    std::unique_ptr<mips::InstructionDecoder> decoder;
    std::unique_ptr<mips::Assembler>          assembler;
};

/**
 * @brief BDD Integration Test 1: Decoder Integration
 *
 * Scenario: InstructionDecoder 支援 XOR 指令解碼
 *   Given 我有一個 InstructionDecoder
 *   When 我解碼 XOR 指令的機器碼 (function code 0x26)
 *   Then 解碼器應該返回 XorInstruction 物件
 *   And 指令名稱應該是 "xor"
 */
TEST_F(XorInstructionIntegrationTest, XorInstruction_DecoderIntegration_ShouldDecodeCorrectly)
{
    // Given: InstructionDecoder 已在 SetUp() 中創建

    // When: 解碼 XOR 指令機器碼
    // XOR $t2, $t0, $t1 => R-type: op=0, rs=8($t0), rt=9($t1), rd=10($t2), shamt=0, funct=0x26
    uint32_t machineCode = (0x00 << 26) | (8 << 21) | (9 << 16) | (10 << 11) | (0 << 6) | 0x26;

    auto instruction = decoder->decode(machineCode);

    // Then: 應該成功解碼為 XorInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼 XOR 指令";
    EXPECT_EQ(instruction->getName(), "xor") << "指令名稱應該是 'xor'";
}

/**
 * @brief BDD Integration Test 2: Assembler Integration
 *
 * Scenario: Assembler 支援 "xor" 語法解析
 *   Given 我有一個 Assembler
 *   When 我解析 "xor $t2, $t0, $t1" 指令
 *   Then 組譯器應該成功解析指令
 *   And 解析出的指令名稱應該是 "xor"
 */
TEST_F(XorInstructionIntegrationTest, XorInstruction_AssemblerIntegration_ShouldParseCorrectly)
{
    // Given: Assembler 已在 SetUp() 中創建

    // When: 解析 XOR 指令語法
    std::string assembly     = "xor $t2, $t0, $t1";
    auto        instructions = assembler->assemble(assembly);

    // Then: 應該成功解析為一條指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "xor") << "指令名稱應該是 'xor'";
}
