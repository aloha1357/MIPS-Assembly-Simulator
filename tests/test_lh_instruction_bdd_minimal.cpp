#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>

class LHInstructionBDDTest : public ::testing::Test
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

        // Clear all registers
        for (int i = 0; i < 32; ++i)
        {
            cpu->getRegisterFile().write(i, 0);
        }
        cpu->setProgramCounter(0);
        cpu->getMemory().reset();

        // Initialize test data in memory
        cpu->getMemory().writeWord(1000, 0x12345678);  // Test data at address 1000
        cpu->getMemory().writeWord(1004, 0x9ABCDEF0);  // Test data at address 1004
    }

    void TearDown() override
    {
        cpu.reset();
        assembler.reset();
        decoder.reset();
    }
};

// Phase 7.4 LH (Load Halfword) - Red Light Tests
TEST_F(LHInstructionBDDTest, LH_BasicLoadSigned_ShouldSignExtend)
{
    // Given: Memory contains 0x5678 at halfword address 1000 (little-endian: LSB halfword of
    // 0x12345678)
    cpu->getRegisterFile().write(1, 1000);  // Base address

    // When: LH $t0, 0($t1) - Load halfword signed from address 1000+0 into $t0
    mips::InstructionDecoder decoder;
    uint32_t lh_instruction = 0x84280000;  // LH opcode=0x21, rs=$t1(9), rt=$t0(8), offset=0
    auto     instruction    = decoder.decode(lh_instruction);

    // Then: $t0 should contain 0x00005678 (sign-extended, positive value)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00005678);
}

TEST_F(LHInstructionBDDTest, LH_NegativeHalfword_ShouldSignExtend)
{
    // Given: Memory contains 0x9ABC at halfword address 1006 (MSB halfword has high bit set)
    cpu->getRegisterFile().write(1, 1006);  // Base address

    // When: LH $t0, 0($t1) - Load halfword signed from address 1006+0
    mips::InstructionDecoder decoder;
    uint32_t                 lh_instruction = 0x84280000;  // LH rs=$t1(9), rt=$t0(8), offset=0
    auto                     instruction    = decoder.decode(lh_instruction);

    // Then: $t0 should contain 0xFFFF9ABC (sign-extended, negative value)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xFFFF9ABC);
}

TEST_F(LHInstructionBDDTest, LH_PositiveOffset_ShouldCalculateAddress)
{
    // Given: Memory setup with test data
    cpu->getRegisterFile().write(1, 998);  // Base address

    // When: LH $t0, 2($t1) - Load from address 998+2=1000
    mips::InstructionDecoder decoder;
    uint32_t                 lh_instruction = 0x84280002;  // LH rs=$t1(9), rt=$t0(8), offset=2
    auto                     instruction    = decoder.decode(lh_instruction);

    // Then: Should load halfword from correct calculated address
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00005678);
}

TEST_F(LHInstructionBDDTest, LH_NegativeOffset_ShouldCalculateAddress)
{
    // Given: Memory setup with test data
    cpu->getRegisterFile().write(1, 1008);  // Base address

    // When: LH $t0, -8($t1) - Load from address 1008-8=1000
    mips::InstructionDecoder decoder;
    uint32_t                 lh_instruction = 0x84280000 | (0xFFF8 & 0xFFFF);  // LH with offset=-8
    auto                     instruction    = decoder.decode(lh_instruction);

    // Then: Should load halfword from correct calculated address
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00005678);
}
