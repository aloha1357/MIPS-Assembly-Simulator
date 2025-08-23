/**
 * @file test_sltiu_instruction.cpp
 * @brief TDD Tests for SLTIU Instruction - Phase A: Red Light
 * 
 * SLTIU (Set Less Than Immediate Unsigned) - I-type 指令
 * 功能碼: 0x0B
 * 格式: sltiu $rt, $rs, immediate
 * 功能: rt = (rs < sign_extend(immediate)) ? 1 : 0 (無符號比較)
 * 
 * 按照 BDD 最小增量原則，先建立失敗的測試
 */

#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include <memory>

/**
 * @brief SLTIU Instruction Test Fixture
 */
class SltiuInstructionTest : public ::testing::Test {
protected:
    std::unique_ptr<mips::Cpu> cpu;
    
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        
        // 設定CPU為乾淨狀態
        for (int i = 0; i < 32; ++i) {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
    }
    
    void TearDown() override {
        cpu.reset();
    }
};

// ============================================================================
// Phase A: 紅燈測試 - SLTIU 指令（預期失敗）
// ============================================================================

/**
 * @brief Test 1: SLTIU 指令基本功能 - 小於立即值
 * 期望失敗：SltiuInstruction 類別尚未實作
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_BasicLessThan_ShouldSetOne) {
    // Arrange: 5 < 10 (unsigned) 應該回傳 1
    cpu->getRegisterFile().write(8, 5);   // $t0 = 5
    
    // Act: 執行 sltiu $t1, $t0, 10
    // 這裡應該會編譯失敗，因為 SltiuInstruction 還沒宣告
    mips::SltiuInstruction instr(9, 8, 10);  // sltiu $t1, $t0, 10
    instr.execute(*cpu);
    
    // Assert: 5 < 10 is true (unsigned), so result should be 1
    EXPECT_EQ(cpu->getRegisterFile().read(9), 1);
    
    // 確認源暫存器沒有被修改
    EXPECT_EQ(cpu->getRegisterFile().read(8), 5);
}

/**
 * @brief Test 2: SLTIU 指令 - 大於立即值
 * 期望失敗：SltiuInstruction::execute 方法尚未實作
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_BasicGreaterThan_ShouldSetZero) {
    // Arrange: 15 < 10 (unsigned) 應該回傳 0
    cpu->getRegisterFile().write(8, 15);  // $t0 = 15
    
    // Act: 執行 sltiu $t1, $t0, 10
    mips::SltiuInstruction instr(9, 8, 10);
    instr.execute(*cpu);
    
    // Assert: 15 < 10 is false, so result should be 0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 15);
}

/**
 * @brief Test 3: SLTIU 指令 - 相等情況
 * 期望失敗：需要正確處理相等值比較
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_EqualValues_ShouldSetZero) {
    // Arrange: 42 < 42 (unsigned) 應該回傳 0
    cpu->getRegisterFile().write(8, 42);  // $t0 = 42
    
    // Act: 執行 sltiu $t1, $t0, 42
    mips::SltiuInstruction instr(9, 8, 42);
    instr.execute(*cpu);
    
    // Assert: 42 < 42 is false, so result should be 0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);
}

/**
 * @brief Test 4: SLTIU 指令 - 負數暫存器值與正立即值比較（關鍵測試）
 * 期望失敗：需要正確處理無符號比較
 * 
 * 這是 SLTIU 的關鍵測試：
 * 0xFFFFFFFF 在有符號數是 -1，但在無符號數是 4294967295
 * 與正立即值比較時，應該視為無符號大數
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_UnsignedComparison_CriticalTest) {
    // Arrange: 0xFFFFFFFF vs 100 的無符號比較
    cpu->getRegisterFile().write(8, 0xFFFFFFFF);  // $t0 = 4294967295 (unsigned)
    
    // Act: 執行 sltiu $t1, $t0, 100
    mips::SltiuInstruction instr(9, 8, 100);
    instr.execute(*cpu);
    
    // Assert: 4294967295 < 100 is false (unsigned), so result should be 0
    // 注意：這與 SLTI 指令不同，SLTI 會認為 -1 < 100 是 true
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0);
}

/**
 * @brief Test 5: SLTIU 指令 - 零值比較
 * 期望失敗：邊界值測試
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_ZeroComparison_ShouldWork) {
    // Arrange: 0 vs 1 的無符號比較
    cpu->getRegisterFile().write(8, 0);  // $t0 = 0
    
    // Act: 執行 sltiu $t1, $t0, 1
    mips::SltiuInstruction instr(9, 8, 1);
    instr.execute(*cpu);
    
    // Assert: 0 < 1 is true, so result should be 1
    EXPECT_EQ(cpu->getRegisterFile().read(9), 1);
}

/**
 * @brief Test 6: SLTIU 指令 - 最大立即值測試 (16位)
 * 期望失敗：需要正確處理16位立即值的符號擴展
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_MaxImmediate_ShouldWork) {
    // Arrange: 32767 vs -1 (0xFFFF 符號擴展) 的無符號比較
    // MIPS 立即值是16位，0xFFFF 會被符號擴展為 0xFFFFFFFF (4294967295)
    cpu->getRegisterFile().write(8, 32767);  // $t0 = 32767
    
    // Act: 執行 sltiu $t1, $t0, -1 (0xFFFF)
    // -1 會被符號擴展為 0xFFFFFFFF (4294967295)
    mips::SltiuInstruction instr(9, 8, -1);
    instr.execute(*cpu);
    
    // Assert: 32767 < 4294967295 is true, so result should be 1
    EXPECT_EQ(cpu->getRegisterFile().read(9), 1);
}

/**
 * @brief Test 7: SLTIU 指令 - 程式計數器遞增測試
 * 期望失敗：PC 應該在指令執行後遞增
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_ProgramCounter_ShouldIncrement) {
    // Arrange
    cpu->getRegisterFile().write(8, 100);  // $t0 = 100
    cpu->setProgramCounter(50);            // PC = 50
    
    // Act: 執行 sltiu $t1, $t0, 200
    mips::SltiuInstruction instr(9, 8, 200);
    instr.execute(*cpu);
    
    // Assert: PC 應該遞增 1
    EXPECT_EQ(cpu->getProgramCounter(), 51);
    // 順便檢查結果正確性：100 < 200 is true
    EXPECT_EQ(cpu->getRegisterFile().read(9), 1);
}

// ============================================================================
// Integration Tests - 這些測試驗證 SLTIU 指令與整個系統的集成
// ============================================================================

/**
 * @brief Integration Test: SLTIU 指令與解碼器集成
 * 測試解碼器能正確解碼 SLTIU 指令的機器碼
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // Arrange: 建立解碼器
    mips::InstructionDecoder decoder;
    
    // SLTIU $t1, $t0, 100 的機器碼
    // opcode=0x0B, rs=$t0(8), rt=$t1(9), immediate=100
    // 格式: [opcode:6][rs:5][rt:5][immediate:16]
    uint32_t machineCode = (0x0B << 26) | (8 << 21) | (9 << 16) | (100 & 0xFFFF);
    
    // Act: 解碼指令
    auto instruction = decoder.decode(machineCode);
    
    // Assert: 驗證解碼結果
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼 SLTIU 指令";
    EXPECT_EQ(instruction->getName(), "sltiu") << "指令名稱應該是 'sltiu'";
    
    // 驗證指令可以執行而不會崩潰
    // 注意：不測試執行結果，只測試解碼器能正確識別指令類型
}

/**
 * @brief Integration Test: SLTIU 指令與彙編器集成  
 * 測試彙編器能正確解析 SLTIU 指令的彙編語法
 */
TEST_F(SltiuInstructionTest, SltiuInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // Arrange: 建立彙編器
    mips::Assembler assembler;
    
    // SLTIU 彙編語法: "sltiu $t1, $t0, 100"
    std::string assembly = "sltiu $t1, $t0, 100";
    
    // Act: 解析指令
    auto instructions = assembler.assemble(assembly);
    
    // Assert: 驗證解析結果
    ASSERT_EQ(instructions.size(), 1) << "應該解析出一個指令";
    ASSERT_NE(instructions[0], nullptr) << "彙編器應該能夠解析 SLTIU 指令";
    EXPECT_EQ(instructions[0]->getName(), "sltiu") << "指令名稱應該是 'sltiu'";
    
    // 驗證指令可以執行而不會崩潰
    // 注意：不測試執行結果，只測試彙編器能正確解析語法
}
