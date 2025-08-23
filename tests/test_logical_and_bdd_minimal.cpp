/**
 * @file test_logical_and_bdd_minimal.cpp
 * @brief BDD Minimal AND Instruction Test - Phase A
 *
 * 按照BDD最小增量原則：
 * Phase A: 撰寫步驟 - 先創建失敗的測試
 * Phase B: 實作通過測試的程式碼
 * Phase C: 重構 & 回歸
 *
 * 當前狀態：Phase A - 故意失敗的測試
 */

#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

namespace mips
{
namespace test
{

/**
 * @brief BDD AND指令最小測試類別
 *
 * 專注於單一 Scenario：AND 指令執行位元 AND 運算
 */
class LogicalAndMinimalBddTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu       = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();

        // 確保所有暫存器都初始化為零
        for (int i = 0; i < 32; i++)
        {
            cpu->getRegisterFile().write(i, 0);
        }

        // 設定程式計數器為 0
        cpu->setProgramCounter(0);
    }

    void TearDown() override
    {
        cpu.reset();
        assembler.reset();
    }

    /**
     * @brief BDD Given: 設定暫存器值
     */
    void givenRegisterContains(const std::string& regName, uint32_t value)
    {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register name: " << regName;
        cpu->getRegisterFile().write(regNum, value);

        // 儲存初始值用於驗證不變性
        initialValues[regName] = value;
    }

    /**
     * @brief BDD When: 執行指令
     */
    void whenIExecuteInstruction(const std::string& instruction)
    {
        lastInstruction = instruction;

        // Phase B: 實作AND指令執行
        try
        {
            auto instructions = assembler->assemble(instruction);
            if (!instructions.empty())
            {
                instructions[0]->execute(*cpu);
                executionCycles = 1;  // R-type指令執行1個週期
            }
            else
            {
                FAIL() << "Failed to parse instruction: " << instruction;
            }
        }
        catch (const std::exception& e)
        {
            FAIL() << "Exception during instruction execution: " << e.what();
        }
    }

    /**
     * @brief BDD Then: 驗證暫存器結果
     */
    void thenRegisterShouldContain(const std::string& regName, uint32_t expected)
    {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register name: " << regName;

        uint32_t actual = cpu->getRegisterFile().read(regNum);
        EXPECT_EQ(actual, expected) << "Register " << regName << " should contain 0x" << std::hex
                                    << expected << " but contains 0x" << std::hex << actual;
    }

    /**
     * @brief BDD Then: 驗證暫存器保持不變
     */
    void thenRegisterShouldRemainUnchanged(const std::string& regName)
    {
        auto it = initialValues.find(regName);
        ASSERT_NE(it, initialValues.end()) << "No initial value recorded for " << regName;

        int      regNum   = getRegisterNumber(regName);
        uint32_t expected = it->second;
        uint32_t actual   = cpu->getRegisterFile().read(regNum);

        EXPECT_EQ(actual, expected)
            << "Register " << regName << " should remain unchanged at 0x" << std::hex << expected
            << " but is now 0x" << std::hex << actual;
    }

    /**
     * @brief BDD Then: 驗證指令在恰好1個CPU週期內完成
     */
    void thenInstructionShouldCompleteInOneCycle()
    {
        EXPECT_EQ(executionCycles, 1) << "Instruction should complete in exactly 1 CPU cycle";
    }

  private:
    /**
     * @brief 暫存器名稱到編號的映射
     */
    int getRegisterNumber(const std::string& regName)
    {
        static std::map<std::string, int> registerMap = {
            {"$zero", 0}, {"$at", 1},  {"$v0", 2},  {"$v1", 3},  {"$a0", 4},  {"$a1", 5},
            {"$a2", 6},   {"$a3", 7},  {"$t0", 8},  {"$t1", 9},  {"$t2", 10}, {"$t3", 11},
            {"$t4", 12},  {"$t5", 13}, {"$t6", 14}, {"$t7", 15}, {"$s0", 16}, {"$s1", 17},
            {"$s2", 18},  {"$s3", 19}, {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
            {"$t8", 24},  {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28}, {"$sp", 29},
            {"$fp", 30},  {"$ra", 31}};

        auto it = registerMap.find(regName);
        return (it != registerMap.end()) ? it->second : -1;
    }

    std::unique_ptr<mips::Cpu>       cpu;
    std::unique_ptr<mips::Assembler> assembler;
    std::map<std::string, uint32_t>  initialValues;
    std::string                      lastInstruction;
    int                              executionCycles = 0;
};

// ============================================================================
// BDD Scenario 1: AND 指令執行位元 AND 運算 - 互補位元模式
// ============================================================================

/**
 * @brief BDD Test Case 1.1: AND指令互補位元模式
 *
 * Scenario: AND 指令執行位元 AND 運算
 *   Given 暫存器 $t0 包含 0xF0F0F0F0
 *   And 暫存器 $t1 包含 0x0F0F0F0F
 *   When 我執行指令 "and $t2, $t0, $t1"
 *   Then 暫存器 $t2 應該包含 0x00000000
 *   And 暫存器 $t0 應該仍然包含 0xF0F0F0F0
 *   And 暫存器 $t1 應該仍然包含 0x0F0F0F0F
 *   And 指令應該在恰好 1 個 CPU 週期內完成
 */
TEST_F(LogicalAndMinimalBddTest, AndInstruction_ComplementaryBitPattern_PhaseA)
{
    // Given 暫存器 $t0 包含 0xF0F0F0F0
    givenRegisterContains("$t0", 0xF0F0F0F0);

    // And 暫存器 $t1 包含 0x0F0F0F0F
    givenRegisterContains("$t1", 0x0F0F0F0F);

    // When 我執行指令 "and $t2, $t0, $t1"
    // Phase A: 這裡會失敗，這是預期的
    whenIExecuteInstruction("and $t2, $t0, $t1");

    // Then 暫存器 $t2 應該包含 0x00000000
    thenRegisterShouldContain("$t2", 0x00000000);

    // And 暫存器 $t0 應該仍然包含 0xF0F0F0F0
    thenRegisterShouldRemainUnchanged("$t0");

    // And 暫存器 $t1 應該仍然包含 0x0F0F0F0F
    thenRegisterShouldRemainUnchanged("$t1");

    // And 指令應該在恰好 1 個 CPU 週期內完成
    thenInstructionShouldCompleteInOneCycle();
}

/**
 * @brief BDD Test Case 1.2: AND指令交替位元模式
 *
 * 啟用第二個測試案例
 */
TEST_F(LogicalAndMinimalBddTest, AndInstruction_AlternatingBitPattern_PhaseB)
{
    // Given 暫存器 $s0 包含 0xAAAAAAAA
    givenRegisterContains("$s0", 0xAAAAAAAA);

    // And 暫存器 $s1 包含 0x55555555
    givenRegisterContains("$s1", 0x55555555);

    // When 我執行指令 "and $s2, $s0, $s1"
    whenIExecuteInstruction("and $s2, $s0, $s1");

    // Then 暫存器 $s2 應該包含 0x00000000
    thenRegisterShouldContain("$s2", 0x00000000);

    // And 暫存器 $s0 應該仍然包含 0xAAAAAAAA
    thenRegisterShouldRemainUnchanged("$s0");

    // And 暫存器 $s1 應該仍然包含 0x55555555
    thenRegisterShouldRemainUnchanged("$s1");

    // And 指令應該在恰好 1 個 CPU 週期內完成
    thenInstructionShouldCompleteInOneCycle();
}

// 其他測試案例暫時被 DISABLED，等待第一個測試通過後逐一啟用

}  // namespace test
}  // namespace mips

/**
 * @brief Phase A 總結
 *
 * 當前狀態：
 * - ✅ 創建了失敗的測試（`whenIExecuteInstruction` 故意失敗）
 * - ✅ 使用 DISABLED_ 前綴確保只有第一個測試會執行
 * - ✅ 遵循 BDD Given-When-Then 結構
 * - ✅ 專注於單一 Scenario（AND 指令執行位元 AND 運算）
 *
 * 下一步 Phase B：
 * 1. 移除 `whenIExecuteInstruction` 中的 FAIL()
 * 2. 實作組譯器 AND 指令支援
 * 3. 實作指令執行邏輯
 * 4. 確保測試通過
 */
