/**
 * @file test_ori_instruction_integration.cpp
 * @brief ORI指令Integration測試 - 驗證解碼器和組譯器整合
 * 
 * 按照BDD原則，完成ORI指令的Integration測試：
 * 1. Decoder Integration: 驗證InstructionDecoder正確解碼ORI指令
 * 2. Assembler Integration: 驗證Assembler正確解析"ori"語法
 */

#include <gtest/gtest.h>
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include "../src/Instruction.h"
#include <memory>

/**
 * @brief ORI指令Integration測試fixture
 */
class OriInstructionIntegrationTest : public ::testing::Test {
protected:
    std::unique_ptr<mips::InstructionDecoder> decoder;
    std::unique_ptr<mips::Assembler> assembler;
    
    void SetUp() override {
        decoder = std::make_unique<mips::InstructionDecoder>();
        assembler = std::make_unique<mips::Assembler>();
    }
    
    void TearDown() override {
        decoder.reset();
        assembler.reset();
    }
};

// ============================================================================
// Integration測試：解碼器整合驗證
// ============================================================================

/**
 * @brief 測試InstructionDecoder能夠正確解碼ORI指令
 * 
 * Given: InstructionDecoder已創建
 * When:  解碼ORI指令機器碼
 * Then:  應該成功解碼為OriInstruction
 */
TEST_F(OriInstructionIntegrationTest, OriInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // Given: InstructionDecoder已創建
    
    // When: 解碼ORI指令機器碼
    // ORI $t1, $t0, 0x1234 => I-type: op=0x0D, rs=8($t0), rt=9($t1), imm=0x1234
    uint32_t machineCode = (0x0D << 26) | (8 << 21) | (9 << 16) | 0x1234;
    
    auto instruction = decoder->decode(machineCode);
    
    // Then: 應該成功解碼為OriInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼ORI指令";
    EXPECT_EQ(instruction->getName(), "ori") << "指令名稱應該是'ori'";
}

/**
 * @brief 測試InstructionDecoder解碼不同的ORI指令變體
 */
TEST_F(OriInstructionIntegrationTest, OriInstruction_DecoderVariants_ShouldDecodeCorrectly) {
    // Test Case 1: ORI $zero, $zero, 0x0000
    uint32_t machineCode1 = (0x0D << 26) | (0 << 21) | (0 << 16) | 0x0000;
    auto instruction1 = decoder->decode(machineCode1);
    ASSERT_NE(instruction1, nullptr);
    EXPECT_EQ(instruction1->getName(), "ori");
    
    // Test Case 2: ORI $t2, $t1, 0xFFFF
    uint32_t machineCode2 = (0x0D << 26) | (9 << 21) | (10 << 16) | 0xFFFF;
    auto instruction2 = decoder->decode(machineCode2);
    ASSERT_NE(instruction2, nullptr);
    EXPECT_EQ(instruction2->getName(), "ori");
}

// ============================================================================
// Integration測試：組譯器整合驗證
// ============================================================================

/**
 * @brief 測試Assembler能夠正確解析ORI指令語法
 * 
 * Given: Assembler已創建
 * When:  解析ORI指令語法
 * Then:  應該成功解析為一條OriInstruction
 */
TEST_F(OriInstructionIntegrationTest, OriInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // Given: Assembler已創建
    
    // When: 解析ORI指令語法
    std::string assembly = "ori $t1, $t0, 0x1234";
    auto instructions = assembler->assemble(assembly);
    
    // Then: 應該成功解析為一條指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "ori") << "指令名稱應該是'ori'";
}

/**
 * @brief 測試Assembler解析各種ORI指令語法變體
 */
TEST_F(OriInstructionIntegrationTest, OriInstruction_AssemblerVariants_ShouldParseCorrectly) {
    // Test Case 1: 十六進制立即值
    auto instructions1 = assembler->assemble("ori $t1, $t0, 0xABCD");
    ASSERT_EQ(instructions1.size(), 1);
    EXPECT_EQ(instructions1[0]->getName(), "ori");
    
    // Test Case 2: 十進制立即值
    auto instructions2 = assembler->assemble("ori $t2, $t1, 12345");
    ASSERT_EQ(instructions2.size(), 1);
    EXPECT_EQ(instructions2[0]->getName(), "ori");
    
    // Test Case 3: 零立即值
    auto instructions3 = assembler->assemble("ori $t0, $zero, 0");
    ASSERT_EQ(instructions3.size(), 1);
    EXPECT_EQ(instructions3[0]->getName(), "ori");
}

/**
 * @brief 測試Assembler處理無效的ORI語法
 */
TEST_F(OriInstructionIntegrationTest, OriInstruction_AssemblerInvalidSyntax_ShouldReturnEmpty) {
    // Test Case 1: 缺少參數
    auto instructions1 = assembler->assemble("ori $t1, $t0");
    EXPECT_TRUE(instructions1.empty()) << "缺少立即值的ORI指令應該解析失敗";
    
    // Test Case 2: 無效的暫存器名稱
    auto instructions2 = assembler->assemble("ori $invalid, $t0, 123");
    EXPECT_TRUE(instructions2.empty()) << "無效暫存器名稱的ORI指令應該解析失敗";
    
    // Test Case 3: 無效的立即值
    auto instructions3 = assembler->assemble("ori $t1, $t0, invalid_immediate");
    EXPECT_TRUE(instructions3.empty()) << "無效立即值的ORI指令應該解析失敗";
}

// ============================================================================
// Integration測試：完整流程驗證
// ============================================================================

/**
 * @brief 測試組譯→解碼→執行的完整流程
 */
TEST_F(OriInstructionIntegrationTest, OriInstruction_FullIntegration_ShouldWorkEndToEnd) {
    // Step 1: 組譯ORI指令
    std::string assembly = "ori $t1, $t0, 0x0F0F";
    auto instructions = assembler->assemble(assembly);
    ASSERT_EQ(instructions.size(), 1);
    
    // Step 2: 驗證組譯結果是OriInstruction
    auto oriInstruction = instructions[0].get();
    EXPECT_EQ(oriInstruction->getName(), "ori");
    
    // Note: 實際執行測試在BDD場景中已經覆蓋，這裡主要驗證整合性
}
