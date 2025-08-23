#include "Cpu.h"
#include "Memory.h"
#include "RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

namespace mips
{

class AssemblerDebugTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<Cpu>();
    }

    std::unique_ptr<Cpu> cpu;
};

// Test the exact failing sequence from the enhanced tests
TEST_F(AssemblerDebugTest, ExactFailingSequenceTest)
{
    // Test the exact program that was failing
    std::string program = R"(
# The exact same sequence that was failing  
addi $t0, $zero, 0x48       # 'H' = 0x48
addi $t1, $zero, 0x69       # 'i' = 0x69  
sll $t1, $t1, 8            # shift 'i' to byte position 1
add $t0, $t0, $t1          # combine H + i = should be 0x6948
sw $t0, 0x1000($zero)      # store "Hi\0\0" at address 0x1000

# Print the string
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # string address
syscall                    # Should print "Hi"

addi $v0, $zero, 10        # syscall 10: exit
syscall
)";

    // Execute and check intermediate results
    try
    {
        cpu->loadProgramFromString(program);

        // Execute step by step to see what happens
        int step = 0;
        while (!cpu->shouldTerminate() && step < 20)
        {
            // Check register values at key points
            if (step == 2)
            {                                                 // After loading 'H' and 'i'
                uint32_t t0 = cpu->getRegisterFile().read(8); // $t0
                uint32_t t1 = cpu->getRegisterFile().read(9); // $t1
                std::cout << "Step " << step << ": $t0 = 0x" << std::hex << t0 << ", $t1 = 0x"
                          << std::hex << t1 << std::endl;
            }

            if (step == 3)
            {                                                 // After shift
                uint32_t t1 = cpu->getRegisterFile().read(9); // $t1
                std::cout << "Step " << step << ": After shift, $t1 = 0x" << std::hex << t1
                          << std::endl;
            }

            if (step == 4)
            {                                                 // After add
                uint32_t t0 = cpu->getRegisterFile().read(8); // $t0
                std::cout << "Step " << step << ": After add, $t0 = 0x" << std::hex << t0
                          << std::endl;
            }

            if (step == 5)
            { // After store
                uint32_t memValue = cpu->getMemory().readWord(0x1000);
                std::cout << "Step " << step << ": Memory[0x1000] = 0x" << std::hex << memValue
                          << std::endl;

                // Extract bytes
                char byte0 = (memValue >> 0) & 0xFF;
                char byte1 = (memValue >> 8) & 0xFF;
                std::cout << "  Byte 0: 0x" << std::hex << (int)(unsigned char)byte0 << " ('"
                          << byte0 << "')" << std::endl;
                std::cout << "  Byte 1: 0x" << std::hex << (int)(unsigned char)byte1 << " ('"
                          << byte1 << "')" << std::endl;
            }

            cpu->tick();
            step++;
        }

        // Check final console output
        std::string output = cpu->getConsoleOutput();
        std::cout << "Final console output: '" << output << "'" << std::endl;

        // Hex dump of output
        std::cout << "Hex dump: ";
        for (char c : output)
        {
            std::cout << std::hex << (int)(unsigned char)c << " ";
        }
        std::cout << std::endl;

        EXPECT_TRUE(output.find("Hi") != std::string::npos)
            << "Should contain 'Hi' but was: '" << output << "'";
    }
    catch (const std::exception& e)
    {
        FAIL() << "Execution failed: " << e.what();
    }
}

} // namespace mips
