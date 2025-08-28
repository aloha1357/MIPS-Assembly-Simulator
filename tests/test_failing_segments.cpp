// Tests that isolate the five failing printed outputs observed in the integration
// run. Each test runs a small snippet similar to the original assembly and
// asserts the expected printed value from assignment/test/instructions.out.

#include "gtest/gtest.h"
#include "Cpu.h"
#include "Assembler.h"
#include "Memory.h"

using namespace mips;

// 1) Expected 1 but saw 4294967288 in integration
TEST(FailingSegments, ExpectOne_SltRegion)
{
    Cpu cpu;
    // Recreate the slt sequence near the original test
    std::string prog = R"(
        lhi $a1, 65535
        llo $a1, 0
        llo $a2, 0
        lhi $a2, 0
        slt $a0, $a1, $a2
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    // expected '1' per fixture
    EXPECT_NE(out.find("1\n"), std::string::npos) << "output=" << out;
}

// 2) Expected 0 but saw 4294967288
TEST(FailingSegments, ExpectZero_SltRegion)
{
    Cpu cpu;
    // Recreate original context: earlier in the file $a2 was set to 7
    std::string prog = R"(
        llo $a2, 7
        lhi $a2, 0
        lhi $a1, 65535
        llo $a1, 65535
        slt $a0, $a2, $a1
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    EXPECT_NE(out.find("0\n"), std::string::npos) << "output=" << out;
}

// 3) Expected 4294967295 but saw 255 (LBU/LB region)
TEST(FailingSegments, Expect4294967295_LBvsLBU)
{
    Cpu cpu;
    // Prepare data at address 300
    uint32_t addr = 300;
    cpu.getMemory().writeByte(addr + 0, 0xFF); // -1 signed (should produce 4294967295 when sign-extended)
    std::string prog = R"(
        llo $a1, 300
        lbu $a0, 0 ($a1)
        trap print_int
        lb $a0, 0 ($a1)
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    // check that signed load printed 4294967295
    EXPECT_NE(out.find("4294967295\n"), std::string::npos) << "output=" << out;
}

// 4) Expected 3 but saw 123 (store/load region)
TEST(FailingSegments, ExpectThree_AfterShSb)
{
    Cpu cpu;
    // Use la/data so assembler places data in same layout as the integration test
    std::string prog = R"(
        la $a1, data
        lb $a0, 0 ($a1)
        trap print_int
        lbu $a0, 0 ($a1)
        trap print_int
        lw $a0, 0 ($a1)
        trap print_int
        llo $a0, 259
        sb $a0, 8 ($a1)
        lbu $a0, 8 ($a1)
        trap print_int
        llo $a0, 1
        lhi $a0, 1
        trap print_int
        sh $a0, 8 ($a1)
        lhu $a0, 8 ($a1)
        trap print_int
        trap exit

        data:
        .word 255
        .word 65535
        .byte 123
        .byte 111
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    // Expect to see '3' somewhere per integration fixture
    EXPECT_NE(out.find("3\n"), std::string::npos) << "output=" << out;
}

// 5) Expected 1 but saw 65537 (llo/lhi composition or jal-$ra area)
TEST(FailingSegments, ExpectOne_LloLhiOrRa)
{
    Cpu cpu;
    std::string prog = R"(
        llo $a0, 1
        lhi $a0, 0
        trap print_int
        llo $a0, 1
        lhi $a0, 1
        trap print_int
        trap exit
    )";
    cpu.loadProgramFromString(prog);
    while (!cpu.shouldTerminate()) cpu.tick();
    std::string out = cpu.getConsoleOutput();
    // Expect first print to be 1 (from low half only), second may be 65537 if composition happens
    EXPECT_NE(out.find("1\n"), std::string::npos) << "output=" << out;
}
