/**
 * @file test_ori_instruction.cpp
 * @brief BDD Tests for ORI (OR Immediate) instruction - Red Light Phase
 * 
 * 按照BDD模式實作 ORI 指令：
 * Phase A: 撰寫失敗測試 (紅燈)
 * Phase B: 實作最小程式碼使測試通過 (綠燈)
 * Phase C: 重構與清理
 */

#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Instruction.h"
#include <memory>

/**
 * @brief ORI指令測試fixture
 */
class OriInstructionTest : public ::testing::Test {
protected:
    std::unique_ptr<mips::Cpu> cpu;
    
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        
        // 設定CPU為乾淨狀態 - 所有暫存器清零
        for (int i = 0; i < 32; ++i) {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }
    
    void TearDown() override {
        cpu.reset();
    }
};

// ============================================================================
// Phase A: BDD Scenarios - 期望紅燈 (測試失敗)
// ============================================================================

/**
 * @brief Scenario 1: ORI指令基本OR操作
 * Given register $t0 contains 0xFF00
 * When  I execute "ori $t1, $t0, 0x00FF" 
 * Then  register $t1 should contain 0xFFFF
 */
TEST_F(OriInstructionTest, OriInstruction_BasicOperation_ShouldPass) {
    // Arrange: 設定初始狀態
    cpu->getRegisterFile().write(8, 0xFF00);  // $t0 = 0xFF00
    
    // Act: 執行ORI指令 - 現在應該通過
    mips::OriInstruction instr(9, 8, 0x00FF);  // ori $t1, $t0, 0x00FF
    instr.execute(*cpu);
    
    // Assert: 檢查預期結果
    uint32_t expectedResult = 0xFFFF;  // 0xFF00 | 0x00FF = 0xFFFF
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);
    
    // 確認源暫存器未變
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xFF00);
    
    // 確認PC正確遞增
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * @brief Scenario 2: ORI與零立即值
 * Given register $t0 contains 0x12345678
 * When  I execute "ori $t1, $t0, 0x0000"
 * Then  register $t1 should contain 0x12345678 (identity operation)
 */
TEST_F(OriInstructionTest, OriInstruction_ZeroImmediate_ShouldPass) {
    // Arrange
    cpu->getRegisterFile().write(8, 0x12345678);  // $t0 = 0x12345678
    
    // Act: 執行ORI指令
    mips::OriInstruction instr(9, 8, 0x0000);  // ori $t1, $t0, 0x0000
    instr.execute(*cpu);
    
    // Assert: x OR 0 = x (identity)
    uint32_t expectedResult = 0x12345678;
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);
    
    // 確認源暫存器未變
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x12345678);
}

/**
 * @brief Scenario 3: ORI與全1立即值
 * Given register $t0 contains 0x00000000
 * When  I execute "ori $t1, $t0, 0xFFFF"
 * Then  register $t1 should contain 0x0000FFFF
 */
TEST_F(OriInstructionTest, OriInstruction_AllOnesImmediate_ShouldPass) {
    // Arrange
    cpu->getRegisterFile().write(8, 0x00000000);  // $t0 = 0x00000000
    
    // Act: 執行ORI指令
    mips::OriInstruction instr(9, 8, static_cast<int16_t>(0xFFFF));  // ori $t1, $t0, 0xFFFF
    instr.execute(*cpu);
    
    // Assert: 0 OR 0xFFFF = 0x0000FFFF (16位立即值零擴展)
    uint32_t expectedResult = 0x0000FFFF;
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);
}

/**
 * @brief Scenario 4: ORI位元遮罩操作
 * Given register $t0 contains 0xF0F0F0F0
 * When  I execute "ori $t1, $t0, 0x0F0F"
 * Then  register $t1 should contain 0xF0F0FFFF
 */
TEST_F(OriInstructionTest, OriInstruction_BitMask_ShouldPass) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xF0F0F0F0);  // $t0 = 0xF0F0F0F0
    
    // Act: 執行ORI指令
    mips::OriInstruction instr(9, 8, static_cast<int16_t>(0x0F0F));  // ori $t1, $t0, 0x0F0F
    instr.execute(*cpu);
    
    // Assert: 檢查位元遮罩結果
    uint32_t expectedResult = 0xF0F0FFFF;  // 0xF0F0F0F0 | 0x00000F0F = 0xF0F0FFFF
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);
}

/**
 * @brief Scenario 5: ORI與$zero暫存器
 * Given register $zero contains 0 (always)
 * When  I execute "ori $t1, $zero, 0x1234"
 * Then  register $t1 should contain 0x00001234
 */
TEST_F(OriInstructionTest, OriInstruction_WithZeroRegister_ShouldPass) {
    // Arrange: $zero應該總是0
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);
    
    // Act: 執行ORI指令
    mips::OriInstruction instr(9, 0, static_cast<int16_t>(0x1234));  // ori $t1, $zero, 0x1234
    instr.execute(*cpu);
    
    // Assert: 0 OR 0x1234 = 0x00001234
    uint32_t expectedResult = 0x00001234;
    EXPECT_EQ(cpu->getRegisterFile().read(9), expectedResult);
    
    // 確認$zero仍然是0
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);
}

// ============================================================================
// Phase A: 測試框架驗證（這些應該通過，確認測試環境正常）
// ============================================================================

/**
 * @brief 框架測試：確認CPU初始化正常
 */
TEST_F(OriInstructionTest, Framework_CpuInitialization_ShouldPass) {
    // 驗證CPU初始狀態
    EXPECT_EQ(cpu->getProgramCounter(), 0);
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);  // $zero
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0);  // $t0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);  // $t1
}

/**
 * @brief 框架測試：確認暫存器讀寫功能正常
 */
TEST_F(OriInstructionTest, Framework_RegisterReadWrite_ShouldPass) {
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
