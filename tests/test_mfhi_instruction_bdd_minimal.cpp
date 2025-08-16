/**
 * @file test_mfhi_instruction_bdd_minimal.cpp
 * @brief MFHI指令的BDD測試場景 - Phase 9.1 Red-Light階段
 * 
 * MFHI (Move From HI) - R-type指令
 * Function Code: 0x10
 * 語法: mfhi $rd
 * 功能: $rd = HI (將HI暫存器內容移動到通用暫存器)
 */

#include <gtest/gtest.h>
#include <memory>
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/RegisterFile.h"

using namespace mips;

class MFHIInstructionBDD : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<Cpu>();
        // 使用$t2 (register 10) 作為目標暫存器
        instruction = std::make_unique<MFHIInstruction>(10);
    }

    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<Instruction> instruction;
};

/**
 * 情境1: 基本HI暫存器讀取測試
 * Given: HI暫存器包含特定值0x12345678
 * When: 執行MFHI指令將HI值移動到$t2
 * Then: $t2應包含HI的值，且HI暫存器保持不變
 */
TEST_F(MFHIInstructionBDD, BasicHIRead_ShouldMoveHIToRegister) {
    // Given: HI暫存器包含特定值
    cpu->getRegisterFile().writeHI(0x12345678);
    cpu->getRegisterFile().write(10, 0x00000000); // $t2 初始化為0
    
    // When: 執行MFHI指令
    instruction->execute(*cpu);
    
    // Then: 目標暫存器應包含HI的值
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x12345678);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0x12345678); // HI暫存器不變
    // 程式計數器應前進
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * 情境2: 零值HI暫存器讀取測試
 * Given: HI暫存器為零
 * When: 執行MFHI指令
 * Then: 目標暫存器應為零
 */
TEST_F(MFHIInstructionBDD, ZeroHIRead_ShouldMoveZeroToRegister) {
    // Given: HI暫存器為零
    cpu->getRegisterFile().writeHI(0x00000000);
    cpu->getRegisterFile().write(10, 0xFFFFFFFF); // $t2 初始化為全1
    
    // When: 執行MFHI指令
    instruction->execute(*cpu);
    
    // Then: 目標暫存器應為零
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x00000000);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0x00000000); // HI暫存器保持零
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * 情境3: 最大值HI暫存器讀取測試
 * Given: HI暫存器包含最大32位值
 * When: 執行MFHI指令
 * Then: 目標暫存器應包含最大值
 */
TEST_F(MFHIInstructionBDD, MaxValueHIRead_ShouldMoveMaxValueToRegister) {
    // Given: HI暫存器包含最大32位值
    cpu->getRegisterFile().writeHI(0xFFFFFFFF);
    cpu->getRegisterFile().write(10, 0x00000000); // $t2 初始化為0
    
    // When: 執行MFHI指令
    instruction->execute(*cpu);
    
    // Then: 目標暫存器應包含最大值
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0xFFFFFFFF);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0xFFFFFFFF); // HI暫存器不變
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/**
 * 情境4: 不同目標暫存器測試
 * Given: 使用不同的目標暫存器($t1)
 * When: 執行MFHI指令
 * Then: 指定的暫存器應接收HI的值
 */
TEST_F(MFHIInstructionBDD, DifferentTargetRegister_ShouldMoveToCorrectRegister) {
    // Given: 設定特定的HI值，使用$t1 (register 9) 作為目標
    auto instructionT1 = std::make_unique<MFHIInstruction>(9);
    cpu->getRegisterFile().writeHI(0xABCDEF01);
    cpu->getRegisterFile().write(9, 0x12345678);  // $t1 初始值
    cpu->getRegisterFile().write(10, 0x87654321); // $t2 初始值
    
    // When: 執行MFHI指令 (目標為$t1)
    instructionT1->execute(*cpu);
    
    // Then: 只有$t1應接收HI的值，$t2保持不變
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0xABCDEF01);   // $t1 = HI值
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x87654321);  // $t2 保持原值
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0xABCDEF01);  // HI不變
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}
