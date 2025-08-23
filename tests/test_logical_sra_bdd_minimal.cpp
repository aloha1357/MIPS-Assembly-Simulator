/**
 * @file test_logical_sra_bdd_minimal.cpp
 * @brief SRA指令的BDD測試場景 - 嚴格BDD方法論
 *
 * Phase A: 紅燈階段 - 故意失敗的測試，用於驗證SRA指令實現
 *
 * 測試場景:
 * 1. 正數算術右位移 - 驗證零擴展行為
 * 2. 負數算術右位移 - 驗證符號位擴展行為
 */

#include "Cpu.h"
#include "Instruction.h"
#include "RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

namespace
{

/**
 * @brief SRA指令BDD測試套件
 *
 * 遵循Given-When-Then模式的行為驅動開發測試
 */
class SraInstructionBDDTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<mips::Cpu>();

        // 重設CPU狀態
        for (int i = 0; i < 32; ++i)
        {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }

    void TearDown() override
    {
        cpu.reset();
    }

    /**
     * @brief Given: 設定暫存器初始值
     */
    void given_register_contains(const std::string& reg, uint32_t value)
    {
        int regNum = getRegisterNumber(reg);
        if (regNum >= 0)
        {
            cpu->getRegisterFile().write(regNum, value);
        }
    }

    /**
     * @brief When: 執行SRA指令並運行指定週期
     */
    void when_program_executed_for_cycles(const std::string& program, int cycles)
    {
        // 解析並執行指令
        if (program.find("sra") != std::string::npos)
        {
            // Parse: sra $rd, $rt, shamt
            // 簡化解析 - 從程式字串中提取暫存器和位移量
            parseSraAndExecute(program);
        }

        // 運行指定週期數（這裡簡化為直接執行）
        for (int i = 0; i < cycles; ++i)
        {
            // 週期模擬（如果需要）
        }
    }

    /**
     * @brief Then: 驗證暫存器值是否符合預期
     */
    void then_register_should_equal(const std::string& reg, uint32_t expected)
    {
        int regNum = getRegisterNumber(reg);
        if (regNum >= 0)
        {
            uint32_t actual = cpu->getRegisterFile().read(regNum);
            EXPECT_EQ(actual, expected) << "Register " << reg << " expected: 0x" << std::hex
                                        << expected << " but got: 0x" << std::hex << actual;
        }
    }

  private:
    std::unique_ptr<mips::Cpu> cpu;

    /**
     * @brief 解析暫存器名稱並返回暫存器號碼
     */
    int getRegisterNumber(const std::string& reg)
    {
        if (reg == "$t0")
            return 8;
        if (reg == "$t1")
            return 9;
        if (reg == "$t2")
            return 10;
        if (reg == "$t3")
            return 11;
        if (reg == "$s0")
            return 16;
        if (reg == "$s1")
            return 17;
        // 可擴展更多暫存器
        return -1;
    }

    /**
     * @brief 解析並執行SRA指令
     */
    void parseSraAndExecute(const std::string& program)
    {
        // 簡化的SRA指令解析 - 實際專案中會使用Assembler
        // 假設格式: "sra $rd, $rt, shamt"

        // 硬編碼測試案例的解析 - BDD測試中的簡化做法
        if (program.find("sra $t1, $t0, 8") != std::string::npos)
        {
            // 正數測試: sra $t1, $t0, 8
            mips::SraInstruction instr(9, 8, 8); // rd=9($t1), rt=8($t0), shamt=8
            instr.execute(*cpu);
        }
        else if (program.find("sra $t1, $t0, 4") != std::string::npos)
        {
            // 負數測試: sra $t1, $t0, 4
            mips::SraInstruction instr(9, 8, 4); // rd=9($t1), rt=8($t0), shamt=4
            instr.execute(*cpu);
        }
    }
};

/**
 * @brief Scenario 1: SRA指令正數算術右位移操作
 *
 * Given 暫存器 $t0 包含 0x7FFFFFFF (正數，MSB=0)
 * When 執行 "sra $t1, $t0, 8" 指令
 * Then 暫存器 $t1 應該等於 0x007FFFFF (算術右移8位，零擴展)
 */
TEST_F(SraInstructionBDDTest, Sra_PositiveShift_ShouldZeroExtend)
{
    // Given: 暫存器包含正數值
    given_register_contains("$t0", 0x7FFFFFFF); // 最大正數

    // When: 執行SRA指令進行算術右位移8位
    when_program_executed_for_cycles("sra $t1, $t0, 8", 1);

    // Then: 結果應該是算術右移後的值(正數零擴展)
    then_register_should_equal("$t1", 0x007FFFFF); // 算術右移，MSB=0時零擴展
}

/**
 * @brief Scenario 2: SRA指令負數算術右位移操作
 *
 * Given 暫存器 $t0 包含 0x80000000 (負數，MSB=1)
 * When 執行 "sra $t1, $t0, 4" 指令
 * Then 暫存器 $t1 應該等於 0xF8000000 (算術右移4位，符號位擴展)
 */
TEST_F(SraInstructionBDDTest, Sra_NegativeShift_ShouldSignExtend)
{
    // Given: 暫存器包含負數值(MSB為1)
    given_register_contains("$t0", 0x80000000); // 最小負數

    // When: 執行SRA指令進行算術右位移4位
    when_program_executed_for_cycles("sra $t1, $t0, 4", 1);

    // Then: 結果應該是算術右移後的值(負數符號位擴展)
    then_register_should_equal("$t1", 0xF8000000); // 算術右移，MSB=1時符號位擴展
}

} // namespace
