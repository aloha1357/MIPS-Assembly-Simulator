/**
 * @file test_srl_instruction.cpp
 * @brief SRL (Shift Right Logical) Instruction Integration Tests
 * 
 * Phase 1.4: SRL Integration Tests following BDD methodology
 * Tests both decoder and assembler integration for SRL instruction
 */

#include <gtest/gtest.h>
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include "../src/Instruction.h"
#include <memory>

using namespace mips;

/**
 * @brief Integration test suite for SRL instruction
 */
class SrlInstructionIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        decoder = std::make_unique<InstructionDecoder>();
        assembler = std::make_unique<Assembler>();
    }

    void TearDown() override {
        decoder.reset();
        assembler.reset();
    }

    std::unique_ptr<InstructionDecoder> decoder;
    std::unique_ptr<Assembler> assembler;
};

/**
 * Integration Test 1: Decoder Integration
 * Verifies that SRL instruction can be decoded from binary
 */
TEST_F(SrlInstructionIntegrationTest, SrlInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // Given: Binary representation of "srl $t1, $t0, 4"
    // SRL $t1, $t0, 4 => R-type: op=0, rs=0, rt=8($t0), rd=9($t1), shamt=4, funct=0x02
    uint32_t instruction = (0x00 << 26) | (0 << 21) | (8 << 16) | (9 << 11) | (4 << 6) | 0x02;
    
    // When: Decoding the instruction
    auto decoded = decoder->decode(instruction);
    
    // Then: Should successfully decode as SRL instruction
    ASSERT_NE(decoded, nullptr) << "SRL instruction should be decoded successfully";
    EXPECT_EQ(decoded->getName(), "srl") << "Decoded instruction should be 'srl'";
}

/**
 * Integration Test 2: Assembler Integration  
 * Verifies that SRL instruction can be parsed from assembly text
 */
TEST_F(SrlInstructionIntegrationTest, SrlInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // Given: Assembly string for SRL instruction
    std::string assembly = "srl $t1, $t0, 8";
    
    // When: Parsing the assembly
    auto instructions = assembler->assemble(assembly);
    
    // Then: Should successfully parse as SRL instruction
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "srl") << "指令名稱應該是 'srl'";
}

/**
 * Integration Notes:
 * 
 * Phase 1.4 Integration Test Requirements:
 * 1. Decoder must recognize SRL function code (0x02)
 * 2. Assembler must parse "srl $rd, $rt, shamt" syntax  
 * 3. Both must create valid SrlInstruction objects
 * 
 * Dependencies verified:
 * - InstructionDecoder.cpp: Function 0x02 mapping
 * - Assembler.cpp: "srl" keyword parsing
 * - Instruction.h/cpp: SrlInstruction class implementation
 */
