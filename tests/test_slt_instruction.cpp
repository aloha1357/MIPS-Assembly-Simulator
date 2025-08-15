/**
 * @file test_slt_instruction.cpp
 * @brief TDD Tests for SLT and SLTU Instructions - Phase A: Red Light
 * 
 * 這個檔案專門測試 SLT 和 SLTU 指令的實作
 * 按照 BDD 最小增量原則，先建立失敗的測試
 */

#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Instruction.h"
#include <memory>

/**
 * @brief SLTU Instruction Test Fixture
 */
class SltuInstructionTest : public ::testing::Test {
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
// Phase A: 紅燈測試 - SLTU 指令（預期失敗）
// ============================================================================

/**
 * @brief Test 1: SLTU 指令基本功能 - 正數無符號比較
 * 期望失敗：SltuInstruction 類別尚未實作
 */
TEST_F(SltuInstructionTest, SltuInstruction_PositiveNumbers_ShouldSetCorrectResult) {
    // Arrange: 5 < 10 (unsigned) 應該回傳 1
    cpu->getRegisterFile().write(8, 5);   // $t0 = 5
    cpu->getRegisterFile().write(9, 10);  // $t1 = 10
    
    // Act: 執行 sltu $t2, $t0, $t1
    // 這裡應該會編譯失敗，因為 SltuInstruction 還沒宣告
    mips::SltuInstruction instr(10, 8, 9);  // sltu $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert: 5 < 10 is true (unsigned), so result should be 1
    EXPECT_EQ(cpu->getRegisterFile().read(10), 1);
    
    // 確認源暫存器沒有被修改
    EXPECT_EQ(cpu->getRegisterFile().read(8), 5);
    EXPECT_EQ(cpu->getRegisterFile().read(9), 10);
}

/**
 * @brief Test 2: SLTU 指令 - 大數值與小數值無符號比較（關鍵測試）
 * 期望失敗：SltuInstruction::execute 方法尚未實作
 * 
 * 這是 SLTU 與 SLT 的關鍵差異測試：
 * 0xFFFFFFFF 在有符號數是 -1，在無符號數是 4294967295
 */
TEST_F(SltuInstructionTest, SltuInstruction_UnsignedVsSigned_CriticalDifference) {
    // Arrange: 0xFFFFFFFF vs 5 的無符號比較
    cpu->getRegisterFile().write(8, 0xFFFFFFFF);  // $t0 = 4294967295 (unsigned)
    cpu->getRegisterFile().write(9, 5);           // $t1 = 5
    
    // Act: 執行 sltu $t2, $t0, $t1
    mips::SltuInstruction instr(10, 8, 9);
    instr.execute(*cpu);
    
    // Assert: 4294967295 < 5 is false (unsigned), so result should be 0
    // 注意：這與 SLT 指令不同，SLT 會認為 -1 < 5 是 true
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0);
}

/**
 * @brief Test 3: SLTU 指令 - 反向測試
 * 期望失敗：需要正確處理無符號比較邏輯
 */
TEST_F(SltuInstructionTest, SltuInstruction_SmallVsLarge_ShouldReturnOne) {
    // Arrange: 5 vs 0xFFFFFFFF 的無符號比較
    cpu->getRegisterFile().write(8, 5);           // $t0 = 5
    cpu->getRegisterFile().write(9, 0xFFFFFFFF);  // $t1 = 4294967295 (unsigned)
    
    // Act: 執行 sltu $t2, $t0, $t1
    mips::SltuInstruction instr(10, 8, 9);
    instr.execute(*cpu);
    
    // Assert: 5 < 4294967295 is true (unsigned), so result should be 1
    EXPECT_EQ(cpu->getRegisterFile().read(10), 1);
}

/**
 * @brief Test 4: SLTU 指令 - 相等值比較
 * 期望失敗：需要正確處理相等情況
 */
TEST_F(SltuInstructionTest, SltuInstruction_EqualValues_ShouldReturnZero) {
    // Arrange: 0x80000000 == 0x80000000 應該回傳 0
    cpu->getRegisterFile().write(8, 0x80000000);  // $t0 = 2147483648
    cpu->getRegisterFile().write(9, 0x80000000);  // $t1 = 2147483648
    
    // Act: 執行 sltu $t2, $t0, $t1
    mips::SltuInstruction instr(10, 8, 9);
    instr.execute(*cpu);
    
    // Assert: 2147483648 < 2147483648 is false, so result should be 0
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0);
}

/**
 * @brief Test 5: SLTU 指令 - 零值比較
 * 期望失敗：邊界值測試
 */
TEST_F(SltuInstructionTest, SltuInstruction_ZeroComparison_ShouldWork) {
    // Arrange: 0 vs 1 的無符號比較
    cpu->getRegisterFile().write(8, 0);  // $t0 = 0
    cpu->getRegisterFile().write(9, 1);  // $t1 = 1
    
    // Act: 執行 sltu $t2, $t0, $t1
    mips::SltuInstruction instr(10, 8, 9);
    instr.execute(*cpu);
    
    // Assert: 0 < 1 is true, so result should be 1
    EXPECT_EQ(cpu->getRegisterFile().read(10), 1);
}

/**
 * @brief Test 6: SLTU 指令 - 程式計數器遞增測試
 * 期望失敗：PC 應該在指令執行後遞增
 */
TEST_F(SltuInstructionTest, SltuInstruction_ProgramCounter_ShouldIncrement) {
    // Arrange
    cpu->getRegisterFile().write(8, 100);  // $t0 = 100
    cpu->getRegisterFile().write(9, 200);  // $t1 = 200
    cpu->setProgramCounter(50);            // PC = 50
    
    // Act: 執行 sltu $t2, $t0, $t1
    mips::SltuInstruction instr(10, 8, 9);
    instr.execute(*cpu);
    
    // Assert: PC 應該遞增 1
    EXPECT_EQ(cpu->getProgramCounter(), 51);
}

// ============================================================================
// Integration Tests - 這些測試驗證 SLTU 指令與整個系統的集成
// ============================================================================

/**
 * @brief Integration Test: SLTU 指令與解碼器集成
 * 期望失敗：解碼器還沒支援 SLTU 指令
 */
TEST_F(SltuInstructionTest, DISABLED_SltuInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // 這個測試將在 Phase B 實作解碼器支援後啟用
    // TODO: 在 InstructionDecoder 中添加 SLTU 支援後移除 DISABLED_ 前綴
}

/**
 * @brief Integration Test: SLTU 指令與彙編器集成  
 * 期望失敗：彙編器還沒支援 SLTU 指令文字解析
 */
TEST_F(SltuInstructionTest, DISABLED_SltuInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // 這個測試將在 Phase B 實作彙編器支援後啟用
    // TODO: 在 Assembler 中添加 SLTU 指令解析後移除 DISABLED_ 前綴
}
