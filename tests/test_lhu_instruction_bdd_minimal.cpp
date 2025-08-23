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

class LHUInstructionBDDTest : public ::testing::Test
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

// Phase 7.6 LHU (Load Halfword Unsigned) - Red Light Tests
TEST_F(LHUInstructionBDDTest, LHU_BasicLoadUnsigned_ShouldZeroExtend)
{
    // Given: Memory contains 0x5678 at halfword address 1000 (little-endian: LSB halfword of
    // 0x12345678)
    cpu->getRegisterFile().write(1, 1000);  // Base address

    // When: LHU $t0, 0($t1) - Load halfword unsigned from address 1000+0 into $t0
    mips::InstructionDecoder decoder;
    uint32_t lhu_instruction = 0x94280000;  // LHU opcode=0x25, rs=$t1(9), rt=$t0(8), offset=0
    auto     instruction     = decoder.decode(lhu_instruction);

    // Then: $t0 should contain 0x00005678 (zero-extended, unsigned value)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00005678);
}

TEST_F(LHUInstructionBDDTest, LHU_NegativeHalfword_ShouldZeroExtend)
{
    // Given: Memory contains 0x9ABC at halfword address 1006 (MSB halfword has high bit set)
    cpu->getRegisterFile().write(1, 1006);  // Base address

    // When: LHU $t0, 0($t1) - Load halfword unsigned from address 1006+0
    mips::InstructionDecoder decoder;
    uint32_t                 lhu_instruction = 0x94280000;  // LHU rs=$t1(9), rt=$t0(8), offset=0
    auto                     instruction     = decoder.decode(lhu_instruction);

    // Then: $t0 should contain 0x00009ABC (zero-extended, unsigned value - no sign extension)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x00009ABC);
}

TEST_F(LHUInstructionBDDTest, LHU_PositiveOffset_ShouldCalculateAddress)
{
    // Given: Memory setup with test data
    cpu->getRegisterFile().write(1, 1000);  // Base address

    // When: LHU $t0, 4($t1) - Load halfword unsigned from address 1000+4=1004
    mips::InstructionDecoder decoder;
    uint32_t                 lhu_instruction = 0x94280004;  // LHU rs=$t1(9), rt=$t0(8), offset=4
    auto                     instruction     = decoder.decode(lhu_instruction);

    // Then: $t0 should contain 0x0000DEF0 (zero-extended halfword from address 1004)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x0000DEF0);
}

TEST_F(LHUInstructionBDDTest, LHU_NegativeOffset_ShouldCalculateAddress)
{
    // Given: Memory location with negative offset contains a halfword
    cpu->getRegisterFile().write(1, 1008);  // Base address

    // When: LHU $t0, -4($t1) - Load halfword unsigned from address 1008-4=1004
    mips::InstructionDecoder decoder;
    uint32_t lhu_instruction = 0x9428FFFC;  // LHU rs=$t1(9), rt=$t0(8), offset=-4 (0xFFFC)
    auto     instruction     = decoder.decode(lhu_instruction);

    // Then: $t0 should contain 0x0000DEF0 (zero-extended halfword)
    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0x0000DEF0);
}
