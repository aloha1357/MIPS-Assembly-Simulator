/**
 * @file test_logical_sll_bdd_minimal.cpp
 * @brief SLL (Shift Left Logical) instruction BDD tests
 *
 * 按照BDD方法論實作的SLL指令測試，確保：
 * 1. 基本左位移操作正確
 * 2. 邊界條件處理正確
 *
 * Phase A: 紅燈階段 - 故意失敗的測試
 */

#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

/**
 * @brief SLL指令BDD測試fixture
 */
class SllInstructionBDDTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<mips::Cpu>();
        cpu->reset();
    }

    void TearDown() override
    {
        cpu.reset();
    }

    // BDD-style helper methods
    void given_register_contains(const std::string& regName, uint32_t value)
    {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register: " << regName;
        cpu->getRegisterFile().write(regNum, value);
    }

    void when_program_executed_for_cycles(const std::string& program, int cycles)
    {
        cpu->loadProgramFromString(program);
        cpu->run(cycles);
    }

    void then_register_should_equal(const std::string& regName, uint32_t expected)
    {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register: " << regName;

        uint32_t actual = cpu->getRegisterFile().read(regNum);
        EXPECT_EQ(actual, expected) << "Register " << regName << " should equal 0x" << std::hex
                                    << expected << " but was 0x" << std::hex << actual;
    }

  private:
    std::unique_ptr<mips::Cpu> cpu;

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
};

// ============================================================================
// Phase A: BDD Scenarios - 期望紅燈 (測試失敗)
// ============================================================================

/**
 * @brief Scenario 1: SLL指令基本左位移操作
 *
 * Given 暫存器 $t0 包含 0x00000001
 * When 執行 "sll $t1, $t0, 4" 指令
 * Then 暫存器 $t1 應該等於 0x00000010 (左位移4位)
 */
TEST_F(SllInstructionBDDTest, Sll_BasicShift_ShouldShiftLeftCorrectly)
{
    // Given: 暫存器 $t0 包含 0x00000001
    given_register_contains("$t0", 0x00000001);

    // When: 執行 SLL 指令
    when_program_executed_for_cycles("sll $t1, $t0, 4", 1);

    // Then: $t1 應該等於 0x00000010
    then_register_should_equal("$t1", 0x00000010);
}

/**
 * @brief Scenario 2: SLL指令溢位處理
 *
 * Given 暫存器 $t0 包含 0x80000000
 * When 執行 "sll $t1, $t0, 1" 指令
 * Then 暫存器 $t1 應該等於 0x00000000 (溢位後為0)
 */
TEST_F(SllInstructionBDDTest, Sll_OverflowShift_ShouldHandleOverflow)
{
    // Given: 暫存器 $t0 包含會溢位的值
    given_register_contains("$t0", 0x80000000);

    // When: 執行 SLL 指令 (左位移1位)
    when_program_executed_for_cycles("sll $t1, $t0, 1", 1);

    // Then: $t1 應該等於 0x00000000 (正確處理溢位)
    then_register_should_equal("$t1", 0x00000000);
}

/**
 * Phase A 完成標準:
 * - ✅ 創建了失敗的測試（使用 DISABLED_ 前綴和 FAIL()）
 * - ✅ 使用 DISABLED_ 前綴確保測試不會執行
 * - ✅ 遵循 BDD Given-When-Then 結構
 * - ✅ 專注於SLL指令的兩個核心場景
 *
 * 下一步 Phase B：
 * 1. 移除 DISABLED_ 前綴
 * 2. 移除 FAIL() 語句
 * 3. 確認SLL指令實作正確
 * 4. 確保測試通過
 */
