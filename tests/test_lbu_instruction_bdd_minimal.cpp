#include <gtest/gtest.h>
#include <cstdint>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Memory.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Assembler.h"
#include <memory>

class LBUInstructionBDDTest : public ::testing::Test {
protected:
    std::unique_ptr<mips::Cpu> cpu;
    std::unique_ptr<mips::Assembler> assembler;
    std::unique_ptr<mips::InstructionDecoder> decoder;
    
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        assembler = std::make_unique<mips::Assembler>();
        decoder = std::make_unique<mips::InstructionDecoder>();
        
        // Clear all registers
        for (int i = 0; i < 32; ++i) {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
        cpu->getMemory().reset();
        
        // Initialize test data in memory  
        cpu->getMemory().writeWord(1000, 0x12345678);  // Test data at address 1000
        cpu->getMemory().writeWord(1004, 0x9ABCDEF0);  // Test data at address 1004
    }

    void TearDown() override {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }
};

// Phase 7.3 LBU (Load Byte Unsigned) - Green Light Tests
TEST_F(LBUInstructionBDDTest, LBU_BasicLoadUnsigned_ShouldZeroExtend) {
    // Given: Memory contains 0x78 at byte address 1000 (little-endian: LSB of 0x12345678)
    cpu->getRegisterFile().write(1, 1000);  // Base address
    
    // When: LBU $t0, 0($t1) - Load byte unsigned from address 1000+0 into $t0
    mips::InstructionDecoder decoder;
    uint32_t lbu_instruction = 0x90280000;  // LBU opcode=0x24, rs=$t1(9), rt=$t0(8), offset=0
    auto instruction = decoder.decode(lbu_instruction);
    
    // Then: $t0 should contain 0x00000078 (zero-extended)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00000078);
}

TEST_F(LBUInstructionBDDTest, LBU_HighBitSet_ShouldZeroExtend) {
    // Given: Memory contains 0x90 at byte address 1004 (MSB has high bit set)
    cpu->getRegisterFile().write(1, 1004);  // Base address
    
    // When: LBU $t0, 0($t1) - Load byte unsigned from address 1004+0
    mips::InstructionDecoder decoder;
    uint32_t lbu_instruction = 0x90280000;  // LBU rs=$t1(9), rt=$t0(8), offset=0
    auto instruction = decoder.decode(lbu_instruction);
    
    // Then: $t0 should contain 0x000000F0 (zero-extended, NOT sign-extended)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x000000F0);
}

TEST_F(LBUInstructionBDDTest, LBU_PositiveOffset_ShouldCalculateAddress) {
    // Given: Memory setup with test data
    cpu->getRegisterFile().write(1, 996);  // Base address
    
    // When: LBU $t0, 4($t1) - Load from address 996+4=1000
    mips::InstructionDecoder decoder;
    uint32_t lbu_instruction = 0x90280004;  // LBU rs=$t1(9), rt=$t0(8), offset=4
    auto instruction = decoder.decode(lbu_instruction);
    
    // Then: Should load byte from correct calculated address
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00000078);
}

TEST_F(LBUInstructionBDDTest, LBU_NegativeOffset_ShouldCalculateAddress) {
    // Given: Memory setup with test data
    cpu->getRegisterFile().write(1, 1008);  // Base address
    
    // When: LBU $t0, -8($t1) - Load from address 1008-8=1000
    mips::InstructionDecoder decoder;
    uint32_t lbu_instruction = 0x90280000 | (0xFFF8 & 0xFFFF);  // LBU with offset=-8
    auto instruction = decoder.decode(lbu_instruction);
    
    // Then: Should load byte from correct calculated address
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00000078);
}
