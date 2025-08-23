#include "Assembler.h"
#include "Cpu.h"
#include "Memory.h"
#include "RegisterFile.h"
#include <gtest/gtest.h>

class CpuTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<mips::Cpu>();
    }

    void TearDown() override
    {
        cpu.reset();
    }

    std::unique_ptr<mips::Cpu> cpu;
};

TEST_F(CpuTest, InitialState)
{
    EXPECT_EQ(cpu->getCycleCount(), 0);
}

TEST_F(CpuTest, BasicTick)
{
    cpu->tick();
    EXPECT_EQ(cpu->getCycleCount(), 1);
}

TEST_F(CpuTest, RunMultipleCycles)
{
    cpu->run(5);
    EXPECT_EQ(cpu->getCycleCount(), 5);
}

// Register file tests
TEST(RegisterFileTest, InitialZeroState)
{
    mips::RegisterFile regFile;

    for (int i = 0; i < 32; ++i)
    {
        EXPECT_EQ(regFile.read(i), 0) << "Register $" << i << " should be zero";
    }
}

TEST(RegisterFileTest, WriteAndRead)
{
    mips::RegisterFile regFile;

    regFile.write(1, 0x12345678);
    EXPECT_EQ(regFile.read(1), 0x12345678);
}

TEST(RegisterFileTest, ZeroRegisterAlwaysZero)
{
    mips::RegisterFile regFile;

    regFile.write(0, 0xDEADBEEF);
    EXPECT_EQ(regFile.read(0), 0) << "$zero register must always be 0";
}

// Memory tests
TEST(MemoryTest, InitialZeroState)
{
    mips::Memory memory;

    EXPECT_EQ(memory.readWord(0), 0);
    EXPECT_EQ(memory.readWord(0x1000), 0);
}

TEST(MemoryTest, WriteAndRead)
{
    mips::Memory memory;

    memory.writeWord(0x1000, 0xDEADBEEF);
    EXPECT_EQ(memory.readWord(0x1000), 0xDEADBEEF);
}

TEST(MemoryTest, WordAlignment)
{
    mips::Memory memory;

    EXPECT_TRUE(memory.isValidAddress(0x1000));
    EXPECT_TRUE(memory.isValidAddress(0x1004));
    EXPECT_FALSE(memory.isValidAddress(0x1001)); // Not word-aligned
    EXPECT_FALSE(memory.isValidAddress(0x1002)); // Not word-aligned
}

// Instruction execution tests
TEST(InstructionTest, AddInstructionBasic)
{
    mips::Cpu cpu;

    // Set up registers: $t0 = 3, $t1 = 5
    cpu.getRegisterFile().write(8, 3); // $t0
    cpu.getRegisterFile().write(9, 5); // $t1

    // Execute: add $t2, $t0, $t1
    cpu.loadProgramFromString("add $t2, $t0, $t1");
    cpu.run(1);

    // Check result: $t2 should be 8
    EXPECT_EQ(cpu.getRegisterFile().read(10), 8) << "$t2 should contain 3 + 5 = 8";
}

TEST(InstructionTest, AddInstructionWithNegative)
{
    mips::Cpu cpu;

    // Set up registers: $s0 = -1, $s1 = 1
    cpu.getRegisterFile().write(16, static_cast<uint32_t>(-1)); // $s0
    cpu.getRegisterFile().write(17, 1);                         // $s1

    // Execute: add $s2, $s0, $s1
    cpu.loadProgramFromString("add $s2, $s0, $s1");
    cpu.run(1);

    // Check result: $s2 should be 0
    EXPECT_EQ(cpu.getRegisterFile().read(18), 0) << "$s2 should contain -1 + 1 = 0";
}

TEST(InstructionTest, AddInstructionZeros)
{
    mips::Cpu cpu;

    // Registers are already zero-initialized

    // Execute: add $v0, $a0, $a1
    cpu.loadProgramFromString("add $v0, $a0, $a1");
    cpu.run(1);

    // Check result: $v0 should be 0
    EXPECT_EQ(cpu.getRegisterFile().read(2), 0) << "$v0 should contain 0 + 0 = 0";
}

// ADDI instruction tests
TEST(InstructionTest, AddiInstructionPositiveImmediate)
{
    mips::Cpu cpu;

    // Set up register: $t0 = 5
    cpu.getRegisterFile().write(8, 5); // $t0

    // Execute: addi $t1, $t0, 10
    cpu.loadProgramFromString("addi $t1, $t0, 10");
    cpu.run(1);

    // Check result: $t1 should be 15
    EXPECT_EQ(cpu.getRegisterFile().read(9), 15) << "$t1 should contain 5 + 10 = 15";
}

TEST(InstructionTest, AddiInstructionNegativeImmediate)
{
    mips::Cpu cpu;

    // Set up register: $s0 = 3
    cpu.getRegisterFile().write(16, 3); // $s0

    // Execute: addi $s1, $s0, -4
    cpu.loadProgramFromString("addi $s1, $s0, -4");
    cpu.run(1);

    // Check result: $s1 should be -1 (0xFFFFFFFF in unsigned representation)
    EXPECT_EQ(cpu.getRegisterFile().read(17), static_cast<uint32_t>(-1))
        << "$s1 should contain 3 + (-4) = -1";
}

TEST(InstructionTest, AddiInstructionZeroImmediate)
{
    mips::Cpu cpu;

    // Set up register: $t2 = 42
    cpu.getRegisterFile().write(10, 42); // $t2

    // Execute: addi $t3, $t2, 0
    cpu.loadProgramFromString("addi $t3, $t2, 0");
    cpu.run(1);

    // Check result: $t3 should be 42
    EXPECT_EQ(cpu.getRegisterFile().read(11), 42) << "$t3 should contain 42 + 0 = 42";
}

// Memory instruction tests
TEST(InstructionTest, LwInstructionBasic)
{
    mips::Cpu cpu;

    // Set up memory: address 0x1000 contains 0xDEADBEEF
    cpu.getMemory().writeWord(0x1000, 0xDEADBEEF);

    // Execute: lw $t0, 0x1000($zero)
    cpu.loadProgramFromString("lw $t0, 0x1000($zero)");
    cpu.run(1);

    // Check result: $t0 should contain 0xDEADBEEF
    EXPECT_EQ(cpu.getRegisterFile().read(8), 0xDEADBEEF) << "$t0 should contain loaded value";
}

TEST(InstructionTest, SwInstructionBasic)
{
    mips::Cpu cpu;

    // Set up register: $t1 = 0xCAFEBABE
    cpu.getRegisterFile().write(9, 0xCAFEBABE); // $t1

    // Execute: sw $t1, 0x2000($zero)
    cpu.loadProgramFromString("sw $t1, 0x2000($zero)");
    cpu.run(1);

    // Check result: memory at 0x2000 should contain 0xCAFEBABE
    EXPECT_EQ(cpu.getMemory().readWord(0x2000), 0xCAFEBABE) << "Memory should contain stored value";
}

TEST(InstructionTest, LwSwWithOffset)
{
    mips::Cpu cpu;

    // Set up base register: $s0 = 0x1000
    cpu.getRegisterFile().write(16, 0x1000); // $s0

    // Set up register: $t0 = 0x12345678
    cpu.getRegisterFile().write(8, 0x12345678); // $t0

    // Execute: sw $t0, 4($s0)  (store at 0x1004)
    cpu.loadProgramFromString("sw $t0, 4($s0)");
    cpu.run(1);

    // Execute: lw $t1, 4($s0)  (load from 0x1004)
    cpu.loadProgramFromString("lw $t1, 4($s0)");
    cpu.run(1);

    // Check result: $t1 should contain 0x12345678
    EXPECT_EQ(cpu.getRegisterFile().read(9), 0x12345678)
        << "$t1 should contain the stored and loaded value";
}

// Control flow instruction tests
TEST(InstructionTest, BeqInstructionTaken)
{
    mips::Cpu cpu;

    // Set up registers: $t0 = $t1 = 5 (equal values)
    cpu.getRegisterFile().write(8, 5); // $t0
    cpu.getRegisterFile().write(9, 5); // $t1

    // Execute program with branch taken
    std::string program = R"(
        beq $t0, $t1, target
        addi $v0, $zero, 1
        target:
        addi $v0, $zero, 42
    )";

    cpu.loadProgramFromString(program);
    cpu.run(10); // Run enough cycles to complete

    // Check result: $v0 should be 42 (branch was taken)
    EXPECT_EQ(cpu.getRegisterFile().read(2), 42) << "$v0 should be 42 (branch taken)";
}

TEST(InstructionTest, BeqInstructionNotTaken)
{
    mips::Cpu cpu;

    // Set up registers: $t0 = 5, $t1 = 3 (not equal)
    cpu.getRegisterFile().write(8, 5); // $t0
    cpu.getRegisterFile().write(9, 3); // $t1

    // Execute program with branch not taken
    std::string program = R"(
        beq $t0, $t1, target
        addi $v0, $zero, 1
        target:
        addi $v0, $zero, 42
    )";

    cpu.loadProgramFromString(program);
    cpu.run(10); // Run enough cycles to complete

    // Check result: $v0 should be 42 (both instructions executed)
    EXPECT_EQ(cpu.getRegisterFile().read(2), 42) << "$v0 should be 42 (both instructions executed)";
}

TEST(InstructionTest, JInstructionUnconditional)
{
    mips::Cpu cpu;

    // Execute program with unconditional jump
    std::string program = R"(
        j target
        addi $v0, $zero, 1
        target:
        addi $v0, $zero, 7
    )";

    cpu.loadProgramFromString(program);
    cpu.run(10); // Run enough cycles to complete

    // Check result: $v0 should be 7 (jump was taken, middle instruction skipped)
    EXPECT_EQ(cpu.getRegisterFile().read(2), 7) << "$v0 should be 7 (jump taken)";
}
