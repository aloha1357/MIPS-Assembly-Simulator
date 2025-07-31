#include <gtest/gtest.h>
#include "Cpu.h"
#include "RegisterFile.h"
#include "Memory.h"
#include "Assembler.h"

class CpuTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
    }

    void TearDown() override {
        cpu.reset();
    }

    std::unique_ptr<mips::Cpu> cpu;
};

TEST_F(CpuTest, InitialState) {
    EXPECT_EQ(cpu->getCycleCount(), 0);
}

TEST_F(CpuTest, BasicTick) {
    cpu->tick();
    EXPECT_EQ(cpu->getCycleCount(), 1);
}

TEST_F(CpuTest, RunMultipleCycles) {
    cpu->run(5);
    EXPECT_EQ(cpu->getCycleCount(), 5);
}

// Register file tests
TEST(RegisterFileTest, InitialZeroState) {
    mips::RegisterFile regFile;
    
    for (int i = 0; i < 32; ++i) {
        EXPECT_EQ(regFile.read(i), 0) << "Register $" << i << " should be zero";
    }
}

TEST(RegisterFileTest, WriteAndRead) {
    mips::RegisterFile regFile;
    
    regFile.write(1, 0x12345678);
    EXPECT_EQ(regFile.read(1), 0x12345678);
}

TEST(RegisterFileTest, ZeroRegisterAlwaysZero) {
    mips::RegisterFile regFile;
    
    regFile.write(0, 0xDEADBEEF);
    EXPECT_EQ(regFile.read(0), 0) << "$zero register must always be 0";
}

// Memory tests
TEST(MemoryTest, InitialZeroState) {
    mips::Memory memory;
    
    EXPECT_EQ(memory.readWord(0), 0);
    EXPECT_EQ(memory.readWord(0x1000), 0);
}

TEST(MemoryTest, WriteAndRead) {
    mips::Memory memory;
    
    memory.writeWord(0x1000, 0xDEADBEEF);
    EXPECT_EQ(memory.readWord(0x1000), 0xDEADBEEF);
}

TEST(MemoryTest, WordAlignment) {
    mips::Memory memory;
    
    EXPECT_TRUE(memory.isValidAddress(0x1000));
    EXPECT_TRUE(memory.isValidAddress(0x1004));
    EXPECT_FALSE(memory.isValidAddress(0x1001)); // Not word-aligned
    EXPECT_FALSE(memory.isValidAddress(0x1002)); // Not word-aligned
}

// Instruction execution tests
TEST(InstructionTest, AddInstructionBasic) {
    mips::Cpu cpu;
    
    // Set up registers: $t0 = 3, $t1 = 5
    cpu.getRegisterFile().write(8, 3);  // $t0
    cpu.getRegisterFile().write(9, 5);  // $t1
    
    // Execute: add $t2, $t0, $t1
    cpu.loadProgramFromString("add $t2, $t0, $t1");
    cpu.run(1);
    
    // Check result: $t2 should be 8
    EXPECT_EQ(cpu.getRegisterFile().read(10), 8) << "$t2 should contain 3 + 5 = 8";
}

TEST(InstructionTest, AddInstructionWithNegative) {
    mips::Cpu cpu;
    
    // Set up registers: $s0 = -1, $s1 = 1
    cpu.getRegisterFile().write(16, static_cast<uint32_t>(-1));  // $s0
    cpu.getRegisterFile().write(17, 1);  // $s1
    
    // Execute: add $s2, $s0, $s1  
    cpu.loadProgramFromString("add $s2, $s0, $s1");
    cpu.run(1);
    
    // Check result: $s2 should be 0
    EXPECT_EQ(cpu.getRegisterFile().read(18), 0) << "$s2 should contain -1 + 1 = 0";
}

TEST(InstructionTest, AddInstructionZeros) {
    mips::Cpu cpu;
    
    // Registers are already zero-initialized
    
    // Execute: add $v0, $a0, $a1  
    cpu.loadProgramFromString("add $v0, $a0, $a1");
    cpu.run(1);
    
    // Check result: $v0 should be 0
    EXPECT_EQ(cpu.getRegisterFile().read(2), 0) << "$v0 should contain 0 + 0 = 0";
}
