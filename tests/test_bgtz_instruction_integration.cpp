/**
 * @file test_bgtz_instruction_integration.cpp
 * @brief BGTZ指令的Integration測試 - Phase 3.6 Integration Testing
 *
 * BGTZ (Branch Greater Than Zero) - I-type 指令整合測試
 * Opcode: 0x07, 語法: bgtz $rs, offset
 * 功能: 如果 $rs > 0 則分支
 *
 * Phase 3.6 目標: 6-7個整合測試，驗證解碼器+組譯器完整整合
 */

#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/RegisterFile.h"
#include <climits>
#include <gtest/gtest.h>
#include <memory>

/**
 * @brief BGTZ指令Integration測試fixture
 *
 * 測試解碼器與組譯器的完整整合功能
 */
class BGTZInstructionIntegrationTest : public ::testing::Test
{
  protected:
    std::unique_ptr<mips::Cpu>                cpu;
    std::unique_ptr<mips::Assembler>          assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;

    void SetUp() override
    {
        cpu       = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder   = std::make_unique<mips::InstructionDecoder>();

        // 清除所有暫存器
        for (int i = 0; i < 32; ++i)
        {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }

    void TearDown() override
    {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }

    // Integration Helper方法
    uint32_t encodeITypeInstruction(uint32_t opcode, uint32_t rs, uint32_t rt, int16_t immediate)
    {
        return (opcode << 26) | (rs << 21) | (rt << 16) | (static_cast<uint16_t>(immediate));
    }

    int parseRegisterNumber(const std::string& assembly)
    {
        if (assembly.find("$t0") != std::string::npos)
            return 8;
        if (assembly.find("$t1") != std::string::npos)
            return 9;
        if (assembly.find("$t2") != std::string::npos)
            return 10;
        if (assembly.find("$s0") != std::string::npos)
            return 16;
        return 0;
    }

    void validateInstructionExecution(const std::string& assembly, uint32_t initialRs,
                                      uint32_t initialPC, uint32_t expectedPC)
    {
        // 設定暫存器初始值
        int regNum = parseRegisterNumber(assembly);
        cpu->getRegisterFile().write(regNum, initialRs);
        cpu->setProgramCounter(initialPC);

        // 組譯指令
        auto instructions = assembler->assemble(assembly);
        ASSERT_FALSE(instructions.empty()) << "Failed to assemble: " << assembly;

        // 執行指令
        instructions[0]->execute(*cpu);

        // 驗證PC值
        EXPECT_EQ(cpu->getProgramCounter(), expectedPC);
    }
};

// ============================================================================
// Phase 3.6: Integration Testing Scenarios
// ============================================================================

/**
 * @brief Integration Test 1: 解碼器整合測試
 *
 * 驗證InstructionDecoder能正確解碼BGTZ機器碼
 */
TEST_F(BGTZInstructionIntegrationTest, BGTZInstruction_DecoderIntegration_ShouldDecodeCorrectly)
{
    // 測試機器碼: bgtz $t0, 8
    // opcode=0x07, rs=$t0(8), rt=0, immediate=8
    uint32_t machineCode = encodeITypeInstruction(0x07, 8, 0, 8);

    // 解碼指令
    auto instruction = decoder->decode(machineCode);

    // 驗證解碼成功
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "bgtz");

    // 驗證指令執行功能
    cpu->getRegisterFile().write(8, 10);  // $t0 = 10 (正數)
    cpu->setProgramCounter(0x1000);

    instruction->execute(*cpu);

    // 應該分支: PC = (0x1000 + 4) + (8 << 2) = 0x1004 + 32 = 0x1024
    EXPECT_EQ(cpu->getProgramCounter(), 0x1024);
}

/**
 * @brief Integration Test 2: 解碼器變體測試
 *
 * 測試不同暫存器和偏移值的解碼
 */
TEST_F(BGTZInstructionIntegrationTest, BGTZInstruction_DecoderVariants_ShouldDecodeCorrectly)
{
    // 測試多種機器碼變體
    struct TestCase
    {
        uint32_t rs;
        int16_t  offset;
        uint32_t regValue;
        bool     shouldBranch;
    };

    std::vector<TestCase> testCases = {
        {9, 4, 1, true},                             // $t1=1, offset=4, 應分支
        {10, -2, 0, false},                          // $t2=0, offset=-2, 不應分支
        {16, 16, static_cast<uint32_t>(-5), false},  // $s0=-5, offset=16, 不應分支
        {8, 12, INT_MAX, true}                       // $t0=INT_MAX, offset=12, 應分支
    };

    for (const auto& testCase : testCases)
    {
        uint32_t machineCode = encodeITypeInstruction(0x07, testCase.rs, 0, testCase.offset);
        auto     instruction = decoder->decode(machineCode);

        ASSERT_NE(instruction, nullptr);
        EXPECT_EQ(instruction->getName(), "bgtz");

        // 設定暫存器值並執行
        cpu->getRegisterFile().write(testCase.rs, testCase.regValue);
        cpu->setProgramCounter(0x2000);

        instruction->execute(*cpu);

        if (testCase.shouldBranch)
        {
            uint32_t expectedPC = 0x2000 + 4 + (testCase.offset << 2);
            EXPECT_EQ(cpu->getProgramCounter(), expectedPC)
                << "Register $" << testCase.rs << " with value " << testCase.regValue
                << " should branch";
        }
        else
        {
            EXPECT_EQ(cpu->getProgramCounter(), 0x2004)
                << "Register $" << testCase.rs << " with value " << testCase.regValue
                << " should not branch";
        }
    }
}

/**
 * @brief Integration Test 3: 組譯器整合測試
 *
 * 驗證Assembler能正確解析BGTZ語法
 */
TEST_F(BGTZInstructionIntegrationTest, BGTZInstruction_AssemblerIntegration_ShouldParseCorrectly)
{
    // 測試基本語法解析
    auto instructions = assembler->assemble("bgtz $t0, 4");

    ASSERT_FALSE(instructions.empty());
    EXPECT_EQ(instructions[0]->getName(), "bgtz");

    // 驗證組譯後的指令功能
    cpu->getRegisterFile().write(8, 5);  // $t0 = 5
    cpu->setProgramCounter(0x3000);

    instructions[0]->execute(*cpu);

    // 應該分支: PC = (0x3000 + 4) + (4 << 2) = 0x3004 + 16 = 0x3014
    EXPECT_EQ(cpu->getProgramCounter(), 0x3014);
}

/**
 * @brief Integration Test 4: 組譯器語法變體測試
 *
 * 測試各種語法變體的解析
 */
TEST_F(BGTZInstructionIntegrationTest, BGTZInstruction_AssemblerVariants_ShouldParseCorrectly)
{
    std::vector<std::string> syntaxVariants = {
        "bgtz $t1, 8",     // 基本語法
        "bgtz $t2, -4",    // 負偏移
        "bgtz $s0, 0x10",  // 十六進制偏移
        "bgtz $t0, 4"      // 正偏移 (改為非零偏移)
    };

    for (const auto& syntax : syntaxVariants)
    {
        auto instructions = assembler->assemble(syntax);

        ASSERT_FALSE(instructions.empty()) << "Failed to parse: " << syntax;
        EXPECT_EQ(instructions[0]->getName(), "bgtz") << "Wrong instruction type for: " << syntax;

        // 基本執行測試 (設定正數值確保分支)
        int regNum = parseRegisterNumber(syntax);
        cpu->getRegisterFile().write(regNum, 1);
        cpu->setProgramCounter(0x4000);

        uint32_t oldPC = cpu->getProgramCounter();
        instructions[0]->execute(*cpu);

        // 應該有分支行為 (PC != oldPC + 4)
        EXPECT_NE(cpu->getProgramCounter(), oldPC + 4)
            << "Should branch for positive value with: " << syntax;
    }
}

/**
 * @brief Integration Test 5: 組譯器錯誤語法測試
 *
 * 驗證組譯器正確處理無效語法
 */
TEST_F(BGTZInstructionIntegrationTest, BGTZInstruction_AssemblerInvalidSyntax_ShouldReturnEmpty)
{
    std::vector<std::string> invalidSyntax = {
        "bgtz",        // 缺少參數
        "bgtz $t0",    // 缺少偏移
        "bgtz 5, 8",   // 無效暫存器格式
        "bgtz $32, 4"  // 無效暫存器編號
        // 注意: "bgtz $t0, $t1" 會被當作標籤處理，所以不是真正的錯誤
    };

    for (const auto& syntax : invalidSyntax)
    {
        auto instructions = assembler->assemble(syntax);

        EXPECT_TRUE(instructions.empty()) << "Should reject invalid syntax: " << syntax;
    }
}

/**
 * @brief Integration Test 6: 端對端整合測試
 *
 * 測試完整的組譯→解碼→執行流程
 */
TEST_F(BGTZInstructionIntegrationTest, BGTZInstruction_EndToEndIntegration_ShouldWorkCorrectly)
{
    // 組譯指令
    auto instructions = assembler->assemble("bgtz $t2, -8");
    ASSERT_FALSE(instructions.empty());

    // 模擬機器碼生成與解碼 (在實際環境中會由組譯器生成機器碼)
    // 這裡手動構造對應的機器碼進行解碼驗證
    uint32_t machineCode        = encodeITypeInstruction(0x07, 10, 0, -8);  // $t2, offset=-8
    auto     decodedInstruction = decoder->decode(machineCode);

    ASSERT_NE(decodedInstruction, nullptr);
    EXPECT_EQ(decodedInstruction->getName(), "bgtz");

    // 測試兩種路徑的執行結果一致性
    // 路徑1: 組譯器生成的指令
    cpu->getRegisterFile().write(10, 3);  // $t2 = 3 (正數)
    cpu->setProgramCounter(0x5000);

    instructions[0]->execute(*cpu);
    uint32_t pc1 = cpu->getProgramCounter();

    // 路徑2: 解碼器生成的指令
    cpu->getRegisterFile().write(10, 3);  // $t2 = 3 (正數)
    cpu->setProgramCounter(0x5000);

    decodedInstruction->execute(*cpu);
    uint32_t pc2 = cpu->getProgramCounter();

    // 兩個路徑應產生相同結果
    EXPECT_EQ(pc1, pc2) << "Assembler and decoder should produce consistent results";

    // 驗證正確的分支計算: PC = (0x5000 + 4) + (-8 << 2) = 0x5004 - 32 = 0x4FE4
    EXPECT_EQ(pc1, 0x4FE4);
}

/**
 * @brief Integration Test 7: 分支條件整合驗證
 *
 * 全面測試BGTZ分支條件的正確性
 */
TEST_F(BGTZInstructionIntegrationTest, BGTZInstruction_BranchConditionIntegration_ShouldBeAccurate)
{
    struct BranchTest
    {
        uint32_t    regValue;
        bool        shouldBranch;
        std::string description;
    };

    std::vector<BranchTest> branchTests = {
        {1, true, "positive small"},
        {INT_MAX, true, "positive maximum"},
        {0, false, "zero"},
        {static_cast<uint32_t>(-1), false, "negative -1"},
        {static_cast<uint32_t>(INT_MIN), false, "negative minimum"}};

    for (const auto& test : branchTests)
    {
        // 使用組譯器路徑
        validateInstructionExecution("bgtz $t0, 4", test.regValue, 0x6000,
                                     test.shouldBranch ? 0x6014 : 0x6004);

        // 使用解碼器路徑
        uint32_t machineCode = encodeITypeInstruction(0x07, 8, 0, 4);
        auto     instruction = decoder->decode(machineCode);

        cpu->getRegisterFile().write(8, test.regValue);
        cpu->setProgramCounter(0x6000);

        instruction->execute(*cpu);

        uint32_t expectedPC = test.shouldBranch ? 0x6014 : 0x6004;
        EXPECT_EQ(cpu->getProgramCounter(), expectedPC)
            << "Branch condition failed for " << test.description << " (value=" << test.regValue
            << ")";
    }
}
