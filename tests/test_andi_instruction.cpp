/**
 * @file test_andi_instruction.cpp
 * @brief BDD Tests for ANDI (AND Immediate) instruction - Red Light Phase
 *
 * 按照BDD模式實作 ANDI 指令：
 * Phase A: 撰寫失敗測試 (紅燈)
 * Phase B: 實作最小程式碼使測試通過 (綠燈)
 * Phase C: 重構與清理
 */

#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

/**
 * @brief ANDI指令測試fixture
 */
class AndiInstructionTest : public ::testing::Test
{
  protected:
    std::unique_ptr<mips::Cpu> cpu;

    void SetUp() override
    {
        cpu = std::make_unique<mips::Cpu>();

        // 設定CPU為乾淨狀態 - 所有暫存器清零
        for (int i = 0; i < 32; ++i)
        {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }

    void TearDown() override
    {
        cpu.reset();
    }
};

// ============================================================================
// Phase A: BDD Scenarios - 期望紅燈 (測試失敗)
// ============================================================================

/**
 * @brief Scenario 1: ANDI指令基本AND操作
 * Given register $t0 contains 0xFFFF
 * When  I execute "andi $t1, $t0, 0x00FF"
 * Then  register $t1 should contain 0x00FF
 */
TEST_F(AndiInstructionTest, AndiInstruction_BasicOperation_ShouldPass)
{
    // Arrange: 設定初始狀態
    cpu->getRegisterFile().write(8, 0xFFFF);  // $t0 = 0xFFFF

    // Act: 執行ANDI指令 - 現在應該通過
    mips::AndiInstruction instr(9, 8, static_cast<int16_t>(0x00FF));  // andi $t1, $t0, 0x00FF
    instr.execute(*cpu);

    // Assert: 檢查預期結果
    uint32_t expectedResult = 0x00FF;  // 0xFFFF & 0x00FF = 0x00FF
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);

    // 確認源暫存器未變
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xFFFF);

    // 確認PC正確遞增
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * @brief Scenario 2: ANDI位元遮罩操作
 * Given register $t0 contains 0xAAAABBBB
 * When  I execute "andi $t1, $t0, 0xF0F0"
 * Then  register $t1 should contain 0x0000B0B0
 */
TEST_F(AndiInstructionTest, AndiInstruction_BitMask_ShouldPass)
{
    // Arrange
    cpu->getRegisterFile().write(8, 0xAAAABBBB);  // $t0 = 0xAAAABBBB

    // Act: 執行ANDI指令
    mips::AndiInstruction instr(9, 8, static_cast<int16_t>(0xF0F0));  // andi $t1, $t0, 0xF0F0
    instr.execute(*cpu);

    // Assert: 檢查位元遮罩結果
    // 0xAAAABBBB 的低16位是 0xBBBB
    // 0xBBBB & 0xF0F0 = 0xB0B0
    uint32_t expectedResult = 0x0000B0B0;
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);

    // 確認源暫存器未變
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xAAAABBBB);
}

/**
 * @brief Scenario 3: ANDI與零立即值
 * Given register $t0 contains 0x12345678
 * When  I execute "andi $t1, $t0, 0x0000"
 * Then  register $t1 should contain 0x00000000 (zero mask)
 */
TEST_F(AndiInstructionTest, AndiInstruction_ZeroImmediate_ShouldPass)
{
    // Arrange
    cpu->getRegisterFile().write(8, 0x12345678);  // $t0 = 0x12345678

    // Act: 執行ANDI指令
    mips::AndiInstruction instr(9, 8, 0x0000);  // andi $t1, $t0, 0x0000
    instr.execute(*cpu);

    // Assert: x AND 0 = 0 (zero mask)
    uint32_t expectedResult = 0x00000000;
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);

    // 確認源暫存器未變
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x12345678);
}

/**
 * @brief Scenario 4: ANDI與全1立即值
 * Given register $t0 contains 0x12345678
 * When  I execute "andi $t1, $t0, 0xFFFF"
 * Then  register $t1 should contain 0x00005678 (low 16 bits preserved)
 */
TEST_F(AndiInstructionTest, AndiInstruction_AllOnesImmediate_ShouldPass)
{
    // Arrange
    cpu->getRegisterFile().write(8, 0x12345678);  // $t0 = 0x12345678

    // Act: 執行ANDI指令
    mips::AndiInstruction instr(9, 8, static_cast<int16_t>(0xFFFF));  // andi $t1, $t0, 0xFFFF
    instr.execute(*cpu);

    // Assert: 0x12345678 AND 0x0000FFFF = 0x00005678 (保留低16位)
    uint32_t expectedResult = 0x00005678;
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);
}

/**
 * @brief Scenario 5: ANDI與$zero暫存器
 * Given register $zero contains 0 (always)
 * When  I execute "andi $t1, $zero, 0xFFFF"
 * Then  register $t1 should contain 0x00000000
 */
TEST_F(AndiInstructionTest, AndiInstruction_WithZeroRegister_ShouldPass)
{
    // Arrange: $zero應該總是0
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);

    // Act: 執行ANDI指令
    mips::AndiInstruction instr(9, 0, static_cast<int16_t>(0xFFFF));  // andi $t1, $zero, 0xFFFF
    instr.execute(*cpu);

    // Assert: 0 AND 0xFFFF = 0x00000000
    uint32_t expectedResult = 0x00000000;
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);

    // 確認$zero仍然是0
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);
}

/**
 * @brief Scenario 6: ANDI低位元提取測試
 * Given register $t0 contains 0xDEADBEEF
 * When  I execute "andi $t1, $t0, 0x00FF"
 * Then  register $t1 should contain 0x000000EF (extract lowest byte)
 */
TEST_F(AndiInstructionTest, AndiInstruction_ByteExtraction_ShouldPass)
{
    // Arrange
    cpu->getRegisterFile().write(8, 0xDEADBEEF);  // $t0 = 0xDEADBEEF

    // Act: 執行ANDI指令
    mips::AndiInstruction instr(9, 8, static_cast<int16_t>(0x00FF));  // andi $t1, $t0, 0x00FF
    instr.execute(*cpu);

    // Assert: 提取最低位元組
    uint32_t expectedResult = 0x000000EF;  // 0xDEADBEEF & 0x000000FF = 0x000000EF
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);
}

// ============================================================================
// Phase A: 測試框架驗證（這些應該通過，確認測試環境正常）
// ============================================================================

/**
 * @brief 框架測試：確認CPU初始化正常
 */
TEST_F(AndiInstructionTest, Framework_CpuInitialization_ShouldPass)
{
    // 驗證CPU初始狀態
    EXPECT_EQ(cpu->getProgramCounter(), 0);
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);  // $zero
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0);  // $t0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);  // $t1
}

/**
 * @brief 框架測試：確認暫存器讀寫功能正常
 */
TEST_F(AndiInstructionTest, Framework_RegisterReadWrite_ShouldPass)
{
    // 測試暫存器寫入/讀取
    cpu->getRegisterFile().write(8, 0xDEADBEEF);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xDEADBEEF);

    // 測試$zero暫存器的不可寫特性
    cpu->getRegisterFile().write(0, 0xFFFFFFFF);
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);  // 應該仍然是0
}

// ============================================================================
// 注意：移除主函式避免與其他測試衝突
// main函式由GoogleTest框架在unit_tests.exe中統一提供
// ============================================================================
