#include "../src/Assembler.h"
#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>

using namespace mips;

class SHInstructionBDDTest : public ::testing::Test
{
  protected:
    std::unique_ptr<Cpu>                cpu;
    std::unique_ptr<Assembler>          assembler;
    std::unique_ptr<InstructionDecoder> decoder;

    void SetUp() override
    {
        cpu       = std::make_unique<Cpu>();
        assembler = std::make_unique<Assembler>();
        decoder   = std::make_unique<InstructionDecoder>();

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

// Phase 7.5 SH (Store Halfword) - Red Light Tests
TEST_F(SHInstructionBDDTest, SH_BasicStore_ShouldStoreHalfword)
{
    // Given: Register contains value to store
    cpu->getRegisterFile().write(8, 0x12345678);  // $t0 contains full word
    cpu->getRegisterFile().write(9, 1000);        // $t1 contains base address

    // When: SH $t0, 0($t1) - Store halfword (lower 16 bits) to address 1000+0
    mips::InstructionDecoder decoder;
    uint32_t sh_instruction = 0xA5280000;  // SH opcode=0x29, rs=$t1(9), rt=$t0(8), offset=0
    auto     instruction    = decoder.decode(sh_instruction);

    // Then: Memory should contain 0x5678 at halfword address 1000
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getMemory().readHalfword(1000), 0x5678);
}

TEST_F(SHInstructionBDDTest, SH_PartialWordOverwrite_ShouldPreserveOtherBits)
{
    // Given: Memory contains existing data and register has new halfword value
    cpu->getMemory().writeWord(1000, 0x12345678);  // Original word
    cpu->getRegisterFile().write(8, 0xABCDEF00);   // $t0 contains new value
    cpu->getRegisterFile().write(9, 1002);         // $t1 points to upper halfword of existing word

    // When: SH $t0, 0($t1) - Store halfword to address 1002 (upper half of word at 1000)
    mips::InstructionDecoder decoder;
    uint32_t                 sh_instruction = 0xA5280000;  // SH rs=$t1(9), rt=$t0(8), offset=0
    auto                     instruction    = decoder.decode(sh_instruction);

    // Then: Memory should have lower halfword unchanged, upper halfword updated
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getMemory().readHalfword(1000), 0x5678);  // Lower half preserved
    EXPECT_EQ(cpu->getMemory().readHalfword(1002), 0xEF00);  // Upper half updated
}

TEST_F(SHInstructionBDDTest, SH_PositiveOffset_ShouldCalculateAddress)
{
    // Given: Register contains value and base address with offset
    cpu->getRegisterFile().write(8, 0x12349ABC);  // $t0 contains value
    cpu->getRegisterFile().write(9, 1000);        // $t1 contains base address

    // When: SH $t0, 4($t1) - Store halfword to address 1000+4=1004
    mips::InstructionDecoder decoder;
    uint32_t                 sh_instruction = 0xA5280004;  // SH rs=$t1(9), rt=$t0(8), offset=4
    auto                     instruction    = decoder.decode(sh_instruction);

    // Then: Memory should contain 0x9ABC at address 1004
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getMemory().readHalfword(1004), 0x9ABC);
}

TEST_F(SHInstructionBDDTest, SH_NegativeOffset_ShouldCalculateAddress)
{
    // Given: Register contains value and base address with negative offset
    cpu->getRegisterFile().write(8, 0x567890AB);  // $t0 contains value
    cpu->getRegisterFile().write(9, 1008);        // $t1 contains base address

    // When: SH $t0, -4($t1) - Store halfword to address 1008-4=1004
    mips::InstructionDecoder decoder;
    uint32_t sh_instruction = 0xA528FFFC;  // SH rs=$t1(9), rt=$t0(8), offset=-4 (0xFFFC)
    auto     instruction    = decoder.decode(sh_instruction);

    // Then: Memory should contain 0x90AB at address 1004
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getMemory().readHalfword(1004), 0x90AB);
}
