/**
 * @file test_andi_instruction_integration.cpp
 * @brief ANDI指令Integration測試 - 驗證解碼器和組譯器整合
 * 
 * 按照BDD原則，完成ANDI指令的Integration測試：
 * 1. Decoder Integration: 驗證InstructionDecoder正確解碼ANDI指令
 * 2. Assembler Integration: 驗證Assembler正確解析"andi"語法
 */

#include <gtest/gtest.h>
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include "../src/Instruction.h"
#include <memory>

/**
 * @brief ANDI指令Integration測試fixture
 */
class AndiInstructionIntegrationTest : public ::testing::Test {
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
 * @brief 測試InstructionDecoder能夠正確解碼ANDI指令
 * 
 * Given: InstructionDecoder已創建
 * When:  解碼ANDI指令機器碼
 * Then:  應該成功解碼為AndiInstruction
 */
TEST_F(AndiInstructionIntegrationTest, AndiInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // Given: InstructionDecoder已創建
    
    // When: 解碼ANDI指令機器碼
    // ANDI $t1, $t0, 0x0F0F => I-type: op=0x0C, rs=8($t0), rt=9($t1), imm=0x0F0F
    uint32_t machineCode = (0x0C << 26) | (8 << 21) | (9 << 16) | 0x0F0F;
    
    auto instruction = decoder->decode(machineCode);
    
    // Then: 應該成功解碼為AndiInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼ANDI指令";
    EXPECT_EQ(instruction->getName(), "andi") << "指令名稱應該是'andi'";
}

/**
 * @brief 測試InstructionDecoder解碼不同的ANDI指令變體
 */
TEST_F(AndiInstructionIntegrationTest, AndiInstruction_DecoderVariants_ShouldDecodeCorrectly) {
    // Test Case 1: ANDI $zero, $zero, 0x0000
    uint32_t machineCode1 = (0x0C << 26) | (0 << 21) | (0 << 16) | 0x0000;
    auto instruction1 = decoder->decode(machineCode1);
    ASSERT_NE(instruction1, nullptr);
    EXPECT_EQ(instruction1->getName(), "andi");
    
    // Test Case 2: ANDI $t2, $t1, 0xFFFF
    uint32_t machineCode2 = (0x0C << 26) | (9 << 21) | (10 << 16) | 0xFFFF;
    auto instruction2 = decoder->decode(machineCode2);
    ASSERT_NE(instruction2, nullptr);
    EXPECT_EQ(instruction2->getName(), "andi");
}

// ============================================================================
// Integration測試：組譯器整合驗證
// ============================================================================

/**
 * @brief 測試Assembler能夠正確解析ANDI指令語法
 * 
 * Given: Assembler已創建
 * When:  解析ANDI指令語法
 * Then:  應該成功解析為一條AndiInstruction
 */
TEST_F(AndiInstructionIntegrationTest, AndiInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // Given: Assembler已創建
    
    // When: 解析ANDI指令語法
    std::string assembly = "andi $t1, $t0, 0x0F0F";
    auto instructions = assembler->assemble(assembly);
    
    // Then: 應該成功解析為一條指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "andi") << "指令名稱應該是'andi'";
}

/**
 * @brief 測試Assembler解析各種ANDI指令語法變體
 */
TEST_F(AndiInstructionIntegrationTest, AndiInstruction_AssemblerVariants_ShouldParseCorrectly) {
    // Test Case 1: 十六進制立即值
    auto instructions1 = assembler->assemble("andi $t1, $t0, 0xFF00");
    ASSERT_EQ(instructions1.size(), 1);
    EXPECT_EQ(instructions1[0]->getName(), "andi");
    
    // Test Case 2: 十進制立即值
    auto instructions2 = assembler->assemble("andi $t2, $t1, 255");
    ASSERT_EQ(instructions2.size(), 1);
    EXPECT_EQ(instructions2[0]->getName(), "andi");
    
    // Test Case 3: 零立即值
    auto instructions3 = assembler->assemble("andi $t0, $zero, 0");
    ASSERT_EQ(instructions3.size(), 1);
    EXPECT_EQ(instructions3[0]->getName(), "andi");
}

/**
 * @brief 測試Assembler處理無效的ANDI語法
 */
TEST_F(AndiInstructionIntegrationTest, AndiInstruction_AssemblerInvalidSyntax_ShouldReturnEmpty) {
    // Test Case 1: 缺少參數
    auto instructions1 = assembler->assemble("andi $t1, $t0");
    EXPECT_TRUE(instructions1.empty()) << "缺少立即值的ANDI指令應該解析失敗";
    
    // Test Case 2: 無效的暫存器名稱
    auto instructions2 = assembler->assemble("andi $invalid, $t0, 123");
    EXPECT_TRUE(instructions2.empty()) << "無效暫存器名稱的ANDI指令應該解析失敗";
}

/**
 * @brief 測試ANDI指令的位元遮罩功能
 */
TEST_F(AndiInstructionIntegrationTest, AndiInstruction_BitMaskIntegration_ShouldWorkCorrectly) {
    // Given: 組譯器和解碼器已創建
    
    // When: 組譯位元遮罩ANDI指令
    std::string assembly = "andi $t1, $t0, 0xF000";
    auto instructions = assembler->assemble(assembly);
    
    // Then: 應該成功解析
    ASSERT_EQ(instructions.size(), 1);
    EXPECT_EQ(instructions[0]->getName(), "andi");
    
    // Note: 實際位元遮罩執行測試在BDD場景中已經覆蓋
}
