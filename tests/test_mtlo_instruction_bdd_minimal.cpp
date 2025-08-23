/*
 * Phase 9.4: MTLO指令BDD測試 (Red-Light階段)
 * 
 * MTLO (Move To LO): 將指定通用暫存器內容移至LO暫存器
 * 指令格式: MTLO rs
 * 功能: LO = rs
 * 編碼: 000000 rsrss 00000 00000 00000 010011
 * 函數代碼: 0x13
 * 
 * BDD場景覆蓋:
 * 1. 基本LO寫入功能
 * 2. 零值寫入LO
 * 3. 最大值寫入LO  
 * 4. 覆蓋現有LO值
 */

#include <gtest/gtest.h>
#include <memory>
#include "../src/Instruction.h"
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"

using namespace mips;

class MTLOInstructionBDD : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<Cpu>();
        cpu->reset(); // 確保CPU處於乾淨狀態
    }

    void TearDown() override {
        cpu.reset();
    }

    std::unique_ptr<Cpu> cpu;
};

/*
 * BDD場景1: 基本暫存器到LO寫入
 * Given: $t0暫存器包含特定值，LO為零
 * When: 執行MTLO指令將$t0值移至LO
 * Then: LO應包含$t0的值，$t0保持不變
 */
TEST_F(MTLOInstructionBDD, BasicLOWrite_ShouldMoveRegisterToLO) {
    // Given: $t0暫存器包含特定值，LO為零
    std::unique_ptr<Instruction> instruction = std::make_unique<MTLOInstruction>(8); // 來源從$t0 (暫存器8)
    cpu->getRegisterFile().write(8, 0x12345678);   // $t0 = 特定值
    cpu->getRegisterFile().writeLO(0);             // LO = 0
    
    // When: 執行MTLO指令 (來源從$t0)
    instruction->execute(*cpu);
    
    // Then: LO應包含$t0的值，$t0暫存器保持不變
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0x12345678);     // LO = $t0值
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x12345678);     // $t0 保持原值
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/*
 * BDD場景2: 零值寫入LO暫存器
 * Given: $t1暫存器為零，LO包含其他值
 * When: 執行MTLO指令
 * Then: LO應變為零
 */
TEST_F(MTLOInstructionBDD, ZeroLOWrite_ShouldMoveZeroToLO) {
    // Given: $t1暫存器為零，LO包含其他值
    std::unique_ptr<Instruction> instruction = std::make_unique<MTLOInstruction>(9); // 來源從$t1 (暫存器9)
    cpu->getRegisterFile().write(9, 0);            // $t1 = 0
    cpu->getRegisterFile().writeLO(0xDEADBEEF);    // LO = 其他值
    
    // When: 執行MTLO指令 (來源從$t1)
    instruction->execute(*cpu);
    
    // Then: LO應變為零，$t1保持為零
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0);             // LO = 0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);              // $t1 = 0
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/*
 * BDD場景3: 最大值寫入LO暫存器
 * Given: $v0暫存器包含最大值，LO為零
 * When: 執行MTLO指令
 * Then: LO應為最大值
 */
TEST_F(MTLOInstructionBDD, MaxValueLOWrite_ShouldMoveMaxValueToLO) {
    // Given: $v0暫存器為最大值，LO為零
    std::unique_ptr<Instruction> instruction = std::make_unique<MTLOInstruction>(2); // 來源從$v0 (暫存器2)
    cpu->getRegisterFile().write(2, 0xFFFFFFFF);   // $v0 = 最大值
    cpu->getRegisterFile().writeLO(0);             // LO = 0
    
    // When: 執行MTLO指令 (來源從$v0)
    instruction->execute(*cpu);
    
    // Then: LO應為最大值，$v0保持最大值
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0xFFFFFFFF);    // LO = 最大值
    EXPECT_EQ(cpu->getRegisterFile().read(2), 0xFFFFFFFF);     // $v0 = 最大值
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/*
 * BDD場景4: 覆蓋現有LO值
 * Given: LO暫存器已有值，$t1暫存器包含新值
 * When: 執行MTLO指令將$t1值移至LO
 * Then: LO應被$t1的值覆蓋
 */
TEST_F(MTLOInstructionBDD, OverwriteExistingLO_ShouldReplaceLOValue) {
    // Given: LO暫存器已有值，$t1暫存器包含新值
    std::unique_ptr<Instruction> instructionT1 = std::make_unique<MTLOInstruction>(9); // 來源從$t1 (暫存器9)
    cpu->getRegisterFile().writeLO(0x11111111);    // LO原有值
    cpu->getRegisterFile().write(9, 0xABCDEF01);   // $t1 = 新值
    cpu->getRegisterFile().write(10, 0x87654321);  // $t2 = 其他值
    
    // When: 執行MTLO指令 (來源從$t1)
    instructionT1->execute(*cpu);
    
    // Then: LO暫存器應被$t1的值覆寫
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0xABCDEF01);     // LO = $t1值
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0xABCDEF01);     // $t1 保持原值
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x87654321);    // $t2 保持不變
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}
