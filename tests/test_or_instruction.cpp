/**
 * @file test_or_instruction.cpp
 * @brief OR instruction integration tests
 * 
 * 按照BDD方法論實作的OR指令整合測試，確保：
 * 1. InstructionDecoder 正確解碼 OR 指令
 * 2. Assembler 正確解析 "or" 語法
 */

#include <gtest/gtest.h>
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include "../src/Instruction.h"

class OrInstructionIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        decoder = std::make_unique<mips::InstructionDecoder>();
        assembler = std::make_unique<mips::Assembler>();
    }

    void TearDown() override {
        decoder.reset();
        assembler.reset();
    }

    std::unique_ptr<mips::InstructionDecoder> decoder;
    std::unique_ptr<mips::Assembler> assembler;
};

/**
 * @brief BDD Integration Test 1: Decoder Integration
 * 
 * Scenario: InstructionDecoder 支援 OR 指令解碼
 *   Given 我有一個 InstructionDecoder
 *   When 我解碼 OR 指令的機器碼 (function code 0x25)
 *   Then 解碼器應該返回 OrInstruction 物件
 *   And 指令名稱應該是 "or"
 */
TEST_F(OrInstructionIntegrationTest, OrInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // Given: InstructionDecoder 已在 SetUp() 中創建
    
    // When: 解碼 OR 指令機器碼
    // OR $t2, $t0, $t1 => R-type: op=0, rs=8($t0), rt=9($t1), rd=10($t2), shamt=0, funct=0x25
    uint32_t machineCode = (0x00 << 26) | (8 << 21) | (9 << 16) | (10 << 11) | (0 << 6) | 0x25;
    
    auto instruction = decoder->decode(machineCode);
    
    // Then: 應該成功解碼為 OrInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼 OR 指令";
    EXPECT_EQ(instruction->getName(), "or") << "指令名稱應該是 'or'";
}

/**
 * @brief BDD Integration Test 2: Assembler Integration
 * 
 * Scenario: Assembler 支援 "or" 語法解析
 *   Given 我有一個 Assembler
 *   When 我解析 "or $t2, $t0, $t1" 指令
 *   Then 組譯器應該成功解析指令
 *   And 解析出的指令名稱應該是 "or"
 */
TEST_F(OrInstructionIntegrationTest, OrInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // Given: Assembler 已在 SetUp() 中創建
    
    // When: 解析 OR 指令語法
    std::string assembly = "or $t2, $t0, $t1";
    auto instructions = assembler->assemble(assembly);
    
    // Then: 應該成功解析為一條指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "or") << "指令名稱應該是 'or'";
}
