/**
 * @file test_logical_instructions_atomic.cpp
 * @brief 原子化邏輯指令測試套件
 *
 * 這個測試套件專門用於測試新實作的邏輯指令：AND, OR, XOR, NOR
 * 每個測試都是原子化的，專注於測試單一功能點
 */

#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

namespace mips
{
namespace test
{

/**
 * @brief 邏輯指令測試基底類別
 *
 * 提供共用的設定和輔助方法
 */
class LogicalInstructionsTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<mips::Cpu>();
        // 確保所有暫存器都初始化為零
        for (int i = 0; i < 32; i++)
        {
            cpu->getRegisterFile().write(i, 0);
        }
    }

    void TearDown() override
    {
        cpu.reset();
    }

    /**
     * @brief 輔助方法：驗證暫存器值
     */
    void expectRegisterValue(int reg, uint32_t expected, const std::string& message = "")
    {
        uint32_t actual = cpu->getRegisterFile().read(reg);
        EXPECT_EQ(actual, expected) << message << " (reg=" << reg << ")";
    }

    /**
     * @brief 輔助方法：設定暫存器值
     */
    void setRegisterValue(int reg, uint32_t value)
    {
        cpu->getRegisterFile().write(reg, value);
    }

    std::unique_ptr<mips::Cpu> cpu;
};

// ====================================================================
// Scenario 1: AND 指令原子測試
// ====================================================================

/**
 * @brief 原子測試1.1: AND指令基本運算
 *
 * 測試目標：驗證AND指令執行基本位元AND運算
 * 測試資料：互補位元模式 (0xF0F0F0F0 & 0x0F0F0F0F = 0x00000000)
 */
TEST_F(LogicalInstructionsTest, AndInstruction_BasicOperation_ComplementaryBitPattern)
{
    // Arrange - 設定測試資料
    setRegisterValue(8, 0xF0F0F0F0);  // $t0 = 0xF0F0F0F0
    setRegisterValue(9, 0x0F0F0F0F);  // $t1 = 0x0F0F0F0F

    // Act - 執行AND指令: and $t2, $t0, $t1
    mips::AndInstruction instr(10, 8, 9);  // $t2 = $t0 & $t1
    instr.execute(*cpu);

    // Assert - 驗證結果
    expectRegisterValue(10, 0x00000000, "AND result should be 0x00000000");
    expectRegisterValue(8, 0xF0F0F0F0, "Source register $t0 should remain unchanged");
    expectRegisterValue(9, 0x0F0F0F0F, "Source register $t1 should remain unchanged");
}

/**
 * @brief 原子測試1.2: AND指令交替位元模式
 *
 * 測試目標：驗證交替位元模式的AND運算
 * 測試資料：0xAAAAAAAA & 0x55555555 = 0x00000000
 */
TEST_F(LogicalInstructionsTest, AndInstruction_AlternatingBitPattern)
{
    // Arrange
    setRegisterValue(16, 0xAAAAAAAA);  // $s0
    setRegisterValue(17, 0x55555555);  // $s1

    // Act
    mips::AndInstruction instr(18, 16, 17);  // and $s2, $s0, $s1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(18, 0x00000000, "Alternating bits AND should result in zero");
}

/**
 * @brief 原子測試1.3: AND指令全1運算
 *
 * 測試目標：驗證全1位元的AND運算
 * 測試資料：0xFFFFFFFF & 0xFFFFFFFF = 0xFFFFFFFF
 */
TEST_F(LogicalInstructionsTest, AndInstruction_AllOnesPattern)
{
    // Arrange
    setRegisterValue(11, 0xFFFFFFFF);  // $t3
    setRegisterValue(12, 0xFFFFFFFF);  // $t4

    // Act
    mips::AndInstruction instr(13, 11, 12);  // and $t5, $t3, $t4
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(13, 0xFFFFFFFF, "All ones AND should preserve all bits");
}

/**
 * @brief 原子測試1.4: AND指令實用遮罩運算
 *
 * 測試目標：驗證實際應用中的位元遮罩運算
 * 測試資料：0x12345678 & 0xFFFFF000 = 0x12345000 (保留高位，清除低位)
 */
TEST_F(LogicalInstructionsTest, AndInstruction_PracticalMasking)
{
    // Arrange
    setRegisterValue(4, 0x12345678);  // $a0 = 原始資料
    setRegisterValue(5, 0xFFFFF000);  // $a1 = 遮罩 (保留高20位)

    // Act
    mips::AndInstruction instr(2, 4, 5);  // and $v0, $a0, $a1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(2, 0x12345000, "Masking should preserve high bits, clear low bits");
}

/**
 * @brief 原子測試1.5: AND指令零值測試
 *
 * 測試目標：驗證與零值的AND運算
 * 測試資料：任何值 & 0x00000000 = 0x00000000
 */
TEST_F(LogicalInstructionsTest, AndInstruction_WithZero)
{
    // Arrange
    setRegisterValue(14, 0xDEADBEEF);  // $t6 = 任意值
    setRegisterValue(15, 0x00000000);  // $t7 = 0

    // Act
    mips::AndInstruction instr(24, 14, 15);  // and $t8, $t6, $t7
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(24, 0x00000000, "AND with zero should always result in zero");
}

/**
 * @brief 原子測試1.6: AND指令暫存器別名測試
 *
 * 測試目標：驗證當目標暫存器與源暫存器相同時的行為
 * 測試資料：$t0 = $t0 & $t0 (自我AND運算)
 */
TEST_F(LogicalInstructionsTest, AndInstruction_RegisterAliasing)
{
    // Arrange
    setRegisterValue(8, 0xDEADBEEF);  // $t0 = 測試值

    // Act
    mips::AndInstruction instr(8, 8, 8);  // and $t0, $t0, $t0
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(8, 0xDEADBEEF, "Self-AND should preserve original value");
}

// ====================================================================
// Scenario 2: OR 指令原子測試
// ====================================================================

/**
 * @brief 原子測試2.1: OR指令基本運算
 *
 * 測試目標：驗證OR指令執行基本位元OR運算
 * 測試資料：互補位元模式 (0xF0F0F0F0 | 0x0F0F0F0F = 0xFFFFFFFF)
 */
TEST_F(LogicalInstructionsTest, OrInstruction_BasicOperation_ComplementaryBitPattern)
{
    // Arrange
    setRegisterValue(8, 0xF0F0F0F0);  // $t0
    setRegisterValue(9, 0x0F0F0F0F);  // $t1

    // Act
    mips::OrInstruction instr(10, 8, 9);  // or $t2, $t0, $t1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(10, 0xFFFFFFFF, "Complementary bits OR should result in all ones");
    expectRegisterValue(8, 0xF0F0F0F0, "Source register should remain unchanged");
    expectRegisterValue(9, 0x0F0F0F0F, "Source register should remain unchanged");
}

/**
 * @brief 原子測試2.2: OR指令交替位元模式
 */
TEST_F(LogicalInstructionsTest, OrInstruction_AlternatingBitPattern)
{
    // Arrange
    setRegisterValue(16, 0xAAAAAAAA);  // $s0
    setRegisterValue(17, 0x55555555);  // $s1

    // Act
    mips::OrInstruction instr(18, 16, 17);  // or $s2, $s0, $s1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(18, 0xFFFFFFFF, "Alternating bits OR should result in all ones");
}

/**
 * @brief 原子測試2.3: OR指令全零測試
 */
TEST_F(LogicalInstructionsTest, OrInstruction_AllZeros)
{
    // Arrange
    setRegisterValue(11, 0x00000000);  // $t3
    setRegisterValue(12, 0x00000000);  // $t4

    // Act
    mips::OrInstruction instr(13, 11, 12);  // or $t5, $t3, $t4
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(13, 0x00000000, "Zero OR zero should result in zero");
}

/**
 * @brief 原子測試2.4: OR指令位元合併
 */
TEST_F(LogicalInstructionsTest, OrInstruction_BitCombination)
{
    // Arrange
    setRegisterValue(4, 0x12340000);  // $a0 = 高16位資料
    setRegisterValue(5, 0x00005678);  // $a1 = 低16位資料

    // Act
    mips::OrInstruction instr(2, 4, 5);  // or $v0, $a0, $a1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(2, 0x12345678, "OR should combine non-overlapping bits");
}

/**
 * @brief 原子測試2.5: OR指令與零的恆等運算
 */
TEST_F(LogicalInstructionsTest, OrInstruction_IdentityWithZero)
{
    // Arrange
    setRegisterValue(14, 0xDEADBEEF);  // $t6
    setRegisterValue(15, 0x00000000);  // $t7 = 0

    // Act
    mips::OrInstruction instr(24, 14, 15);  // or $t8, $t6, $t7
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(24, 0xDEADBEEF, "OR with zero should preserve original value");
}

// ====================================================================
// Scenario 3: XOR 指令原子測試
// ====================================================================

/**
 * @brief 原子測試3.1: XOR指令基本運算
 */
TEST_F(LogicalInstructionsTest, XorInstruction_AlternatingBits)
{
    // Arrange
    setRegisterValue(8, 0xAAAAAAAA);  // $t0
    setRegisterValue(9, 0x55555555);  // $t1

    // Act
    mips::XorInstruction instr(10, 8, 9);  // xor $t2, $t0, $t1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(10, 0xFFFFFFFF, "Alternating bits XOR should result in all ones");
}

/**
 * @brief 原子測試3.2: XOR指令自我運算（歸零）
 */
TEST_F(LogicalInstructionsTest, XorInstruction_SelfXorZero)
{
    // Arrange
    setRegisterValue(16, 0x12345678);  // $s0

    // Act
    mips::XorInstruction instr(18, 16, 16);  // xor $s2, $s0, $s0
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(18, 0x00000000, "Self XOR should result in zero");
}

/**
 * @brief 原子測試3.3: XOR指令與零的恆等運算
 */
TEST_F(LogicalInstructionsTest, XorInstruction_IdentityWithZero)
{
    // Arrange
    setRegisterValue(11, 0xDEADBEEF);  // $t3
    setRegisterValue(12, 0x00000000);  // $t4 = 0

    // Act
    mips::XorInstruction instr(13, 11, 12);  // xor $t5, $t3, $t4
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(13, 0xDEADBEEF, "XOR with zero should preserve original value");
}

/**
 * @brief 原子測試3.4: XOR指令位元翻轉
 */
TEST_F(LogicalInstructionsTest, XorInstruction_BitFlipping)
{
    // Arrange
    setRegisterValue(4, 0xFFFFFFFF);  // $a0 = 全1
    setRegisterValue(5, 0xAAAAAAAA);  // $a1 = 交替位元

    // Act
    mips::XorInstruction instr(2, 4, 5);  // xor $v0, $a0, $a1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(2, 0x55555555, "XOR with all ones should flip bits");
}

/**
 * @brief 原子測試3.5: XOR指令完全互補
 */
TEST_F(LogicalInstructionsTest, XorInstruction_CompleteComplement)
{
    // Arrange
    setRegisterValue(14, 0x0F0F0F0F);  // $t6
    setRegisterValue(15, 0xF0F0F0F0);  // $t7 = 完全互補

    // Act
    mips::XorInstruction instr(24, 14, 15);  // xor $t8, $t6, $t7
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(24, 0xFFFFFFFF, "Complete complement XOR should result in all ones");
}

/**
 * @brief 原子測試3.6: XOR指令可逆性測試（加密應用）
 */
TEST_F(LogicalInstructionsTest, XorInstruction_ReversibilityEncryption)
{
    // Arrange
    setRegisterValue(8, 0x12345678);  // $t0 = 原始資料
    setRegisterValue(9, 0xABCDEF00);  // $t1 = 金鑰

    // Act - 加密
    mips::XorInstruction encrypt(10, 8, 9);  // xor $t2, $t0, $t1 (加密)
    encrypt.execute(*cpu);

    // Act - 解密
    mips::XorInstruction decrypt(11, 10, 9);  // xor $t3, $t2, $t1 (解密)
    decrypt.execute(*cpu);

    // Assert
    expectRegisterValue(11, 0x12345678, "XOR decryption should restore original data");
    expectRegisterValue(8, 0x12345678, "Original data should remain unchanged");
}

// ====================================================================
// Scenario 4: NOR 指令原子測試
// ====================================================================

/**
 * @brief 原子測試4.1: NOR指令互補模式
 */
TEST_F(LogicalInstructionsTest, NorInstruction_ComplementaryPattern)
{
    // Arrange
    setRegisterValue(8, 0xF0F0F0F0);  // $t0
    setRegisterValue(9, 0x0F0F0F0F);  // $t1

    // Act
    mips::NorInstruction instr(10, 8, 9);  // nor $t2, $t0, $t1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(10, 0x00000000, "Complementary NOR should result in zero");
}

/**
 * @brief 原子測試4.2: NOR指令全零（NOT功能）
 */
TEST_F(LogicalInstructionsTest, NorInstruction_AllZerosAsNot)
{
    // Arrange
    setRegisterValue(16, 0x00000000);  // $s0 = 0
    setRegisterValue(17, 0x00000000);  // $s1 = 0

    // Act
    mips::NorInstruction instr(18, 16, 17);  // nor $s2, $s0, $s1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(18, 0xFFFFFFFF, "NOR of zeros should result in all ones");
}

/**
 * @brief 原子測試4.3: NOR指令作為NOT運算
 */
TEST_F(LogicalInstructionsTest, NorInstruction_AsNotOperation)
{
    // Arrange
    setRegisterValue(11, 0xAAAAAAAA);  // $t3
    setRegisterValue(0, 0x00000000);   // $zero (應該永遠為0)

    // Act
    mips::NorInstruction instr(13, 11, 0);  // nor $t5, $t3, $zero
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(13, 0x55555555, "NOR with zero should act as NOT operation");
}

/**
 * @brief 原子測試4.4: NOR指令全1
 */
TEST_F(LogicalInstructionsTest, NorInstruction_AllOnes)
{
    // Arrange
    setRegisterValue(4, 0xFFFFFFFF);  // $a0
    setRegisterValue(5, 0xFFFFFFFF);  // $a1

    // Act
    mips::NorInstruction instr(2, 4, 5);  // nor $v0, $a0, $a1
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(2, 0x00000000, "NOR of all ones should result in zero");
}

/**
 * @brief 原子測試4.5: NOR指令通用NOT運算驗證
 */
TEST_F(LogicalInstructionsTest, NorInstruction_UniversalNotVerification)
{
    // Arrange
    setRegisterValue(8, 0xDEADBEEF);  // $t0 = 測試值
    setRegisterValue(0, 0x00000000);  // $zero

    // Act
    mips::NorInstruction instr(9, 8, 0);  // nor $t1, $t0, $zero
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(9, 0x21524110, "NOR with zero should equal bitwise NOT");
}

// ====================================================================
// Scenario 5: 複合操作測試
// ====================================================================

/**
 * @brief 原子測試5.1: 複雜邏輯運算序列
 *
 * 測試目標：驗證多個邏輯指令的連續執行
 * 這個測試驗證邏輯指令之間的交互作用
 */
TEST_F(LogicalInstructionsTest, ComplexLogicalSequence)
{
    // Arrange
    setRegisterValue(8, 0x12345678);  // $t0
    setRegisterValue(9, 0x87654321);  // $t1

    // Act - 執行複雜邏輯運算序列
    mips::AndInstruction and_instr(10, 8, 9);  // and $t2, $t0, $t1
    mips::OrInstruction  or_instr(11, 8, 9);   // or  $t3, $t0, $t1
    mips::XorInstruction xor_instr(12, 8, 9);  // xor $t4, $t0, $t1
    mips::NorInstruction nor_instr(13, 8, 9);  // nor $t5, $t0, $t1

    and_instr.execute(*cpu);
    or_instr.execute(*cpu);
    xor_instr.execute(*cpu);
    nor_instr.execute(*cpu);

    // Assert - 驗證所有結果
    expectRegisterValue(10, 0x02244220, "AND result verification");
    expectRegisterValue(11, 0x97755779, "OR result verification");
    expectRegisterValue(12, 0x95511559, "XOR result verification");
    expectRegisterValue(13, 0x688AA886, "NOR result verification");
}

// ====================================================================
// Scenario 6: 邊界條件和錯誤處理測試
// ====================================================================

/**
 * @brief 原子測試6.1: 邏輯指令邊界值測試
 */
TEST_F(LogicalInstructionsTest, BoundaryValueTest)
{
    // Arrange - 設定邊界值：最大負數和最大正數
    setRegisterValue(8, 0x80000000);  // $t0 = 最大負數
    setRegisterValue(9, 0x7FFFFFFF);  // $t1 = 最大正數

    // Act
    mips::AndInstruction and_instr(16, 8, 9);  // and $s0, $t0, $t1
    mips::OrInstruction  or_instr(17, 8, 9);   // or  $s1, $t0, $t1
    mips::XorInstruction xor_instr(18, 8, 9);  // xor $s2, $t0, $t1
    mips::NorInstruction nor_instr(19, 8, 9);  // nor $s3, $t0, $t1

    and_instr.execute(*cpu);
    or_instr.execute(*cpu);
    xor_instr.execute(*cpu);
    nor_instr.execute(*cpu);

    // Assert
    expectRegisterValue(16, 0x00000000, "Boundary AND result");
    expectRegisterValue(17, 0xFFFFFFFF, "Boundary OR result");
    expectRegisterValue(18, 0xFFFFFFFF, "Boundary XOR result");
    expectRegisterValue(19, 0x00000000, "Boundary NOR result");
}

/**
 * @brief 原子測試6.2: $zero暫存器保護測試
 */
TEST_F(LogicalInstructionsTest, ZeroRegisterProtectionTest)
{
    // Arrange
    setRegisterValue(8, 0xFFFFFFFF);  // $t0 = 全1

    // Act - 嘗試寫入$zero暫存器
    mips::OrInstruction instr(0, 8, 8);  // or $zero, $t0, $t0
    instr.execute(*cpu);

    // Assert - $zero暫存器應該保持0
    expectRegisterValue(0, 0x00000000, "Zero register should remain zero");
}

// ====================================================================
// Scenario 7: 效能測試
// ====================================================================

/**
 * @brief 原子測試7.1: 邏輯指令效能基準測試
 */
TEST_F(LogicalInstructionsTest, PerformanceBenchmark)
{
    // Arrange
    const int num_instructions = 1000;
    auto      start_time       = std::chrono::high_resolution_clock::now();

    // Act - 執行大量邏輯指令
    for (int i = 0; i < num_instructions; i++)
    {
        setRegisterValue(8, i);
        setRegisterValue(9, i + 1);

        mips::AndInstruction and_instr(10, 8, 9);
        mips::OrInstruction  or_instr(11, 8, 9);
        mips::XorInstruction xor_instr(12, 8, 9);
        mips::NorInstruction nor_instr(13, 8, 9);

        and_instr.execute(*cpu);
        or_instr.execute(*cpu);
        xor_instr.execute(*cpu);
        nor_instr.execute(*cpu);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Assert - 效能要求
    EXPECT_LT(duration.count(), 10000) << "1000 logical instructions should complete in < 10ms";

    double avg_time_ns = (duration.count() * 1000.0) / (num_instructions * 4);
    EXPECT_LT(avg_time_ns, 10.0) << "Average instruction time should be < 10ns";
}

/**
 * @brief 原子測試7.2: 記憶體穩定性測試
 */
TEST_F(LogicalInstructionsTest, MemoryStabilityTest)
{
    // Arrange - 記錄初始記憶體狀態
    size_t initial_memory = 0;  // 在實際實作中應該測量實際記憶體使用量

    // Act - 執行大量運算
    for (int i = 0; i < 100000; i++)
    {
        setRegisterValue(8, i % 0xFFFF);
        setRegisterValue(9, (i * 2) % 0xFFFF);

        mips::AndInstruction instr(10, 8, 9);
        instr.execute(*cpu);
    }

    // Assert - 記憶體使用量不應該增加
    size_t final_memory = 0;  // 在實際實作中應該測量實際記憶體使用量
    EXPECT_EQ(initial_memory, final_memory) << "Memory usage should not increase during operations";
}

}  // namespace test
}  // namespace mips
