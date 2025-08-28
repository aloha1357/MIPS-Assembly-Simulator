#include "gtest/gtest.h"
#include "Cpu.h"
#include "Assembler.h"

TEST(JalJrIntegration, JalLabelThenJrReturns) {
    std::string asmCode =
        "addi $v0, $zero, 1\n"
        "jal label_debug\n"
        "lhi $a0, 15\n"
        "llo $a0, 16959\n"
        "trap print_int\n"
        "trap exit\n"
        "label_debug:\n"
        "addu $a0, $ra, $zero\n"
        "trap print_int\n"
        "jr $ra\n";

    mips::Cpu cpu;
    cpu.loadProgramFromString(asmCode);

    // Run until terminate or 200 steps
    for (int i = 0; i < 200 && !cpu.shouldTerminate(); ++i) {
        cpu.tick();
    }

    EXPECT_TRUE(cpu.shouldTerminate());
    // Expect console output contains the printed $ra (non-zero) and the marker 999999
    std::string out = cpu.getConsoleOutput();
    EXPECT_NE(out.find("999999"), std::string::npos);
}
