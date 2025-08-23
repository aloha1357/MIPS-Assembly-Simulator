/*
 * Phase 9.3: MFLO指令BDD測試 (Red-Light階段)
 *
 * MFLO (Move From LO): 將LO暫存器內容移至指定的通用暫存器
 * 指令格式: MFLO rd
 * 功能: rd = LO
 * 編碼: 000000 00000 00000 rddes 00000 010010
 * 函數代碼: 0x12
 *
 * BDD場景覆蓋:
 * 1. 基本LO讀取功能
 * 2. LO為零值時的讀取
 * 3. LO為最大值時的讀取
 * 4. 不同目標暫存器的讀取
 */

#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

using namespace mips;

class MFLOInstructionBDD : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<Cpu>();
        cpu->reset(); // 確保CPU處於乾淨狀態
    }

    void TearDown() override
    {
        cpu.reset();
    }

    std::unique_ptr<Cpu> cpu;
};

/*
 * BDD場景1: 基本LO暫存器讀取
 * Given: LO暫存器包含特定值
 * When: 執行MFLO指令將LO值移至$t0
 * Then: $t0應包含LO的值，LO保持不變
 */
TEST_F(MFLOInstructionBDD, BasicLORead_ShouldMoveLOToRegister)
{
    // Given: LO暫存器包含特定值，$t0為零
    std::unique_ptr<Instruction> instruction =
        std::make_unique<MFLOInstruction>(8);   // 目標到$t0 (暫存器8)
    cpu->getRegisterFile().writeLO(0x12345678); // LO = 特定值
    cpu->getRegisterFile().write(8, 0);         // $t0 = 0

    // When: 執行MFLO指令 (目標到$t0)
    instruction->execute(*cpu);

    // Then: $t0應包含LO的值，LO暫存器保持不變
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x12345678);  // $t0 = LO值
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0x12345678); // LO 保持原值
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/*
 * BDD場景2: 零值LO暫存器讀取
 * Given: LO暫存器為零
 * When: 執行MFLO指令
 * Then: 目標暫存器應為零
 */
TEST_F(MFLOInstructionBDD, ZeroLORead_ShouldMoveZeroToRegister)
{
    // Given: LO暫存器為零，$t1包含其他值
    std::unique_ptr<Instruction> instruction =
        std::make_unique<MFLOInstruction>(9);    // 目標到$t1 (暫存器9)
    cpu->getRegisterFile().writeLO(0);           // LO = 0
    cpu->getRegisterFile().write(9, 0xDEADBEEF); // $t1 = 其他值

    // When: 執行MFLO指令 (目標到$t1)
    instruction->execute(*cpu);

    // Then: $t1應變為零，LO保持為零
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);  // $t1 = 0
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0); // LO = 0
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/*
 * BDD場景3: 最大值LO暫存器讀取
 * Given: LO暫存器包含最大值
 * When: 執行MFLO指令
 * Then: 目標暫存器應為最大值
 */
TEST_F(MFLOInstructionBDD, MaxValueLORead_ShouldMoveMaxValueToRegister)
{
    // Given: LO暫存器為最大值，$v0為零
    std::unique_ptr<Instruction> instruction =
        std::make_unique<MFLOInstruction>(2);   // 目標到$v0 (暫存器2)
    cpu->getRegisterFile().writeLO(0xFFFFFFFF); // LO = 最大值
    cpu->getRegisterFile().write(2, 0);         // $v0 = 0

    // When: 執行MFLO指令 (目標到$v0)
    instruction->execute(*cpu);

    // Then: $v0應為最大值，LO保持最大值
    EXPECT_EQ(cpu->getRegisterFile().read(2), 0xFFFFFFFF);  // $v0 = 最大值
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0xFFFFFFFF); // LO = 最大值
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}

/*
 * BDD場景4: 不同目標暫存器讀取
 * Given: LO暫存器包含值，多個暫存器有不同值
 * When: 執行MFLO指令將LO移至$t2
 * Then: 只有$t2被修改，其他暫存器保持不變
 */
TEST_F(MFLOInstructionBDD, DifferentTargetRegister_ShouldMoveOnlyToSpecifiedRegister)
{
    // Given: LO暫存器包含值，$t1和$t2有不同值
    std::unique_ptr<Instruction> instructionT2 =
        std::make_unique<MFLOInstruction>(10);    // 目標到$t2 (暫存器10)
    cpu->getRegisterFile().writeLO(0xABCDEF01);   // LO = 特定值
    cpu->getRegisterFile().write(9, 0x11111111);  // $t1 = 值1
    cpu->getRegisterFile().write(10, 0x22222222); // $t2 = 值2

    // When: 執行MFLO指令 (目標到$t2)
    instructionT2->execute(*cpu);

    // Then: 只有$t2被LO值覆蓋，$t1保持不變
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0xABCDEF01); // $t2 = LO值
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0x11111111);  // $t1 保持原值
    EXPECT_EQ(cpu->getRegisterFile().readLO(), 0xABCDEF01); // LO 保持原值
    EXPECT_EQ(cpu->getProgramCounter(), 1);
}
