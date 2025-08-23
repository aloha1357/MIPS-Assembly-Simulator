/**
 * @file test_addu_instruction_bdd_minimal.cpp
 * @brief Phase 4.3 BDD測試 - ADDU指令 (Add Unsigned) 最小化測試場景
 * 
 * 開發方法論: 嚴格BDD Red-Light → Green-Light → Integration
 * 遵循"最小增量原則，一次實作一個 Scenario"
 * 
 * ADDU指令規格:
 * - R-type指令，Function Code: 0x21
 * - 語法: addu $rd, $rs, $rt
 * - 功能: rd = rs + rt (無溢位檢查)
 * - 特點: 不會產生算術異常，即使結果發生溢位
 */

#include <gtest/gtest.h>
#include "../src/Instruction.h"
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Assembler.h"
#include "../src/InstructionDecoder.h"

namespace mips {

/**
 * @brief ADDU指令BDD測試基礎類別
 * 
 * 提供Given-When-Then模式的測試基礎設施
 * 每個測試場景遵循BDD三段式結構
 */
class ADDUInstructionBDDTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder = std::make_unique<mips::InstructionDecoder>();
    }

    void TearDown() override {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }

    std::unique_ptr<mips::Cpu> cpu;
    std::unique_ptr<mips::Assembler> assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;
};

/**
 * @brief Scenario: 基本無符號加法
 * 
 * Given: 兩個正數寄存器值
 * When: 執行ADDU加法指令
 * Then: 結果寄存器應包含正確的和值
 */
TEST_F(ADDUInstructionBDDTest, BasicUnsignedAddition) {
    // Given: 準備測試資料
    const int rd_register = 3;  // $v1
    const int rs_register = 1;  // $at  
    const int rt_register = 2;  // $v0
    const uint32_t rs_value = 100;     // 加數1
    const uint32_t rt_value = 50;      // 加數2
    const uint32_t expected_result = 150; // 預期結果

    // Given: 設定初始寄存器狀態
    cpu->getRegisterFile().write(rs_register, rs_value);
    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rd_register, 0xFFFFFFFF); // 清除目標寄存器

    // Given: 建構ADDU指令 (Function Code: 0x21)
    uint32_t instruction_encoding = (rs_register << 21) | (rt_register << 16) | (rd_register << 11) | 0x21;
    (void)instruction_encoding; // Suppress unused variable warning
    
    // When: 創建並執行ADDU指令
    mips::ADDUInstruction addu_instruction(rd_register, rs_register, rt_register);
    addu_instruction.execute(*cpu);

    // Then: 驗證結果
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result) 
        << "ADDU $v1, $at, $v0: 期望 " << expected_result 
        << " 但實際得到 " << actual_result;
}

/**
 * @brief Scenario: 溢位不產生異常
 * 
 * Given: 兩個大數值相加會導致溢位的情況  
 * When: 執行ADDU加法指令
 * Then: 結果應為正確的無符號值，不拋出異常
 */
TEST_F(ADDUInstructionBDDTest, OverflowNoException) {
    // Given: 準備溢位測試資料
    const int rd_register = 4;  // $a0
    const int rs_register = 5;  // $a1
    const int rt_register = 6;  // $a2
    const uint32_t rs_value = 0xFFFFFFFF;    // 最大32位無符號數
    const uint32_t rt_value = 1;             // 加1將導致溢位
    const uint32_t expected_result = 0;      // 溢位後回繞為0

    // Given: 設定初始寄存器狀態
    cpu->getRegisterFile().write(rs_register, rs_value);
    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rd_register, 0x12345678);

    // When: 創建並執行ADDU指令，不應拋出異常
    mips::ADDUInstruction addu_instruction(rd_register, rs_register, rt_register);
    EXPECT_NO_THROW(addu_instruction.execute(*cpu));

    // Then: 驗證溢位結果正確
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result)
        << "ADDU溢位: 期望 " << expected_result 
        << " 但實際得到 " << actual_result;
}

/**
 * @brief Scenario: 零值加法恆等
 * 
 * Given: 任意數值和零的加法
 * When: 執行ADDU加法指令  
 * Then: 結果應等於非零加數
 */
TEST_F(ADDUInstructionBDDTest, ZeroAdditionIdentity) {
    // Given: 準備零值加法測試
    const int rd_register = 7;  // $a3
    const int rs_register = 8;  // $t0
    const int rt_register = 9;  // $t1
    const uint32_t rs_value = 0xABCDEF12;    // 任意非零值
    const uint32_t rt_value = 0;             // 零值
    const uint32_t expected_result = rs_value; // 期望結果等於非零加數

    // Given: 設定初始寄存器狀態  
    cpu->getRegisterFile().write(rs_register, rs_value);
    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rd_register, 0x55555555);

    // When: 執行ADDU指令
    mips::ADDUInstruction addu_instruction(rd_register, rs_register, rt_register);
    addu_instruction.execute(*cpu);

    // Then: 驗證恆等結果
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result)
        << "ADDU零值恆等: 期望 " << expected_result 
        << " 但實際得到 " << actual_result;
}

/**
 * @brief Scenario: 大數值無符號加法
 * 
 * Given: 兩個大的無符號值相加
 * When: 執行ADDU加法指令
 * Then: 結果應為正確的無符號和值
 */
TEST_F(ADDUInstructionBDDTest, LargeUnsignedAddition) {
    // Given: 準備大數值測試資料
    const int rd_register = 10; // $t2
    const int rs_register = 11; // $t3  
    const int rt_register = 12; // $t4
    const uint32_t rs_value = 0x80000000;       // 2^31
    const uint32_t rt_value = 0x70000000;       // 大值
    const uint32_t expected_result = 0xF0000000; // 預期和值

    // Given: 設定初始值
    cpu->getRegisterFile().write(rs_register, rs_value);
    cpu->getRegisterFile().write(rt_register, rt_value);
    cpu->getRegisterFile().write(rd_register, 0);

    // When: 執行ADDU指令
    mips::ADDUInstruction addu_instruction(rd_register, rs_register, rt_register);
    addu_instruction.execute(*cpu);

    // Then: 驗證大數值加法結果
    uint32_t actual_result = cpu->getRegisterFile().read(rd_register);
    EXPECT_EQ(expected_result, actual_result)
        << "ADDU大數值加法: 期望 " << std::hex << expected_result 
        << " 但實際得到 " << std::hex << actual_result;
}

} // namespace mips
