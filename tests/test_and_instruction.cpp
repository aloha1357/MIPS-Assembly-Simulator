/**
 * @file test_and_instruction.cpp
 * @brief AND instruction integration tests
 * 
 * 按照BDD方法論實作的AND指令整合測試，確保：
 * 1. InstructionDecoder 正確解碼 AND 指令
 * 2. Assembler 正確解析 "and" 語法
 */

#include <gtest/gtest.h>
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include "../src/Instruction.h"

class AndInstructionIntegrationTest : public ::testing::Test {
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
 * Scenario: InstructionDecoder 支援 AND 指令解碼
 *   Given 我有一個 InstructionDecoder
 *   When 我解碼 AND 指令的機器碼 (function code 0x24)
 *   Then 解碼器應該返回 AndInstruction 物件
 *   And 指令名稱應該是 "and"
 */
TEST_F(AndInstructionIntegrationTest, AndInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // Given: InstructionDecoder 已在 SetUp() 中創建
    
    // When: 解碼 AND 指令機器碼
    // AND $t2, $t0, $t1 => R-type: op=0, rs=8($t0), rt=9($t1), rd=10($t2), shamt=0, funct=0x24
    uint32_t machineCode = (0x00 << 26) | (8 << 21) | (9 << 16) | (10 << 11) | (0 << 6) | 0x24;
    
    auto instruction = decoder->decode(machineCode);
    
    // Then: 應該成功解碼為 AndInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼 AND 指令";
    EXPECT_EQ(instruction->getName(), "and") << "指令名稱應該是 'and'";
}

/**
 * @brief BDD Integration Test 2: Assembler Integration
 * 
 * Scenario: Assembler 支援 "and" 語法解析
 *   Given 我有一個 Assembler
 *   When 我解析 "and $t2, $t0, $t1" 指令
 *   Then 組譯器應該成功解析指令
 *   And 解析出的指令名稱應該是 "and"
 */
TEST_F(AndInstructionIntegrationTest, AndInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // Given: Assembler 已在 SetUp() 中創建
    
    // When: 解析 AND 指令語法
    std::string assembly = "and $t2, $t0, $t1";
    auto instructions = assembler->assemble(assembly);
    
    // Then: 應該成功解析為一條指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "and") << "指令名稱應該是 'and'";
}
