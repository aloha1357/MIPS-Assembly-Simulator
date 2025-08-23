/**
 * @file test_logical_ori_bdd_minimal.cpp
 * @brief ORI指令的BDD測試場景 - 嚴格BDD循環
 *
 * Phase A: 撰寫階段 (紅燈) - 故意失敗的測試
 *
 * 根據開發報告Phase 2要求：
 * - 立即值邏輯指令群組 (ORI, ANDI, XORI)
 * - 目標: 182 → 200個測試 (+18)
 * - 每個指令需要BDD測試 + Integration測試
 */

#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

/**
 * @brief ORI指令BDD測試fixture
 *
 * 使用Given-When-Then結構實現BDD場景
 */
class OriInstructionBDDTest : public ::testing::Test
{
  protected:
    std::unique_ptr<mips::Cpu>                cpu;
    std::unique_ptr<mips::Assembler>          assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;

    void SetUp() override
    {
        cpu       = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder   = std::make_unique<mips::InstructionDecoder>();

        // 清除所有暫存器
        for (int i = 0; i < 32; ++i)
        {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }

    void TearDown() override
    {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }

    // BDD Helper方法
    void given_register_contains(const std::string& reg, uint32_t value)
    {
        int regNum = parseRegisterName(reg);
        cpu->getRegisterFile().write(regNum, value);
    }

    void when_program_executed_for_cycles(const std::string& assembly, int cycles)
    {
        auto instructions = assembler->assemble(assembly);
        ASSERT_FALSE(instructions.empty()) << "組譯失敗: " << assembly;

        for (int i = 0; i < cycles && i < static_cast<int>(instructions.size()); ++i)
        {
            instructions[i]->execute(*cpu);
        }
    }

    void then_register_should_equal(const std::string& reg, uint32_t expected)
    {
        int      regNum = parseRegisterName(reg);
        uint32_t actual = cpu->getRegisterFile().read(regNum);
        EXPECT_EQ(actual, expected) << "暫存器 " << reg << " 期望值: 0x" << std::hex << expected
                                    << ", 實際值: 0x" << std::hex << actual;
    }

  private:
    int parseRegisterName(const std::string& reg)
    {
        if (reg == "$zero")
            return 0;
        if (reg == "$t0")
            return 8;
        if (reg == "$t1")
            return 9;
        if (reg == "$t2")
            return 10;
        // 簡化實現，實際應該解析所有暫存器名稱
        return 0;
    }
};

// ============================================================================
// Phase A: BDD Scenarios - 紅燈階段 (應該失敗)
// ============================================================================

/**
 * @brief Scenario 1: ORI基本立即值OR操作
 *
 * Given register $t0 contains 0xF0F00000
 * When  I execute "ori $t1, $t0, 0x0F0F"
 * Then  register $t1 should contain 0xF0F00F0F
 */
TEST_F(OriInstructionBDDTest, Ori_BasicImmediateOperation_ShouldOrCorrectly)
{
    // Given: 暫存器包含初始值
    given_register_contains("$t0", 0xF0F00000);

    // When: 執行ORI指令
    when_program_executed_for_cycles("ori $t1, $t0, 0x0F0F", 1);

    // Then: 結果應該是OR操作後的值
    then_register_should_equal("$t1", 0xF0F00F0F);

    // 移除故意失敗，進入綠燈階段
    // FAIL() << "Phase A: 故意失敗 - 需要實作ORI指令的解碼器和組譯器支援";
}

/**
 * @brief Scenario 2: ORI與零立即值操作
 *
 * Given register $t0 contains 0x12345678
 * When  I execute "ori $t1, $t0, 0x0000"
 * Then  register $t1 should contain 0x12345678 (identity operation)
 */
TEST_F(OriInstructionBDDTest, Ori_ZeroImmediate_ShouldBeIdentity)
{
    // Given: 暫存器包含測試值
    given_register_contains("$t0", 0x12345678);

    // When: 執行ORI指令與零立即值
    when_program_executed_for_cycles("ori $t1, $t0, 0x0000", 1);

    // Then: 應該保持原值 (x OR 0 = x)
    then_register_should_equal("$t1", 0x12345678);

    // 移除故意失敗，進入綠燈階段
    // FAIL() << "Phase A: 故意失敗 - 需要實作ORI指令組譯器解析";
}

// ============================================================================
// Phase A: 框架驗證測試 (這些應該通過)
// ============================================================================

/**
 * @brief 框架測試：驗證BDD helper方法正常運作
 */
TEST_F(OriInstructionBDDTest, Framework_BddHelpers_ShouldWork)
{
    // 測試Given helper
    given_register_contains("$t0", 0xDEADBEEF);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xDEADBEEF);

    // 測試Then helper
    then_register_should_equal("$t0", 0xDEADBEEF);
}

/**
 * @brief 框架測試：驗證CPU初始狀態
 */
TEST_F(OriInstructionBDDTest, Framework_CpuInitialization_ShouldBeClean)
{
    EXPECT_EQ(cpu->getProgramCounter(), 0);
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0); // $zero
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0); // $t0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0); // $t1
}

// ============================================================================
// 注意：DISABLED_前綴用於暫停失敗的BDD場景
// 在Phase B實作完成後，移除DISABLED_前綴啟用測試
// ============================================================================
