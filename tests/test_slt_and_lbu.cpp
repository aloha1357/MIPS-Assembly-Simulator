// Minimal gtest cases for SLT and LBU/LB semantics
#include "Assembler.h"
#include "Cpu.h"
#include "Memory.h"
#include "gtest/gtest.h"

using namespace mips;

TEST(SltInstructionTest, BasicSignedComparison)
{
    Cpu cpu;
    // Build small program: set registers and use slt
    std::string asmCode = R"(
        llo $a0, 5
        llo $a1, 10
        slt $a0, $a0, $a1
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(asmCode);
    cpu.tick();  // execute llo
    cpu.tick();  // execute llo
    cpu.tick();  // slt
    cpu.tick();  // trap print_int

    auto out = cpu.getConsoleOutput();
    // printed value should be 1 (5 < 10)
    EXPECT_NE(out.find("1"), std::string::npos);
}

TEST(LbuInstructionTest, ZeroAndSignExtension)
{
    Cpu cpu;
    // Data: at end using assembler data directives
    std::string asmCode = R"(
        llo $a0, 0
        lhi $a0, 0
        trap exit
    )";
    // We'll manually write bytes and then test via instructions
    cpu.getMemory().writeByte(100, 0xFF);  // -1 as signed, 255 as unsigned
    // Program: load address into $a1, then lb/lbu
    std::string prog = R"(
        llo $a1, 100
        lbu $a0, 0 ($a1)
        trap print_int
        lb $a0, 0 ($a1)
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    // Execute until exit
    while (!cpu.shouldTerminate())
        cpu.tick();

    auto out = cpu.getConsoleOutput();
    // Expect first printed value to be 255 (zero-extended)
    EXPECT_NE(out.find("255"), std::string::npos);
    // Expect signed lb to print 4294967295 for 0xFF interpreted as -1
    EXPECT_NE(out.find("4294967295"), std::string::npos);
}
