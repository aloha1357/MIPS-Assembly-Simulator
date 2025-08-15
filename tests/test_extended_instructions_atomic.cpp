/**
 * @file test_extended_instructions_atomic.cpp
 * @brief 原子化擴展指令測試套件
 * 
 * 這個測試套件專門用於測試新實作的擴展MIPS指令：SLT, SLTI, BNE, BGTZ等
 * 每個測試都是原子化的，專注於測試單一功能點
 * 
 * BDD開發方法：
 * A. 撰寫步驟：定義測試接口，實作留空，測試應該失敗
 * B. 實作程式碼：使測試通過
 * C. 重構：改善程式碼品質
 */

#include <gtest/gtest.h>
#include "../src/Instruction.h"
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Memory.h"
#include <chrono>
#include <memory>

namespace mips {
namespace test {

/**
 * @brief 擴展指令測試基底類別
 * 
 * 提供共用的設定和輔助方法
 */
class ExtendedInstructionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        // 確保所有暫存器都初始化為零
        for (int i = 0; i < 32; i++) {
            cpu->getRegisterFile().write(i, 0);
        }
    }
    
    void TearDown() override {
        cpu.reset();
    }
    
    /**
     * @brief 輔助方法：驗證暫存器值
     */
    void expectRegisterValue(int reg, uint32_t expected, const std::string& message = "") {
        uint32_t actual = cpu->getRegisterFile().read(reg);
        EXPECT_EQ(actual, expected) << message << " (reg=" << reg << ")";
    }
    
    /**
     * @brief 輔助方法：設定暫存器值
     */
    void setRegisterValue(int reg, uint32_t value) {
        cpu->getRegisterFile().write(reg, value);
    }
    
    /**
     * @brief 輔助方法：設定有符號暫存器值
     */
    void setRegisterValueSigned(int reg, int32_t value) {
        cpu->getRegisterFile().write(reg, static_cast<uint32_t>(value));
    }
    
    /**
     * @brief 輔助方法：取得有符號暫存器值
     */
    int32_t getRegisterValueSigned(int reg) {
        return static_cast<int32_t>(cpu->getRegisterFile().read(reg));
    }

protected:
    std::unique_ptr<mips::Cpu> cpu;
};

// ====================================================================
// Phase A: SLT 指令原子測試 - RED 階段（預期失敗）
// ====================================================================

/**
 * @brief 原子測試B.1: SLT指令基本功能 - 正數比較
 * 
 * 測試場景：rs < rt (5 < 10)，應該設定rd = 1
 * 
 * Phase B: 此測試現在應該失敗，因為SLT指令尚未實作
 */
TEST_F(ExtendedInstructionsTest, SltInstruction_PositiveNumbers_LessThan) {
    // Arrange
    setRegisterValueSigned(8, 5);    // $t0 = 5
    setRegisterValueSigned(9, 10);   // $t1 = 10
    
    // Act
    // TODO: 在Phase B中實作SLT指令類別
    mips::SltInstruction instr(10, 8, 9);  // slt $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert  
    // TODO: 在Phase B中驗證結果
    expectRegisterValue(10, 1, "SLT: 5 < 10 should set result to 1");
    
    // Phase A: 暫時讓測試通過以避免編譯錯誤
    // SUCCEED() << "Phase A: SLT指令尚未實作，此測試將在Phase B中實作";
}

/**
 * @brief 原子測試B.2: SLT指令基本功能 - 正數比較（反向）
 * 
 * 測試場景：rs > rt (10 > 5)，應該設定rd = 0
 */
TEST_F(ExtendedInstructionsTest, SltInstruction_PositiveNumbers_GreaterThan) {
    // Arrange
    setRegisterValueSigned(16, 10);  // $s0 = 10
    setRegisterValueSigned(17, 5);   // $s1 = 5
    
    // Act
    // TODO: 在Phase B中實作
    mips::SltInstruction instr(18, 16, 17);  // slt $s2, $s0, $s1
    instr.execute(*cpu);
    
    // Assert
    // TODO: 在Phase B中驗證
    expectRegisterValue(18, 0, "SLT: 10 > 5 should set result to 0");
    
    // SUCCEED() << "Phase A: SLT指令尚未實作";
}

/**
 * @brief 原子測試B.3: SLT指令 - 負數與正數比較
 * 
 * 測試場景：rs < rt (-5 < 5)，應該設定rd = 1
 */
TEST_F(ExtendedInstructionsTest, SltInstruction_NegativeVsPositive) {
    // Arrange
    setRegisterValueSigned(4, -5);   // $a0 = -5
    setRegisterValueSigned(5, 5);    // $a1 = 5
    
    // Act
    // TODO: 在Phase B中實作
    mips::SltInstruction instr(2, 4, 5);  // slt $v0, $a0, $a1
    instr.execute(*cpu);
    
    // Assert
    // TODO: 在Phase B中驗證
    expectRegisterValue(2, 1, "SLT: -5 < 5 should set result to 1");
    
    // SUCCEED() << "Phase A: SLT指令尚未實作";
}

/**
 * @brief 原子測試B.4: SLT指令 - 相等數值比較
 * 
 * 測試場景：rs == rt (10 == 10)，應該設定rd = 0
 */
TEST_F(ExtendedInstructionsTest, SltInstruction_EqualValues) {
    // Arrange
    setRegisterValueSigned(11, 10);  // $t3 = 10
    setRegisterValueSigned(12, 10);  // $t4 = 10
    
    // Act
    // TODO: 在Phase B中實作
    mips::SltInstruction instr(13, 11, 12);  // slt $t5, $t3, $t4
    instr.execute(*cpu);
    
    // Assert
    // TODO: 在Phase B中驗證
    expectRegisterValue(13, 0, "SLT: 10 == 10 should set result to 0");
    
    // SUCCEED() << "Phase A: SLT指令尚未實作";
}

/**
 * @brief 原子測試B.5: SLT指令 - 邊界值測試（最大正數與最小負數）
 * 
 * 測試場景：INT32_MIN < INT32_MAX，應該設定rd = 1
 */
TEST_F(ExtendedInstructionsTest, SltInstruction_EdgeCases) {
    // Arrange
    setRegisterValueSigned(14, INT32_MIN);  // $t6 = -2147483648
    setRegisterValueSigned(15, INT32_MAX);  // $t7 = 2147483647
    
    // Act
    // TODO: 在Phase B中實作
    mips::SltInstruction instr(24, 14, 15);  // slt $t8, $t6, $t7
    instr.execute(*cpu);
    
    // Assert
    // TODO: 在Phase B中驗證
    expectRegisterValue(24, 1, "SLT: INT32_MIN < INT32_MAX should set result to 1");
    
    // SUCCEED() << "Phase A: SLT指令尚未實作";
}

// ====================================================================
// Phase A 完成總結
// ====================================================================

/*
 * PHASE A 完成清單：
 * 
 * ✅ Extended Instructions Feature文件中只啟用SLT scenario
 * ✅ 其他scenarios全部標記為@ignore
 * ✅ 建立測試類別ExtendedInstructionsTest
 * ✅ 定義5個原子化SLT測試案例
 * ✅ 所有測試標記為DISABLED_（避免編譯錯誤）
 * ✅ 測試覆蓋場景：正數、負數、相等、邊界值
 * ⚠️  下一步：運行測試確認RED狀態
 * 
 * 預期測試結果：
 * - 編譯成功（因為使用DISABLED_前綴）
 * - 測試不會執行（被跳過）
 * - 這確認了測試框架設定正確
 * 
 * Phase B 任務：
 * 1. 移除DISABLED_前綴，讓測試執行（RED狀態）
 * 2. 在Instruction.h中新增SltInstruction類別
 * 3. 實作SltInstruction::execute()方法
 * 4. 在InstructionDecoder中新增SLT支援（function code 0x2A）
 * 5. 運行測試達到GREEN狀態
 */

} // namespace test
} // namespace mips
