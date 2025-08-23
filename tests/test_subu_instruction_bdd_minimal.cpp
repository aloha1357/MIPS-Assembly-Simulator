/**
 * @file test_subu_instruction_bdd_minimal.cpp
 * @brief Phase 4.2 BDD測試 - SUBU指令 (Subtract Unsigned) 最小化測試場景
 *
 * 開發方法論: 嚴格BDD Red-Light → Green-Light → Integration
 * 遵循"最小增量原則，一次實作一個 Scenario"
 *
 * SUBU指令規格:
 * - R-type指令，Function Code: 0x23
 * - 語法: subu $rd, $rs, $rt
 * - 功能: rd = rs - rt (無下溢檢查)
 * - 特點: 不會產生算術異常，即使結果為負數或發生下溢
 */

#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

namespace mips
{

/**
 * @brief SUBU指令BDD測試基礎類別
 *
 * 提供Given-When-Then模式的測試基礎設施
 * 每個測試場景遵循BDD三段式結構
 */
class SUBUInstructionBDDTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu       = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder   = std::make_unique<mips::InstructionDecoder>();
    }

    void TearDown() override
    {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }

    std::unique_ptr<mips::Cpu>                cpu;
    std::unique_ptr<mips::Assembler>          assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;
};

/**
 * @brief Scenario: 基本無符號減法
 *
 * Given: 兩個正數寄存器值
 * When: 執行SUBU減法指令
 * Then: 結果寄存器應包含正確的差值
 */
TEST_F(SUBUInstructionBDDTest, BasicUnsignedSubtraction)
{
    // Given: 準備測試資料
    const int      rd_register     = 3;   // $v1
    const int      rs_register     = 1;   // $at
    const int      rt_register     = 2;   // $v0
    const uint32_t rs_value        = 100; // 被減數
    const uint32_t rt_value        = 30;  // 減數
    const uint32_t expected_result = 70;  // 預期結果

    // Given: 設定初始寄存器狀態
    cpu->getRegisterFile().write(rs_register, rs_value);
    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rd_register, 0xFFFFFFFF); // 清除目標寄存器

    // Given: 建構SUBU指令 (Function Code: 0x23)
    uint32_t instruction_encoding =
        (rs_register << 21) | (rt_register << 16) | (rd_register << 11) | 0x23;
    (void)instruction_encoding; // Suppress unused variable warning

    // When: 創建並執行SUBU指令
    mips::SUBUInstruction subu_instruction(rd_register, rs_register, rt_register);
    subu_instruction.execute(*cpu);

    // Then: 驗證結果
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result)
        << "SUBU $v1, $at, $v0: 期望 " << expected_result << " 但實際得到 " << actual_result;
}

/**
 * @brief Scenario: 下溢不產生異常
 *
 * Given: 減數大於被減數的情況
 * When: 執行SUBU減法指令
 * Then: 結果應為正確的無符號值，不拋出異常
 */
TEST_F(SUBUInstructionBDDTest, UnderflowNoException)
{
    // Given: 準備下溢測試資料
    const int      rd_register     = 4;                          // $a0
    const int      rs_register     = 5;                          // $a1
    const int      rt_register     = 6;                          // $a2
    const uint32_t rs_value        = 10;                         // 較小的被減數
    const uint32_t rt_value        = 50;                         // 較大的減數
    const uint32_t expected_result = static_cast<uint32_t>(-40); // 無符號下溢結果

    // Given: 設定初始寄存器狀態
    cpu->getRegisterFile().write(rs_register, rs_value);
    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rd_register, 0);

    // When: 創建並執行SUBU指令，不應拋出異常
    mips::SUBUInstruction subu_instruction(rd_register, rs_register, rt_register);
    EXPECT_NO_THROW(subu_instruction.execute(*cpu));

    // Then: 驗證下溢結果正確
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result)
        << "SUBU下溢: 期望 " << expected_result << " 但實際得到 " << actual_result;
}

/**
 * @brief Scenario: 零值減法恆等
 *
 * Given: 任意數值和零的減法
 * When: 執行SUBU減法指令
 * Then: 結果應等於被減數
 */
TEST_F(SUBUInstructionBDDTest, ZeroSubtractionIdentity)
{
    // Given: 準備零值減法測試
    const int      rd_register     = 7;        // $a3
    const int      rs_register     = 8;        // $t0
    const int      rt_register     = 9;        // $t1
    const uint32_t rs_value        = 12345;    // 任意被減數
    const uint32_t rt_value        = 0;        // 零減數
    const uint32_t expected_result = rs_value; // 期望結果等於被減數

    // Given: 設定初始寄存器狀態
    cpu->getRegisterFile().write(rs_register, rs_value);
    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rd_register, 0xAAAAAAAA);

    // When: 執行SUBU指令
    mips::SUBUInstruction subu_instruction(rd_register, rs_register, rt_register);
    subu_instruction.execute(*cpu);

    // Then: 驗證恆等結果
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result)
        << "SUBU零值恆等: 期望 " << expected_result << " 但實際得到 " << actual_result;
}

/**
 * @brief Scenario: 相同值減法為零
 *
 * Given: 兩個相同的寄存器值
 * When: 執行SUBU減法指令
 * Then: 結果應為零
 */
TEST_F(SUBUInstructionBDDTest, SameValueSubtractionZero)
{
    // Given: 準備相同值測試資料
    const int      rd_register     = 10;         // $t2
    const int      rs_register     = 11;         // $t3
    const int      rt_register     = 12;         // $t4
    const uint32_t same_value      = 0x12345678; // 相同的值
    const uint32_t expected_result = 0;          // 期望結果為零

    // Given: 設定相同的初始值
    cpu->getRegisterFile().write(rs_register, same_value);
    cpu->getRegisterFile().write(rt_register, same_value);
    cpu->getRegisterFile().write(rd_register, 0xFFFFFFFF);

    // When: 執行SUBU指令
    mips::SUBUInstruction subu_instruction(rd_register, rs_register, rt_register);
    subu_instruction.execute(*cpu);

    // Then: 驗證結果為零
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result)
        << "SUBU相同值減法: 期望 " << expected_result << " 但實際得到 " << actual_result;
}

} // namespace mips
