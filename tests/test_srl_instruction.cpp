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
    // Format: [6-bit op][5-bit rs][5-bit rt][5-bit rd][5-bit shamt][6-bit func]
    // SRL: op=000000, rs=00000, rt=01000($t0), rd=01001($t1), shamt=00100(4), func=000010
    uint32_t instruction = 0x00084122; // srl $t1, $t0, 4
    
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
    auto parsed = assembler->parseInstruction(assembly);
    
    // Then: Should successfully parse as SRL instruction
    ASSERT_NE(parsed, nullptr) << "SRL assembly should be parsed successfully";
    EXPECT_EQ(parsed->getName(), "srl") << "Parsed instruction should be 'srl'";
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
