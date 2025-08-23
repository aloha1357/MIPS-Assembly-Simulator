/**
 * @file test_logical_srl_bdd_minimal.cpp
 * @brief BDD Tests for SRL (Shift Right Logical) Instruction
 *
 * Phase 1.3: SRL Instruction Group BDD Implementation
 * Following strict BDD methodology with Given-When-Then structure
 *
 * SRL performs logical right shift (zero-fill from left)
 * Format: srl $rd, $rt, shamt
 */

#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>
#include <map>
#include <memory>

class SrlInstructionBDDTest : public ::testing::Test
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

    // BDD Helper Methods Following Given-When-Then Pattern
    void given_register_contains(const std::string& reg, uint32_t value)
    {
        int regNum = getRegisterNumber(reg);
        cpu->getRegisterFile().write(regNum, value);
    }

    void when_program_executed_for_cycles(const std::string& assembly, int cycles)
    {
        // Load and execute the assembly instruction
        cpu->loadProgramFromString(assembly);
        cpu->run(cycles);
    }

    void then_register_should_equal(const std::string& reg, uint32_t expected)
    {
        int regNum = getRegisterNumber(reg);
        ASSERT_NE(regNum, -1) << "Invalid register: " << reg;

        uint32_t actual = cpu->getRegisterFile().read(regNum);
        EXPECT_EQ(expected, actual) << "Register " << reg << " expected: 0x" << std::hex << expected
                                    << " but got: 0x" << std::hex << actual;
    }

  private:
    std::unique_ptr<mips::Cpu> cpu;

    int getRegisterNumber(const std::string& reg)
    {
        static std::map<std::string, int> registerMap = {
            {"$zero", 0}, {"$at", 1},  {"$v0", 2},  {"$v1", 3},  {"$a0", 4},  {"$a1", 5},
            {"$a2", 6},   {"$a3", 7},  {"$t0", 8},  {"$t1", 9},  {"$t2", 10}, {"$t3", 11},
            {"$t4", 12},  {"$t5", 13}, {"$t6", 14}, {"$t7", 15}, {"$s0", 16}, {"$s1", 17},
            {"$s2", 18},  {"$s3", 19}, {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
            {"$t8", 24},  {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28}, {"$sp", 29},
            {"$fp", 30},  {"$ra", 31}};

        auto it = registerMap.find(reg);
        return (it != registerMap.end()) ? it->second : -1;
    }
};

// ============================================================================
// BDD Test Scenarios for SRL Instruction
// ============================================================================

TEST_F(SrlInstructionBDDTest, Srl_BasicShift_ShouldShiftRightCorrectly)
{
    // Scenario: Basic right logical shift operation
    // Given: 暫存器包含要右移的值
    given_register_contains("$t0", 0x80000000);  // MSB set

    // When: 執行SRL指令進行右位移4位
    when_program_executed_for_cycles("srl $t1, $t0, 4", 1);

    // Then: 結果應該是右位移後的值(左側補零)
    then_register_should_equal("$t1", 0x08000000);  // 邏輯右移，左側補零
}

TEST_F(SrlInstructionBDDTest, Srl_ZeroFillShift_ShouldFillWithZeros)
{
    // Scenario: Verify zero-fill behavior during right shift
    // Given: 暫存器包含負數(MSB為1)
    given_register_contains("$t0", 0xFF000000);  // High bits set

    // When: 執行SRL指令進行右位移8位
    when_program_executed_for_cycles("srl $t1, $t0, 8", 1);

    // Then: 應該從左側補零而非符號位延展
    then_register_should_equal("$t1", 0x00FF0000);  // 邏輯右移，左側補零
}

/**
 * BDD Implementation Notes:
 *
 * Phase 1.3.1 (Red): DISABLED tests with FAIL()
 * Phase 1.3.2 (Green): Enable tests, implement actual BDD scenarios
 * Phase 1.3.3 (Refactor): Clean up and optimize
 *
 * Expected SRL behaviors to test:
 * 1. Basic right shift: 0x80000000 >> 1 = 0x40000000
 * 2. Zero fill: 0xFF000000 >> 4 = 0x0FF00000 (not sign extension)
 * 3. Edge cases: shift by 0, shift by 31
 *
 * This follows the same pattern as SLL BDD tests but for right logical shift
 */
