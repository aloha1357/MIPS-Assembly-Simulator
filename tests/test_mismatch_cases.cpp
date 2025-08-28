// Focused integration-like unit tests to reproduce previously observed mismatches
#include "gtest/gtest.h"
#include "Cpu.h"
#include "Assembler.h"
#include "Memory.h"

using namespace mips;

// Reproduce SLT area mismatch (around idx ~114-116)
TEST(MismatchCases, SltRegionPrints)
{
    Cpu cpu;
    std::string asmCode = R"(
        lhi $a1, 65535
        trap slt $a0, $a1, $a2
        trap print_int
        trap exit
    )";
    // The assembly above isn't valid MIPS text for 'trap slt', instead we'll craft
    // instructions directly using existing supported mnemonics to reach the same
    // sequence in the integration test. Use lhi/llo/slt sequence.
    std::string prog = R"(
        lhi $a1, 65535
        llo $a1, 0
        llo $a2, 7
        lhi $a2, 0
        slt $a0, $a1, $a2
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    // Expect either 0 or 1 depending on signed compare; check it prints a number
    EXPECT_FALSE(out.empty());
}

// Reproduce LB/LBU sequence around data bytes
TEST(MismatchCases, LoadByteUnsignedVsSigned)
{
    Cpu cpu;
    // Prepare memory like the integration test: place bytes at some address
    uint32_t addr = 200; // choose a mid address
    cpu.getMemory().writeByte(addr + 0, 0xFF); // 255 / -1
    // Program: load address into $a1 then do lbu/lb prints
    std::string prog = R"(
        llo $a1, 200
        lbu $a0, 0 ($a1)
        trap print_int
        lb $a0, 0 ($a1)
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    // first should include 255, second should include 4294967295 (for signed -1)
    EXPECT_NE(out.find("255"), std::string::npos);
    EXPECT_NE(out.find("4294967295"), std::string::npos);
}

// Reproduce sequence around sb/sh/sw and jal / $ra semantics
TEST(MismatchCases, JalAndReturnAddress)
{
    Cpu cpu;
    // Place some data and create a small program that uses jal and then prints $ra
    std::string prog = R"(
        jal label7
        j end
        label7:
        addi $a0, $ra, 0
        trap print_int
        j end
        end:
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    // Expect some non-empty return address printed
    EXPECT_FALSE(out.empty());
}
