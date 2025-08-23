#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

/**
 * Missing Instructions Integration Test
 * 
 * Tests all missing instructions working together
 * Following Linus's principle: "Integration tests catch the real bugs"
 */
class MissingInstructionsIntegrationTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        memory = std::make_unique<Memory>();
        registerFile = std::make_unique<RegisterFile>();
        cpu = std::make_unique<Cpu>();
    }

    std::unique_ptr<Memory> memory;
    std::unique_ptr<RegisterFile> registerFile;
    std::unique_ptr<Cpu> cpu;
};

/**
 * BDD: Given all missing instructions
 *      When executed in sequence
 *      Then they should work together correctly
 */
TEST_F(MissingInstructionsIntegrationTest, Given_AllMissingInstructions_When_ExecutedInSequence_Then_WorkTogether)
{
    // Test 1: LLO and LHI working together to build a 32-bit constant
    cpu->getRegisterFile().write(1, 0x00000000); // $at = 0
    
    // lhi $at, 0xDEAD    ; Load 0xDEAD into upper 16 bits
    LHIInstruction lhi(1, 0xDEAD);
    lhi.execute(*cpu);
    EXPECT_EQ(0xDEAD0000, cpu->getRegisterFile().read(1));
    
    // llo $at, 0xBEEF    ; Load 0xBEEF into lower 16 bits  
    LLOInstruction llo(1, 0xBEEF);
    llo.execute(*cpu);
    EXPECT_EQ(0xDEADBEEF, cpu->getRegisterFile().read(1));
    
    // Test 2: TRAP instruction
    TrapInstruction trap(0x12345);
    trap.execute(*cpu);
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find("TRAP") != std::string::npos);
    EXPECT_TRUE(output.find("74565") != std::string::npos); // 0x12345 in decimal
    
    // Test 3: Character syscalls
    // Print character 'H'
    cpu->getRegisterFile().write(2, 11);  // $v0 = print_character
    cpu->getRegisterFile().write(4, 72); // $a0 = 'H'
    SyscallInstruction printSyscall;
    printSyscall.execute(*cpu);
    
    // Print character 'i'
    cpu->getRegisterFile().write(2, 11);  // $v0 = print_character  
    cpu->getRegisterFile().write(4, 105); // $a0 = 'i'
    printSyscall.execute(*cpu);
    
    // Verify output contains "Hi"
    output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find('H') != std::string::npos);
    EXPECT_TRUE(output.find('i') != std::string::npos);
    
    // Test 4: Read character
    cpu->setConsoleInput("World!");
    cpu->getRegisterFile().write(2, 12); // $v0 = read_character
    SyscallInstruction readSyscall;
    readSyscall.execute(*cpu);
    EXPECT_EQ(87, cpu->getRegisterFile().read(2)); // 'W'
}

/**
 * Test instruction decoding for all missing instructions
 */
TEST_F(MissingInstructionsIntegrationTest, Given_AllMissingOpcodes_When_Decoded_Then_CorrectInstructionsCreated)
{
    // Test LLO decoding (opcode 0x18)
    uint32_t lloWord = 0x60011234; // opcode=0x18, rs=0, rt=1, imm=0x1234
    auto lloInstr = InstructionDecoder::decode(lloWord);
    ASSERT_NE(lloInstr, nullptr);
    EXPECT_EQ("llo", lloInstr->getName());
    
    // Test LHI decoding (opcode 0x19)  
    uint32_t lhiWord = 0x64015678; // opcode=0x19, rs=0, rt=1, imm=0x5678
    auto lhiInstr = InstructionDecoder::decode(lhiWord);
    ASSERT_NE(lhiInstr, nullptr);
    EXPECT_EQ("lhi", lhiInstr->getName());
    
    // Test TRAP decoding (opcode 0x1A)
    uint32_t trapWord = 0x68000042; // opcode=0x1A, trapcode=0x42
    auto trapInstr = InstructionDecoder::decode(trapWord);
    ASSERT_NE(trapInstr, nullptr);
    EXPECT_EQ("trap", trapInstr->getName());
}

/**
 * Test edge cases and boundary conditions
 */
TEST_F(MissingInstructionsIntegrationTest, Given_EdgeCases_When_Executed_Then_HandledCorrectly)
{
    // Edge case 1: LLO with register 0 (should not modify $zero)
    cpu->getRegisterFile().write(0, 0); // Ensure $zero is 0
    LLOInstruction lloZero(0, 0xFFFF);
    lloZero.execute(*cpu);
    EXPECT_EQ(0, cpu->getRegisterFile().read(0)); // $zero should remain 0
    
    // Edge case 2: LHI with register 0 (should not modify $zero)
    LHIInstruction lhiZero(0, 0xFFFF);
    lhiZero.execute(*cpu);
    EXPECT_EQ(0, cpu->getRegisterFile().read(0)); // $zero should remain 0
    
    // Edge case 3: Character syscall with 0 (null character)
    cpu->getRegisterFile().write(2, 11); // $v0 = print_character
    cpu->getRegisterFile().write(4, 0); // $a0 = 0 (null)
    SyscallInstruction syscall;
    syscall.execute(*cpu);
    // Should handle null character gracefully (implementation-defined)
    
    // Edge case 4: TRAP with maximum code
    TrapInstruction maxTrap(0x3FFFFFF); // Max 26-bit value
    maxTrap.execute(*cpu);
    // Should handle without crashing
}

/**
 * Performance regression test - ensure new instructions don't break existing functionality
 */
TEST_F(MissingInstructionsIntegrationTest, Given_ExistingInstructions_When_ExecutedWithNewOnes_Then_NoRegression)
{
    // Test existing ADD instruction still works
    cpu->getRegisterFile().write(8, 10);  // $t0 = 10
    cpu->getRegisterFile().write(9, 20);  // $t1 = 20
    
    // Use existing ADD instruction decoder
    uint32_t addWord = 0x01094020; // add $t0, $t0, $t1
    auto addInstr = InstructionDecoder::decode(addWord);
    ASSERT_NE(addInstr, nullptr);
    EXPECT_EQ("add", addInstr->getName());
    
    addInstr->execute(*cpu);
    EXPECT_EQ(30, cpu->getRegisterFile().read(8)); // $t0 should be 30
    
    // Test our new LLO instruction after existing instruction
    LLOInstruction llo(8, 0x1234);
    llo.execute(*cpu);
    EXPECT_EQ(0x00001234, cpu->getRegisterFile().read(8)); // Should preserve upper bits (which are 0)
}
