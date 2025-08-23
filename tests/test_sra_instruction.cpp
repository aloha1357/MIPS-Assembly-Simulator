/**
 * @file test_sra_instruction.cpp
 * @brief SRA指令的Integration測試 - 解碼器和組譯器整合測試
 *
 * 驗證以下組件的整合:
 * 1. InstructionDecoder正確解碼SRA指令 (function code 0x03)
 * 2. Assembler正確解析"sra"語法並生成機器碼
 *
 * 這些測試確保SRA指令在整個系統中的正確整合
 */

#include "Assembler.h"
#include "Instruction.h"
#include "InstructionDecoder.h"
#include <gtest/gtest.h>
#include <memory>

namespace
{

/**
 * @brief SRA指令Integration測試套件
 *
 * 測試SRA指令在解碼器和組譯器中的整合
 */
class SraInstructionIntegrationTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        decoder   = std::make_unique<mips::InstructionDecoder>();
        assembler = std::make_unique<mips::Assembler>();
    }

    void TearDown() override
    {
        decoder.reset();
        assembler.reset();
    }

    std::unique_ptr<mips::InstructionDecoder> decoder;
    std::unique_ptr<mips::Assembler>          assembler;
};

/**
 * @brief Integration測試: 驗證InstructionDecoder正確解碼SRA指令
 *
 * 測試InstructionDecoder能夠正確解碼SRA指令的機器碼
 * Function code 0x03 應該被正確識別為SRA指令
 */
TEST_F(SraInstructionIntegrationTest, SraInstruction_DecoderIntegration_ShouldDecodeCorrectly)
{
    // Given: InstructionDecoder 已創建

    // When: 解碼 SRA 指令機器碼
    // SRA $t1, $t0, 4 => R-type: op=0, rs=0, rt=8($t0), rd=9($t1), shamt=4, funct=0x03
    uint32_t machineCode = (0x00 << 26) | // opcode = 0 (R-type)
                           (0 << 21) |    // rs = 0 (not used in shift)
                           (8 << 16) |    // rt = 8 ($t0)
                           (9 << 11) |    // rd = 9 ($t1)
                           (4 << 6) |     // shamt = 4
                           0x03;          // function code for SRA

    auto instruction = decoder->decode(machineCode);

    // Then: 應該成功解碼為 SraInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼 SRA 指令";
    EXPECT_EQ(instruction->getName(), "sra") << "指令名稱應該是 'sra'";
}

/**
 * @brief Integration測試: 驗證Assembler正確解析SRA指令語法
 *
 * 測試Assembler能夠正確解析"sra $rd, $rt, shamt"語法
 * 並生成正確的SraInstruction物件
 */
TEST_F(SraInstructionIntegrationTest, SraInstruction_AssemblerIntegration_ShouldParseCorrectly)
{
    // Given: Assembler 已創建

    // When: 解析 SRA 指令語法
    std::string assembly     = "sra $t1, $t0, 4";
    auto        instructions = assembler->assemble(assembly);

    // Then: 應該成功解析為一條SRA指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "sra") << "指令名稱應該是 'sra'";
}

} // namespace
