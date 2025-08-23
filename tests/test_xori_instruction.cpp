#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

namespace mips
{
namespace test
{

/**
 * @brief XORI指令測試框架
 *
 * 測試目標：驗證XORI (XOR Immediate) 指令的所有功能
 * 指令格式：xori $rt, $rs, immediate
 * 功能：$rt = $rs XOR zero_extend(immediate)
 */
class XoriInstructionTest : public ::testing::Test
{
  protected:
    std::unique_ptr<mips::Cpu> cpu;

    void SetUp() override
    {
        cpu = std::make_unique<mips::Cpu>();
        // 確保測試環境乾淨
        cpu->getRegisterFile().reset();
        cpu->getMemory().reset();
        cpu->setProgramCounter(0);
    }

    void TearDown() override
    {
        cpu.reset();
    }

    /**
     * @brief 設定暫存器值的輔助方法
     */
    void setRegisterValue(int reg, uint32_t value)
    {
        cpu->getRegisterFile().write(reg, value);
    }

    /**
     * @brief 驗證暫存器值的輔助方法
     */
    void expectRegisterValue(int reg, uint32_t expected, const std::string& message)
    {
        uint32_t actual = cpu->getRegisterFile().read(reg);
        EXPECT_EQ(expected, actual) << message << " - Register $" << reg << " Expected: 0x"
                                    << std::hex << expected << " Actual: 0x" << std::hex << actual;
    }
};

// ====================================================================
// BDD Phase A: 功能測試 (紅燈階段)
// ====================================================================

/**
 * @brief 測試1: XORI指令基本XOR運算
 *
 * 測試目標：驗證基本的XOR立即值運算
 * 測試案例：0xFFFF XORI 0x00FF = 0x0000FF00
 */
TEST_F(XoriInstructionTest, XoriInstruction_BasicXorOperation)
{
    // Arrange
    setRegisterValue(8, 0x0000FFFF);  // $t0 = 0x0000FFFF

    // Act
    mips::XoriInstruction instr(9, 8, 0x00FF);  // xori $t1, $t0, 0x00FF
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(9, 0x0000FF00, "Basic XOR with immediate should work correctly");
    expectRegisterValue(0, 0x00000000, "$zero register should remain zero");
}

/**
 * @brief 測試2: XORI指令零立即值恆等運算
 *
 * 測試目標：驗證與零的XOR運算（恆等操作）
 * 測試案例：0x12345678 XORI 0x0000 = 0x12345678
 */
TEST_F(XoriInstructionTest, XoriInstruction_ZeroImmediateIdentity)
{
    // Arrange
    setRegisterValue(12, 0x12345678);  // $t4

    // Act
    mips::XoriInstruction instr(13, 12, 0x0000);  // xori $t5, $t4, 0x0000
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(13, 0x12345678, "XOR with zero should preserve original value");
}

/**
 * @brief 測試3: XORI指令全1立即值
 *
 * 測試目標：驗證與全1立即值的XOR運算（位元翻轉低16位）
 * 測試案例：0x12345678 XORI 0xFFFF = 0x1234A987
 */
TEST_F(XoriInstructionTest, XoriInstruction_AllOnesImmediate)
{
    // Arrange
    setRegisterValue(14, 0x12345678);  // $t6

    // Act
    mips::XoriInstruction instr(15, 14, static_cast<int16_t>(0xFFFF));  // xori $t7, $t6, 0xFFFF
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(15, 0x1234A987, "XOR with all ones should flip lower 16 bits");
}

/**
 * @brief 測試4: XORI指令位元遮罩
 *
 * 測試目標：驗證使用立即值作為位元遮罩的XOR運算
 * 測試案例：0xAAAABBBB XORI 0x0F0F = 0xAAAAB4B4
 */
TEST_F(XoriInstructionTest, XoriInstruction_BitMask)
{
    // Arrange
    setRegisterValue(16, 0xAAAABBBB);  // $s0

    // Act
    mips::XoriInstruction instr(17, 16, 0x0F0F);  // xori $s1, $s0, 0x0F0F
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(17, 0xAAAAB4B4, "XOR with bit mask should flip specified bits");
}

/**
 * @brief 測試5: XORI指令與$zero暫存器
 *
 * 測試目標：驗證與$zero暫存器的XOR運算
 * 測試案例：$zero XORI 0x1234 = 0x00001234
 */
TEST_F(XoriInstructionTest, XoriInstruction_WithZeroRegister)
{
    // Arrange
    setRegisterValue(0, 0x00000000);  // $zero (實際上總是0)

    // Act
    mips::XoriInstruction instr(18, 0, 0x1234);  // xori $s2, $zero, 0x1234
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(18, 0x00001234, "XOR with $zero should equal zero-extended immediate");
}

/**
 * @brief 測試6: XORI指令位元翻轉模式
 *
 * 測試目標：驗證特定位元模式的翻轉
 * 測試案例：0xDEADBEEF XORI 0x00FF = 0xDEADBE10
 */
TEST_F(XoriInstructionTest, XoriInstruction_BitFlipping)
{
    // Arrange
    setRegisterValue(19, 0xDEADBEEF);  // $s3

    // Act
    mips::XoriInstruction instr(20, 19, 0x00FF);  // xori $s4, $s3, 0x00FF
    instr.execute(*cpu);

    // Assert
    expectRegisterValue(20, 0xDEADBE10, "XOR should flip lower 8 bits correctly");
}

// ====================================================================
// BDD Phase A: 框架測試 (測試基礎設施驗證)
// ====================================================================

/**
 * @brief 框架測試1: 指令名稱驗證
 */
TEST_F(XoriInstructionTest, XoriInstruction_FrameworkTest_GetName)
{
    // Arrange & Act
    mips::XoriInstruction instr(1, 2, 0x1234);

    // Assert
    EXPECT_EQ("xori", instr.getName()) << "指令名稱應該是 'xori'";
}

/**
 * @brief 框架測試2: 程式計數器遞增驗證
 */
TEST_F(XoriInstructionTest, XoriInstruction_FrameworkTest_ProgramCounterIncrement)
{
    // Arrange
    cpu->setProgramCounter(100);
    setRegisterValue(8, 0x12345678);

    // Act
    mips::XoriInstruction instr(9, 8, 0x0000);
    instr.execute(*cpu);

    // Assert
    EXPECT_EQ(101, cpu->getProgramCounter()) << "程式計數器應該遞增1";
}

}  // namespace test
}  // namespace mips
