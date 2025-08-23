/**
 * @file test_mthi_instruction_bdd_minimal.cpp
 * @brief MTHI指令的BDD測試場景 - Phase 9.2 Red-Light階段
 * 
 * MTHI (Move To HI) - R-type指令
 * Function Code: 0x11
 * 語法: mthi $rs
 * 功能: HI = $rs (將通用暫存器內容移動到HI暫存器)
 */

#include <gtest/gtest.h>
#include <memory>
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/RegisterFile.h"

using namespace mips;

class MTHIInstructionBDD : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<Cpu>();
        // 使用$t2 (register 10) 作為源暫存器
        instruction = std::make_unique<MTHIInstruction>(10);
    }

    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<Instruction> instruction;
};

/**
 * 情境1: 基本HI暫存器寫入測試
 * Given: $t2暫存器包含特定值0x12345678
 * When: 執行MTHI指令將$t2值移動到HI暫存器
 * Then: HI暫存器應包含$t2的值，且$t2暫存器保持不變
 */
TEST_F(MTHIInstructionBDD, BasicHIWrite_ShouldMoveRegisterToHI) {
    // Given: $t2暫存器包含特定值
    cpu->getRegisterFile().write(10, 0x12345678); // $t2 = 0x12345678
    cpu->getRegisterFile().writeHI(0x00000000);   // HI初始化為0
    
    // When: 執行MTHI指令
    instruction->execute(*cpu);
    
    // Then: HI暫存器應包含$t2的值
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0x12345678);
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x12345678); // $t2暫存器不變
    // 程式計數器應前進
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * 情境2: 零值HI暫存器寫入測試
 * Given: $t2暫存器為零
 * When: 執行MTHI指令
 * Then: HI暫存器應為零
 */
TEST_F(MTHIInstructionBDD, ZeroHIWrite_ShouldMoveZeroToHI) {
    // Given: $t2暫存器為零
    cpu->getRegisterFile().write(10, 0x00000000);  // $t2 = 0
    cpu->getRegisterFile().writeHI(0xFFFFFFFF);    // HI初始化為全1
    
    // When: 執行MTHI指令
    instruction->execute(*cpu);
    
    // Then: HI暫存器應為零
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0x00000000);
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x00000000); // $t2暫存器保持零
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * 情境3: 最大值HI暫存器寫入測試
 * Given: $t2暫存器包含最大32位值
 * When: 執行MTHI指令
 * Then: HI暫存器應包含最大值
 */
TEST_F(MTHIInstructionBDD, MaxValueHIWrite_ShouldMoveMaxValueToHI) {
    // Given: $t2暫存器包含最大32位值
    cpu->getRegisterFile().write(10, 0xFFFFFFFF);  // $t2 = 最大值
    cpu->getRegisterFile().writeHI(0x00000000);    // HI初始化為0
    
    // When: 執行MTHI指令
    instruction->execute(*cpu);
    
    // Then: HI暫存器應包含最大值
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0xFFFFFFFF);
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0xFFFFFFFF); // $t2暫存器不變
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * 情境4: 覆寫現有HI值測試
 * Given: HI暫存器已包含值，$t1暫存器包含新值
 * When: 執行MTHI指令
 * Then: HI暫存器應被新值覆寫
 */
TEST_F(MTHIInstructionBDD, OverwriteExistingHI_ShouldReplaceHIValue) {
    // Given: HI暫存器已有值，$t1暫存器包含新值
    std::unique_ptr<Instruction> instructionT1 = std::make_unique<MTHIInstruction>(9); // 從$t1 (暫存器9)
    cpu->getRegisterFile().writeHI(0x11111111);    // HI原有值
    cpu->getRegisterFile().write(9, 0xABCDEF01);   // $t1 = 新值
    cpu->getRegisterFile().write(10, 0x87654321);  // $t2 = 其他值
    
    // When: 執行MTHI指令 (目標從$t1)
    instructionT1->execute(*cpu);
    
    // Then: HI暫存器應被$t1的值覆寫
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0xABCDEF01);     // HI = $t1值
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0xABCDEF01);     // $t1 保持原值
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x87654321);    // $t2 保持不變
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}
