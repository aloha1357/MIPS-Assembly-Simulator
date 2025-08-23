#include "Cpu.h"
#include "RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

class PipelineTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<Cpu>();
    }

    std::unique_ptr<Cpu> cpu;
};

TEST_F(PipelineTest, DefaultSingleCycleMode)
{
    // By default, CPU should be in single-cycle mode
    EXPECT_FALSE(cpu->isPipelineMode());
}

TEST_F(PipelineTest, CanEnablePipelineMode)
{
    // Should be able to enable pipeline mode (even if not fully implemented)
    cpu->setPipelineMode(true);
    EXPECT_TRUE(cpu->isPipelineMode());
}

TEST_F(PipelineTest, CanDisablePipelineMode)
{
    // Should be able to disable pipeline mode
    cpu->setPipelineMode(true);
    EXPECT_TRUE(cpu->isPipelineMode());

    cpu->setPipelineMode(false);
    EXPECT_FALSE(cpu->isPipelineMode());
}

TEST_F(PipelineTest, SingleCycleModeStillWorks)
{
    // Ensure single-cycle mode still works correctly
    cpu->setPipelineMode(false);

    cpu->loadProgramFromString("add $t0, $t1, $t2");
    cpu->getRegisterFile().write(9, 3);   // $t1 = 3
    cpu->getRegisterFile().write(10, 5);  // $t2 = 5

    cpu->run(1);

    EXPECT_EQ(cpu->getRegisterFile().read(8), 8);  // $t0 should be 3 + 5 = 8
    EXPECT_EQ(cpu->getCycleCount(), 1);
}
