/**
 * @file test_xori_instruction_integration.cpp
 * @brief XORI指令Integration測試 - 驗證解碼器和組譯器整合
 *
 * 按照BDD原則，完成XORI指令的Integration測試：
 * 1. Decoder Integration: 驗證InstructionDecoder正確解碼XORI指令
 * 2. Assembler Integration: 驗證Assembler正確解析"xori"語法
 */

#include "../src/Assembler.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include <gtest/gtest.h>
#include <memory>

/**
 * @brief XORI指令Integration測試fixture
 */
class XoriInstructionIntegrationTest : public ::testing::Test
{
  protected:
    std::unique_ptr<mips::InstructionDecoder> decoder;
    std::unique_ptr<mips::Assembler>          assembler;

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
};

// ============================================================================
// Integration測試：解碼器整合驗證
// ============================================================================

/**
 * @brief 測試InstructionDecoder能夠正確解碼XORI指令
 *
 * Given: InstructionDecoder已創建
 * When:  解碼XORI指令機器碼
 * Then:  應該成功解碼為XoriInstruction
 */
TEST_F(XoriInstructionIntegrationTest, XoriInstruction_DecoderIntegration_ShouldDecodeCorrectly)
{
    // Given: InstructionDecoder已創建

    // When: 解碼XORI指令機器碼
    // XORI $t1, $t0, 0x0F0F => I-type: op=0x0E, rs=8($t0), rt=9($t1), imm=0x0F0F
    uint32_t machineCode = (0x0E << 26) | (8 << 21) | (9 << 16) | 0x0F0F;

    auto instruction = decoder->decode(machineCode);

    // Then: 應該成功解碼為XoriInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼XORI指令";
    EXPECT_EQ(instruction->getName(), "xori") << "指令名稱應該是'xori'";
}

/**
 * @brief 測試InstructionDecoder解碼不同的XORI指令變體
 */
TEST_F(XoriInstructionIntegrationTest, XoriInstruction_DecoderVariants_ShouldDecodeCorrectly)
{
    // Test Case 1: XORI $zero, $zero, 0x0000
    uint32_t machineCode1 = (0x0E << 26) | (0 << 21) | (0 << 16) | 0x0000;
    auto     instruction1 = decoder->decode(machineCode1);
    ASSERT_NE(instruction1, nullptr);
    EXPECT_EQ(instruction1->getName(), "xori");

    // Test Case 2: XORI $t2, $t1, 0xFFFF
    uint32_t machineCode2 = (0x0E << 26) | (9 << 21) | (10 << 16) | 0xFFFF;
    auto     instruction2 = decoder->decode(machineCode2);
    ASSERT_NE(instruction2, nullptr);
    EXPECT_EQ(instruction2->getName(), "xori");
}

// ============================================================================
// Integration測試：組譯器整合驗證
// ============================================================================

/**
 * @brief 測試Assembler能夠正確解析XORI指令語法
 *
 * Given: Assembler已創建
 * When:  解析XORI指令語法
 * Then:  應該成功解析為一條XoriInstruction
 */
TEST_F(XoriInstructionIntegrationTest, XoriInstruction_AssemblerIntegration_ShouldParseCorrectly)
{
    // Given: Assembler已創建

    // When: 解析XORI指令語法
    std::string assembly     = "xori $t1, $t0, 0x0F0F";
    auto        instructions = assembler->assemble(assembly);

    // Then: 應該成功解析為一條指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "xori") << "指令名稱應該是'xori'";
}

/**
 * @brief 測試Assembler解析各種XORI指令語法變體
 */
TEST_F(XoriInstructionIntegrationTest, XoriInstruction_AssemblerVariants_ShouldParseCorrectly)
{
    // Test Case 1: 十六進制立即值
    auto instructions1 = assembler->assemble("xori $t1, $t0, 0xABCD");
    ASSERT_EQ(instructions1.size(), 1);
    EXPECT_EQ(instructions1[0]->getName(), "xori");

    // Test Case 2: 十進制立即值
    auto instructions2 = assembler->assemble("xori $t2, $t1, 65535");
    ASSERT_EQ(instructions2.size(), 1);
    EXPECT_EQ(instructions2[0]->getName(), "xori");

    // Test Case 3: 零立即值
    auto instructions3 = assembler->assemble("xori $t0, $zero, 0");
    ASSERT_EQ(instructions3.size(), 1);
    EXPECT_EQ(instructions3[0]->getName(), "xori");
}

/**
 * @brief 測試Assembler處理無效的XORI語法
 */
TEST_F(XoriInstructionIntegrationTest, XoriInstruction_AssemblerInvalidSyntax_ShouldReturnEmpty)
{
    // Test Case 1: 缺少參數
    auto instructions1 = assembler->assemble("xori $t1, $t0");
    EXPECT_TRUE(instructions1.empty()) << "缺少立即值的XORI指令應該解析失敗";

    // Test Case 2: 無效的暫存器名稱
    auto instructions2 = assembler->assemble("xori $invalid, $t0, 123");
    EXPECT_TRUE(instructions2.empty()) << "無效暫存器名稱的XORI指令應該解析失敗";
}

/**
 * @brief 測試XORI指令的位元切換功能
 */
TEST_F(XoriInstructionIntegrationTest, XoriInstruction_BitToggleIntegration_ShouldWorkCorrectly)
{
    // Given: 組譯器和解碼器已創建

    // When: 組譯位元切換XORI指令
    std::string assembly     = "xori $t1, $t0, 0xFFFF";
    auto        instructions = assembler->assemble(assembly);

    // Then: 應該成功解析
    ASSERT_EQ(instructions.size(), 1);
    EXPECT_EQ(instructions[0]->getName(), "xori");

    // Note: 實際位元切換執行測試在BDD場景中已經覆蓋
}

/**
 * @brief 測試XORI與其他立即值邏輯指令的差異性
 */
TEST_F(XoriInstructionIntegrationTest, XoriInstruction_DifferentiationIntegration_ShouldBeDistinct)
{
    // 確保XORI、ANDI、ORI被正確區分
    auto xoriInstructions = assembler->assemble("xori $t1, $t0, 0x1234");
    auto andiInstructions = assembler->assemble("andi $t1, $t0, 0x1234");
    auto oriInstructions  = assembler->assemble("ori $t1, $t0, 0x1234");

    ASSERT_EQ(xoriInstructions.size(), 1);
    ASSERT_EQ(andiInstructions.size(), 1);
    ASSERT_EQ(oriInstructions.size(), 1);

    EXPECT_EQ(xoriInstructions[0]->getName(), "xori");
    EXPECT_EQ(andiInstructions[0]->getName(), "andi");
    EXPECT_EQ(oriInstructions[0]->getName(), "ori");
}
