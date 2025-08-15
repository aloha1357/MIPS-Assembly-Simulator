/**
 * @file test_bne_instruction.cpp
 * @brief TDD Tests for BNE Instruction - Phase A: Red Light
 * 
 * BNE (Branch Not Equal) - I-type 分支指令
 * 功能碼: 0x05
 * 格式: bne $rs, $rt, offset
 * 功能: if (rs != rt) PC = PC + 4 + (sign_extend(offset) << 2)
 * 
 * 按照 BDD 最小增量原則，先建立失敗的測試
 */

#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Instruction.h"
#include <memory>

/**
 * @brief BNE Instruction Test Fixture
 */
class BneInstructionTest : public ::testing::Test {
protected:
    std::unique_ptr<mips::Cpu> cpu;
    
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        
        // 設定CPU為乾淨狀態
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
// Phase A: 紅燈測試 - BNE 指令（預期失敗）
// ============================================================================

/**
 * @brief Test 1: BNE 指令基本功能 - 不相等時分支
 * 期望失敗：BneInstruction 類別尚未實作
 */
TEST_F(BneInstructionTest, BneInstruction_NotEqual_ShouldBranch) {
    // Arrange: $t0 != $t1 應該執行分支
    cpu->getRegisterFile().write(8, 5);   // $t0 = 5
    cpu->getRegisterFile().write(9, 10);  // $t1 = 10
    cpu->setProgramCounter(100);          // PC = 100
    
    // Act: 執行 bne $t0, $t1, 4 (跳躍4個指令)
    // 這裡應該會編譯失敗，因為 BneInstruction 還沒宣告
    mips::BneInstruction instr(8, 9, 4);  // bne $t0, $t1, 4
    instr.execute(*cpu);
    
    // Assert: 5 != 10 is true, 所以應該分支
    // PC = 100 + 4 + (4 << 2) = 100 + 4 + 16 = 120
    EXPECT_EQ(cpu->getProgramCounter(), 120);
    
    // 確認源暫存器沒有被修改
    EXPECT_EQ(cpu->getRegisterFile().read(8), 5);
    EXPECT_EQ(cpu->getRegisterFile().read(9), 10);
}

/**
 * @brief Test 2: BNE 指令 - 相等時不分支
 * 期望失敗：BneInstruction::execute 方法尚未實作
 */
TEST_F(BneInstructionTest, BneInstruction_Equal_ShouldNotBranch) {
    // Arrange: $t0 == $t1 不應該執行分支
    cpu->getRegisterFile().write(8, 42);  // $t0 = 42
    cpu->getRegisterFile().write(9, 42);  // $t1 = 42
    cpu->setProgramCounter(100);          // PC = 100
    
    // Act: 執行 bne $t0, $t1, 10
    mips::BneInstruction instr(8, 9, 10);
    instr.execute(*cpu);
    
    // Assert: 42 == 42 is true, 所以不應該分支
    // PC = 100 + 4 = 104 (正常遞增)
    EXPECT_EQ(cpu->getProgramCounter(), 104);
}

/**
 * @brief Test 3: BNE 指令 - 負偏移量分支
 * 期望失敗：需要正確處理負偏移量
 */
TEST_F(BneInstructionTest, BneInstruction_NegativeOffset_ShouldBranchBackward) {
    // Arrange: 測試向後分支
    cpu->getRegisterFile().write(8, 1);   // $t0 = 1
    cpu->getRegisterFile().write(9, 2);   // $t1 = 2
    cpu->setProgramCounter(100);          // PC = 100
    
    // Act: 執行 bne $t0, $t1, -5 (向後跳躍)
    mips::BneInstruction instr(8, 9, -5);
    instr.execute(*cpu);
    
    // Assert: 1 != 2 is true, 所以應該向後分支
    // PC = 100 + 4 + (-5 << 2) = 100 + 4 - 20 = 84
    EXPECT_EQ(cpu->getProgramCounter(), 84);
}

/**
 * @brief Test 4: BNE 指令 - 零偏移量測試
 * 期望失敗：需要正確處理零偏移量
 */
TEST_F(BneInstructionTest, BneInstruction_ZeroOffset_ShouldBranchToNextInstruction) {
    // Arrange: 測試零偏移量分支
    cpu->getRegisterFile().write(8, 100);  // $t0 = 100
    cpu->getRegisterFile().write(9, 200);  // $t1 = 200
    cpu->setProgramCounter(50);            // PC = 50
    
    // Act: 執行 bne $t0, $t1, 0
    mips::BneInstruction instr(8, 9, 0);
    instr.execute(*cpu);
    
    // Assert: 100 != 200 is true, 零偏移量分支
    // PC = 50 + 4 + (0 << 2) = 50 + 4 + 0 = 54
    EXPECT_EQ(cpu->getProgramCounter(), 54);
}

/**
 * @brief Test 5: BNE 指令 - 使用 $zero 暫存器
 * 期望失敗：需要正確處理 $zero 暫存器
 */
TEST_F(BneInstructionTest, BneInstruction_WithZeroRegister_ShouldWork) {
    // Arrange: 與 $zero 暫存器比較
    cpu->getRegisterFile().write(8, 0);   // $t0 = 0 (與 $zero 相同)
    cpu->getRegisterFile().write(0, 0);   // $zero = 0 (永遠是0)
    cpu->setProgramCounter(200);          // PC = 200
    
    // Act: 執行 bne $t0, $zero, 3
    mips::BneInstruction instr(8, 0, 3);
    instr.execute(*cpu);
    
    // Assert: 0 == 0 is true, 所以不應該分支
    // PC = 200 + 4 = 204
    EXPECT_EQ(cpu->getProgramCounter(), 204);
}

/**
 * @brief Test 6: BNE 指令 - 相同暫存器比較
 * 期望失敗：需要正確處理相同暫存器比較
 */
TEST_F(BneInstructionTest, BneInstruction_SameRegister_ShouldNotBranch) {
    // Arrange: 同一個暫存器與自己比較
    cpu->getRegisterFile().write(8, 999);  // $t0 = 999
    cpu->setProgramCounter(300);           // PC = 300
    
    // Act: 執行 bne $t0, $t0, 100
    mips::BneInstruction instr(8, 8, 100);
    instr.execute(*cpu);
    
    // Assert: $t0 == $t0 永遠為真, 所以不應該分支
    // PC = 300 + 4 = 304
    EXPECT_EQ(cpu->getProgramCounter(), 304);
}

/**
 * @brief Test 7: BNE 指令 - 大偏移量測試
 * 期望失敗：需要正確處理16位偏移量的符號擴展
 */
TEST_F(BneInstructionTest, BneInstruction_LargeOffset_ShouldWork) {
    // Arrange: 測試大偏移量
    cpu->getRegisterFile().write(8, 1000);  // $t0 = 1000
    cpu->getRegisterFile().write(9, 2000);  // $t1 = 2000
    cpu->setProgramCounter(1000);           // PC = 1000
    
    // Act: 執行 bne $t0, $t1, 1000 (大偏移量)
    mips::BneInstruction instr(8, 9, 1000);
    instr.execute(*cpu);
    
    // Assert: 1000 != 2000 is true, 應該分支
    // PC = 1000 + 4 + (1000 << 2) = 1000 + 4 + 4000 = 5004
    EXPECT_EQ(cpu->getProgramCounter(), 5004);
}

// ============================================================================
// Integration Tests - 這些測試驗證 BNE 指令與整個系統的集成
// ============================================================================

/**
 * @brief Integration Test: BNE 指令與解碼器集成
 * 期望失敗：解碼器還沒支援 BNE 指令
 */
TEST_F(BneInstructionTest, DISABLED_BneInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // 這個測試將在 Phase B 實作解碼器支援後啟用
    // TODO: 在 InstructionDecoder 中添加 BNE 支援後移除 DISABLED_ 前綴
    // 功能碼應該是 0x05
}

/**
 * @brief Integration Test: BNE 指令與彙編器集成  
 * 期望失敗：彙編器還沒支援 BNE 指令文字解析
 */
TEST_F(BneInstructionTest, DISABLED_BneInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // 這個測試將在 Phase B 實作彙編器支援後啟用
    // TODO: 在 Assembler 中添加 BNE 指令解析後移除 DISABLED_ 前綴
    // 應該解析 "bne $t0, $t1, label" 這樣的字串
}
