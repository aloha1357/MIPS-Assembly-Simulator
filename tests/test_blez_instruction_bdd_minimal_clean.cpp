/**
 * @file test_blez_instruction_bdd_minimal.cpp
 * @brief BLEZ指令的BDD測試場景 - Phase 3.1 Red-Light Phase
 * 
 * BLEZ (Branch Less than or Equal Zero) - I-type 指令
 * Opcode: 0x06, 語法: blez $rs, offset
 * 功能: 如果 $rs ≤ 0 則分支
 * 
 * Phase 3.1 目標: 建立4個DISABLED_測試，確認紅燈狀態
 */

#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Instruction.h"
#include "../src/Assembler.h"
#include "../src/InstructionDecoder.h"
#include <memory>
#include <climits>

/**
 * @brief BLEZ指令BDD測試fixture
 * 
 * 使用Given-When-Then結構實現BDD場景
 */
class BLEZInstructionBDDTest : public ::testing::Test {
protected:
    std::unique_ptr<mips::Cpu> cpu;
    std::unique_ptr<mips::Assembler> assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;
    
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder = std::make_unique<mips::InstructionDecoder>();
        
        // 清除所有暫存器
        for (int i = 0; i < 32; ++i) {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }
    
    void TearDown() override {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }
    
    // BDD Helper方法
    void given_register_contains(const std::string& reg, uint32_t value) {
        int regNum = parseRegisterName(reg);
        cpu->getRegisterFile().write(regNum, value);
    }

    void given_program_counter(uint32_t pc) {
        cpu->setProgramCounter(pc);
    }

    void when_program_executed_for_cycles(const std::string& assembly, int cycles) {
        auto instructions = assembler->assemble(assembly);
        ASSERT_FALSE(instructions.empty()) << "Failed to assemble: " << assembly;
        
        // 執行指定週期數  
        for (int i = 0; i < cycles && i < static_cast<int>(instructions.size()); ++i) {
            instructions[i]->execute(*cpu);
        }
    }

    void then_program_counter_should_be(uint32_t expected_pc) {
        EXPECT_EQ(cpu->getProgramCounter(), expected_pc);
    }

    void then_no_branch_should_occur(uint32_t original_pc) {
        // 無分支時，PC 應該 +4
        EXPECT_EQ(cpu->getProgramCounter(), original_pc + 4);
    }

    void then_branch_should_occur(uint32_t original_pc, int offset) {
        // 分支時，PC = (original_pc + 4) + (offset << 2)
        uint32_t expected_pc = (original_pc + 4) + static_cast<uint32_t>(offset << 2);
        EXPECT_EQ(cpu->getProgramCounter(), expected_pc);
    }
    
private:
    int parseRegisterName(const std::string& reg) {
        if (reg == "$t0") return 8;
        if (reg == "$t1") return 9;
        if (reg == "$t2") return 10;
        if (reg == "$s0") return 16;
        return 0;
    }
};

// ============================================================================
// Phase 3.1: BDD Scenarios - 紅燈階段 (DISABLED_前綴，應該失敗)
// ============================================================================

/**
 * @brief Scenario 1: BLEZ正數測試 (不應分支)
 * 
 * Given register $t0 contains positive value 5
 * When  I execute "blez $t0, 4"
 * Then  no branch should occur (PC = original_pc + 4)
 */
DISABLED_TEST_F(BLEZInstructionBDDTest, PositiveValueNoBranch) {
    // Given: 暫存器包含正數值
    given_register_contains("$t0", 5);
    given_program_counter(0x1000);
    
    // When: 執行 blez $t0, 4
    when_program_executed_for_cycles("blez $t0, 4", 1);
    
    // Then: 應該不分支，PC = 0x1000 + 4 = 0x1004
    then_no_branch_should_occur(0x1000);
}

/**
 * @brief Scenario 2: BLEZ零值測試 (應該分支)
 * 
 * Given register $t1 contains zero
 * When  I execute "blez $t1, 8"
 * Then  branch should occur
 */
DISABLED_TEST_F(BLEZInstructionBDDTest, ZeroValueBranch) {
    // Given: 暫存器包含零
    given_register_contains("$t1", 0);
    given_program_counter(0x2000);
    
    // When: 執行 blez $t1, 8
    when_program_executed_for_cycles("blez $t1, 8", 1);
    
    // Then: 應該分支，PC = (0x2000 + 4) + (8 << 2) = 0x2004 + 32 = 0x2024
    then_branch_should_occur(0x2000, 8);
}

/**
 * @brief Scenario 3: BLEZ負數測試 (應該分支)
 * 
 * Given register $t2 contains negative value -10
 * When  I execute "blez $t2, -4"
 * Then  branch should occur (backward branch)
 */
DISABLED_TEST_F(BLEZInstructionBDDTest, NegativeValueBranch) {
    // Given: 暫存器包含負數
    given_register_contains("$t2", static_cast<uint32_t>(-10));
    given_program_counter(0x3000);
    
    // When: 執行 blez $t2, -4 (負偏移，向後分支)
    when_program_executed_for_cycles("blez $t2, -4", 1);
    
    // Then: 應該分支，PC = (0x3000 + 4) + (-4 << 2) = 0x3004 - 16 = 0x2FF4
    then_branch_should_occur(0x3000, -4);
}

/**
 * @brief Scenario 4: BLEZ邊界值測試 (INT_MIN應該分支)
 * 
 * Given register $s0 contains INT_MIN
 * When  I execute "blez $s0, 16"
 * Then  branch should occur
 */
DISABLED_TEST_F(BLEZInstructionBDDTest, MinIntValueBranch) {
    // Given: 暫存器包含最小負數
    given_register_contains("$s0", static_cast<uint32_t>(INT_MIN));
    given_program_counter(0x4000);
    
    // When: 執行 blez $s0, 16
    when_program_executed_for_cycles("blez $s0, 16", 1);
    
    // Then: 應該分支，PC = (0x4000 + 4) + (16 << 2) = 0x4004 + 64 = 0x4044
    then_branch_should_occur(0x4000, 16);
}

// ============================================================================
// Phase 3.1: 框架驗證測試 (這些應該通過)
// ============================================================================

/**
 * @brief 框架測試：驗證BDD helper方法正常運作
 */
TEST_F(BLEZInstructionBDDTest, Framework_BddHelpers_ShouldWork) {
    // 測試Given helper
    given_register_contains("$t0", 0xDEADBEEF);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xDEADBEEF);
    
    // 測試PC設定
    given_program_counter(0x1000);
    EXPECT_EQ(cpu->getProgramCounter(), 0x1000);
}

/**
 * @brief 框架測試：驗證CPU初始狀態
 */
TEST_F(BLEZInstructionBDDTest, Framework_CpuInitialization_ShouldBeClean) {
    EXPECT_EQ(cpu->getProgramCounter(), 0);
    EXPECT_EQ(cpu->getRegisterFile().read(0), 0);  // $zero
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0);  // $t0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);  // $t1
}

// ============================================================================
// 注意：DISABLED_前綴用於暫停失敗的BDD場景
// 在Phase 3.2實作完成後，移除DISABLED_前綴啟用測試
// ============================================================================
