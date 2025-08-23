#include "Assembler.h"
#include "Cpu.h"
#include "Memory.h"
#include "RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

class PipelineExecutionTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<Cpu>();
    }

    std::unique_ptr<Cpu> cpu;
};

TEST_F(PipelineExecutionTest, BasicPipelineExecution)
{
    // Simple program to test pipeline execution
    std::string program = R"(
        addi $t0, $zero, 10
        addi $t1, $zero, 20
        add $t2, $t0, $t1
        addi $v0, $zero, 10
        syscall
    )";

    cpu->loadProgramFromString(program);

    // Enable pipeline mode
    cpu->setPipelineMode(true);
    EXPECT_TRUE(cpu->isPipelineMode());

    // Execute with maximum cycle limit to prevent infinite loops
    const int MAX_CYCLES = 15;  // Generous limit for 4 instructions + pipeline fill
    int       cycles     = 0;

    for (cycles = 0; cycles < MAX_CYCLES; ++cycles)
    {
        if (cpu->shouldTerminate())
            break;
        cpu->tick();
    }

    // Ensure test terminated properly (not due to timeout)
    EXPECT_LT(cycles, MAX_CYCLES) << "Test exceeded maximum cycle limit";

    // Check that registers have been updated correctly
    EXPECT_EQ(cpu->getRegisterFile().read(8), 10u);   // $t0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 20u);   // $t1
    EXPECT_EQ(cpu->getRegisterFile().read(10), 30u);  // $t2
}

TEST_F(PipelineExecutionTest, PipelineVsSingleCycleComparison)
{
    std::string program = R"(
        addi $t0, $zero, 5
        addi $t1, $zero, 3
        add $t2, $t0, $t1
        sub $t3, $t0, $t1
        addi $v0, $zero, 10
        syscall
    )";

    // Test single-cycle mode
    auto cpu1 = std::make_unique<Cpu>();
    cpu1->loadProgramFromString(program);
    cpu1->setPipelineMode(false);

    const int MAX_CYCLES = 20;
    int       cycles1    = 0;
    while (!cpu1->shouldTerminate() && cycles1 < MAX_CYCLES)
    {
        cpu1->tick();
        cycles1++;
    }
    EXPECT_LT(cycles1, MAX_CYCLES) << "Single-cycle mode exceeded maximum cycles";

    uint32_t t0_single = cpu1->getRegisterFile().read(8);
    uint32_t t1_single = cpu1->getRegisterFile().read(9);
    uint32_t t2_single = cpu1->getRegisterFile().read(10);
    uint32_t t3_single = cpu1->getRegisterFile().read(11);

    // Test pipeline mode
    auto cpu2 = std::make_unique<Cpu>();
    cpu2->loadProgramFromString(program);
    cpu2->setPipelineMode(true);

    int cycles2 = 0;
    while (!cpu2->shouldTerminate() && cycles2 < MAX_CYCLES)
    {
        cpu2->tick();
        cycles2++;
    }
    EXPECT_LT(cycles2, MAX_CYCLES) << "Pipeline mode exceeded maximum cycles";

    uint32_t t0_pipeline = cpu2->getRegisterFile().read(8);
    uint32_t t1_pipeline = cpu2->getRegisterFile().read(9);
    uint32_t t2_pipeline = cpu2->getRegisterFile().read(10);
    uint32_t t3_pipeline = cpu2->getRegisterFile().read(11);

    // Results should be the same regardless of execution mode
    EXPECT_EQ(t0_single, t0_pipeline);  // $t0 = 5
    EXPECT_EQ(t1_single, t1_pipeline);  // $t1 = 3
    EXPECT_EQ(t2_single, t2_pipeline);  // $t2 = 8
    EXPECT_EQ(t3_single, t3_pipeline);  // $t3 = 2

    EXPECT_EQ(t0_single, 5u);
    EXPECT_EQ(t1_single, 3u);
    EXPECT_EQ(t2_single, 8u);
    EXPECT_EQ(t3_single, 2u);
}

TEST_F(PipelineExecutionTest, PipelineModeWithMemoryOperations)
{
    std::string program = R"(
        addi $t0, $zero, 100
        sw $t0, 0($zero)
        lw $t1, 0($zero)
        addi $t2, $t1, 50
        addi $v0, $zero, 10
        syscall
    )";

    cpu->loadProgramFromString(program);
    cpu->setPipelineMode(true);

    // Execute program with timeout protection
    const int MAX_CYCLES = 25;  // More cycles for memory operations
    int       cycles     = 0;
    while (!cpu->shouldTerminate() && cycles < MAX_CYCLES)
    {
        cpu->tick();
        cycles++;
    }
    EXPECT_LT(cycles, MAX_CYCLES) << "Memory operations test exceeded maximum cycles";

    // Verify results
    EXPECT_EQ(cpu->getRegisterFile().read(8), 100u);   // $t0
    EXPECT_EQ(cpu->getRegisterFile().read(9), 100u);   // $t1 (loaded from memory)
    EXPECT_EQ(cpu->getRegisterFile().read(10), 150u);  // $t2
    EXPECT_EQ(cpu->getMemory().readWord(0), 100u);     // Memory location 0
}

TEST_F(PipelineExecutionTest, PipelineStageReset)
{
    std::string program = R"(
        addi $t0, $zero, 42
        syscall
    )";

    cpu->loadProgramFromString(program);
    cpu->setPipelineMode(true);

    // Execute a few cycles
    cpu->tick();
    cpu->tick();

    // Reset should work
    cpu->reset();

    // All registers should be zero after reset
    for (int i = 0; i < 32; ++i)
    {
        EXPECT_EQ(cpu->getRegisterFile().read(i), 0u)
            << "Register " << i << " should be zero after reset";
    }

    EXPECT_EQ(cpu->getProgramCounter(), 0u);
    EXPECT_FALSE(cpu->shouldTerminate());
}

TEST_F(PipelineExecutionTest, PipelineModeToggling)
{
    std::string program = R"(
        addi $t0, $zero, 1
        addi $v0, $zero, 10
        syscall
    )";

    cpu->loadProgramFromString(program);

    // Start in single-cycle mode (default)
    EXPECT_FALSE(cpu->isPipelineMode());

    // Enable pipeline mode
    cpu->setPipelineMode(true);
    EXPECT_TRUE(cpu->isPipelineMode());

    // Execute with timeout protection
    const int MAX_CYCLES = 10;
    int       cycles     = 0;
    while (!cpu->shouldTerminate() && cycles < MAX_CYCLES)
    {
        cpu->tick();
        cycles++;
    }
    EXPECT_LT(cycles, MAX_CYCLES) << "Pipeline toggling test exceeded maximum cycles";

    EXPECT_EQ(cpu->getRegisterFile().read(8), 1u);
}
