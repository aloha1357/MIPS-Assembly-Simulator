#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include "test_timeout_helper.hpp"
#include <gtest/gtest.h>

using namespace mips;

class SyscallTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<Cpu>();
    }

    std::unique_ptr<Cpu> cpu;
};

TEST_F(SyscallTest, PrintIntSyscall)
{
    // Test syscall 1: print_int
    // Set up registers: $v0 = 1 (print_int), $a0 = 42 (value to print)
    cpu->getRegisterFile().write(2, 1);   // $v0
    cpu->getRegisterFile().write(4, 42);  // $a0

    // Create and execute syscall instruction
    std::string program = "syscall";
    cpu->loadProgramFromString(program);

    // Execute one cycle
    cpu->tick();

    // Check console output
    EXPECT_EQ(cpu->getConsoleOutput(), "42");
}

TEST_F(SyscallTest, PrintStringBasic)
{
    // Test syscall 4: print_string
    // Set up string "Hi" in memory at address 0x1000
    uint32_t stringAddr = 0x1000;

    // Store "Hi\0\0" as a 32-bit word (little-endian)
    uint32_t stringWord = ('H') | ('i' << 8) | ('\0' << 16) | ('\0' << 24);
    cpu->getMemory().writeWord(stringAddr, stringWord);

    // Set up registers: $v0 = 4 (print_string), $a0 = string address
    cpu->getRegisterFile().write(2, 4);           // $v0
    cpu->getRegisterFile().write(4, stringAddr);  // $a0

    // Execute syscall
    std::string program = "syscall";
    cpu->loadProgramFromString(program);
    cpu->tick();

    // Check console output
    EXPECT_EQ(cpu->getConsoleOutput(), "Hi");
}

TEST_F(SyscallTest, ReadIntSyscall)
{
    // Test syscall 5: read_int
    // Set up console input
    cpu->setConsoleInput("123");

    // Set up registers: $v0 = 5 (read_int)
    cpu->getRegisterFile().write(2, 5);  // $v0

    // Execute syscall
    std::string program = "syscall";
    cpu->loadProgramFromString(program);
    cpu->tick();

    // Check that $v0 now contains the read integer
    EXPECT_EQ(cpu->getRegisterFile().read(2), 123);
}

TEST_F(SyscallTest, ExitSyscall)
{
    // Test syscall 10: exit
    // Set up registers: $v0 = 10 (exit)
    cpu->getRegisterFile().write(2, 10);  // $v0

    // Check initial state
    EXPECT_FALSE(cpu->shouldTerminate());

    // Execute syscall
    std::string program = "syscall";
    cpu->loadProgramFromString(program);
    cpu->tick();

    // Check that program is marked for termination
    EXPECT_TRUE(cpu->shouldTerminate());
}

TEST_F(SyscallTest, TerminationStopsExecution)
{
    // Test that after termination, tick() doesn't execute more instructions
    std::string program = R"(
        addi $v0, $zero, 10
        syscall
        addi $t0, $zero, 99
    )";

    cpu->loadProgramFromString(program);

    // Execute first instruction (sets $v0 to 10)
    cpu->tick();
    EXPECT_EQ(cpu->getRegisterFile().read(2), 10);  // $v0 should be 10
    EXPECT_FALSE(cpu->shouldTerminate());

    // Execute syscall (should terminate)
    cpu->tick();
    EXPECT_TRUE(cpu->shouldTerminate());

    // Try to execute next instruction (should not execute)
    cpu->tick();
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0);  // $t0 should still be 0
}

TEST_F(SyscallTest, ComplexPrintIntProgram)
{
    // Test a complete program that prints multiple integers
    std::string program = R"(
        addi $a0, $zero, 5
        addi $v0, $zero, 1
        syscall
        
        addi $a0, $zero, 10
        addi $v0, $zero, 1
        syscall
        
        addi $v0, $zero, 10
        syscall
    )";

    cpu->loadProgramFromString(program);

    // Execute until termination with safety limits
    CPU_TEST_WITH_LIMITS(*cpu, 20, 2);  // Max 20 steps, 2 seconds timeout

    // Check console output (should contain both printed integers)
    std::string output = cpu->getConsoleOutput();
    EXPECT_EQ(output, "510");
    EXPECT_TRUE(cpu->shouldTerminate());
}

TEST_F(SyscallTest, UnknownSyscallIgnored)
{
    // Test unknown syscall number (should be ignored)
    cpu->getRegisterFile().write(2, 999);  // $v0 = unknown syscall

    std::string program = "syscall";
    cpu->loadProgramFromString(program);
    cpu->tick();

    // Should not terminate and no console output
    EXPECT_FALSE(cpu->shouldTerminate());
    EXPECT_EQ(cpu->getConsoleOutput(), "");
}
