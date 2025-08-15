/**
 * @file test_logical_nor_bdd_minimal.cpp
 * @brief NOR instruction BDD tests
 * 
 * 按照BDD方法論實作的NOR指令測試，使用Given-When-Then結構：
 * - Given: 設定初始狀態
 * - When: 執行NOR指令
 * - Then: 驗證結果
 * 
 * NOR指令執行邏輯：nor rd, rs, rt => rd = ~(rs | rt)
 */

#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/Assembler.h"
#include "../src/RegisterFile.h"
#include "../src/Instruction.h"

class LogicalNorMinimalBddTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
    }

    void TearDown() override {
        cpu.reset();
        assembler.reset();
    }

    void givenRegisterContains(int regNum, uint32_t value) {
        cpu->getRegisterFile().write(regNum, value);
    }

    void whenIExecuteInstruction(const std::string& instruction) {
        auto instructions = assembler->assemble(instruction);
        ASSERT_FALSE(instructions.empty()) << "組譯器應該能解析指令: " << instruction;
        
        auto& inst = instructions[0];
        ASSERT_NE(inst, nullptr) << "指令不應該為空";
        
        inst->execute(*cpu);
    }

    uint32_t thenRegisterShouldContain(int regNum) {
        return cpu->getRegisterFile().read(regNum);
    }

    std::unique_ptr<mips::Cpu> cpu;
    std::unique_ptr<mips::Assembler> assembler;
};

/**
 * @brief BDD Test 1: NOR指令對全零輸入的測試 (Phase A: Red Light)
 * 
 * Scenario: 執行NOR指令對全零值的運算
 *   Given $t0寄存器包含0x00000000
 *   And $t1寄存器包含0x00000000
 *   When 我執行 "nor $t2, $t0, $t1" 指令
 *   Then $t2寄存器應該包含0xFFFFFFFF (因為 ~(0 | 0) = ~0 = 0xFFFFFFFF)
 */
TEST_F(LogicalNorMinimalBddTest, NorInstruction_ZeroInputs_PhaseB) {
    // Given: 設定輸入寄存器為全零
    givenRegisterContains(8, 0x00000000);  // $t0 = 0
    givenRegisterContains(9, 0x00000000);  // $t1 = 0
    
    // When: 執行NOR指令
    whenIExecuteInstruction("nor $t2, $t0, $t1");
    
    // Then: 結果應該是全1 (~(0 | 0) = ~0 = 0xFFFFFFFF)
    uint32_t result = thenRegisterShouldContain(10);  // $t2
    EXPECT_EQ(result, 0xFFFFFFFF) << "NOR(0, 0) 應該等於 0xFFFFFFFF";
}

/**
 * @brief BDD Test 2: NOR指令對全1輸入的測試 (Phase A: Red Light)
 * 
 * Scenario: 執行NOR指令對全1值的運算
 *   Given $t0寄存器包含0xFFFFFFFF  
 *   And $t1寄存器包含0xFFFFFFFF
 *   When 我執行 "nor $t2, $t0, $t1" 指令
 *   Then $t2寄存器應該包含0x00000000 (因為 ~(0xFFFFFFFF | 0xFFFFFFFF) = ~0xFFFFFFFF = 0)
 */
TEST_F(LogicalNorMinimalBddTest, NorInstruction_AllOnesInputs_PhaseB) {
    // Given: 設定輸入寄存器為全1
    givenRegisterContains(8, 0xFFFFFFFF);  // $t0 = 0xFFFFFFFF
    givenRegisterContains(9, 0xFFFFFFFF);  // $t1 = 0xFFFFFFFF
    
    // When: 執行NOR指令
    whenIExecuteInstruction("nor $t2, $t0, $t1");
    
    // Then: 結果應該是全0 (~(0xFFFFFFFF | 0xFFFFFFFF) = ~0xFFFFFFFF = 0)
    uint32_t result = thenRegisterShouldContain(10);  // $t2
    EXPECT_EQ(result, 0x00000000) << "NOR(0xFFFFFFFF, 0xFFFFFFFF) 應該等於 0x00000000";
}
