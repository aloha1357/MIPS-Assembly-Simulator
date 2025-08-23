#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/RegisterFile.h"
#include "../src/Stage.h"
#include "../src/IFStage.h"
#include "../src/IDStage.h"
#include "../src/EXStage.h"
#include "../src/MEMStage.h"
#include "../src/WBStage.h"

using namespace mips;

class PipelineIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<Cpu>();
    }

    void TearDown() override {
        cpu.reset();
    }

    std::unique_ptr<Cpu> cpu;
};

/**
 * @brief Test that pipeline stages can be instantiated correctly
 */
TEST_F(PipelineIntegrationTest, CanInstantiatePipelineStages) {
    // Create pipeline registers
    PipelineRegister ifIdReg;
    PipelineRegister idExReg;
    PipelineRegister exMemReg;
    PipelineRegister memWbReg;
    
    // Create pipeline stages
    IFStage ifStage(cpu.get());
    IDStage idStage(cpu.get());
    EXStage exStage(cpu.get());
    MEMStage memStage(cpu.get());
    WBStage wbStage(cpu.get());
    
    // Connect stages with registers
    ifStage.setOutputRegister(&ifIdReg);
    
    idStage.setInputRegister(&ifIdReg);
    idStage.setOutputRegister(&idExReg);
    
    exStage.setInputRegister(&idExReg);
    exStage.setOutputRegister(&exMemReg);
    
    memStage.setInputRegister(&exMemReg);
    memStage.setOutputRegister(&memWbReg);
    
    wbStage.setInputRegister(&memWbReg);
    
    // If we can instantiate and connect everything, test passes
    EXPECT_TRUE(true);
}

/**
 * @brief Test that pipeline registers work correctly
 */
TEST_F(PipelineIntegrationTest, PipelineRegistersWorkCorrectly) {
    PipelineRegister reg;
    
    // Initially should be a bubble
    EXPECT_TRUE(reg.isBubble());
    
    // Create test data
    PipelineData testData;
    testData.pc = 0x1000;
    testData.instruction = nullptr; // For now
    testData.regWrite = true;
    testData.memRead = false;
    
    // Set data
    reg.setData(testData);
    
    // Should not be a bubble anymore (after setData)
    EXPECT_FALSE(reg.isBubble());
    
    // But data should still be zero until clockUpdate
    const PipelineData& currentData = reg.getData();
    EXPECT_EQ(currentData.pc, 0);
    EXPECT_FALSE(currentData.regWrite);
    
    // Clock update to transfer data
    reg.clockUpdate();
    
    // Now data should match
    const PipelineData& updatedData = reg.getData();
    EXPECT_EQ(updatedData.pc, 0x1000);
    EXPECT_TRUE(updatedData.regWrite);
    EXPECT_FALSE(updatedData.memRead);
    
    // Reset and check bubble state
    reg.setBubble();
    EXPECT_TRUE(reg.isBubble());
}

/**
 * @brief Test single cycle mode still works after adding pipeline infrastructure
 */
TEST_F(PipelineIntegrationTest, SingleCycleModeStillWorksWithPipelineCode) {
    // Ensure pipeline mode is off
    cpu->setPipelineMode(false);
    EXPECT_FALSE(cpu->isPipelineMode());
    
    // Load a simple program
    std::string program = "add $t0, $t1, $t2";
    cpu->loadProgramFromString(program);
    
    // Set up registers
    cpu->getRegisterFile().write(9, 5);  // $t1 = 5
    cpu->getRegisterFile().write(10, 3); // $t2 = 3
    
    // Execute one cycle
    cpu->tick();
    
    // Check result
    EXPECT_EQ(cpu->getRegisterFile().read(8), 8); // $t0 should be 5+3=8
}

/**
 * @brief Test that pipeline mode can be enabled without crashing
 */
TEST_F(PipelineIntegrationTest, CanEnablePipelineModeWithoutCrashing) {
    // Enable pipeline mode
    cpu->setPipelineMode(true);
    EXPECT_TRUE(cpu->isPipelineMode());
    
    // Load a simple program
    std::string program = "add $t0, $t1, $t2\nnop\nnop\nnop\nnop";
    cpu->loadProgramFromString(program);
    
    // This should not crash even though pipeline execution isn't fully implemented
    EXPECT_NO_THROW({
        for (int i = 0; i < 10; ++i) {
            cpu->tick();
        }
    });
}

/**
 * @brief Test pipeline stage reset functionality
 */
TEST_F(PipelineIntegrationTest, PipelineStagesCanBeReset) {
    PipelineRegister reg;
    IFStage ifStage(cpu.get());
    ifStage.setOutputRegister(&reg);
    
    // Reset should not crash
    EXPECT_NO_THROW(ifStage.reset());
    
    // Register should be a bubble after reset
    EXPECT_TRUE(reg.isBubble());
}
