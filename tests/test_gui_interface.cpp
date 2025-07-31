#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <chrono>
#include "../src/Cpu.h"
#include "../src/gui/MipsSimulatorGUI.h"

using namespace mips;

/**
 * @brief BDD Test Suite for GUI Interface
 * 
 * This file contains step definitions and test implementations for the
 * GUI interface features described in gui_interface.feature
 */
class GUIInterfaceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize GUI application (headless mode for testing)
        gui = std::make_unique<MipsSimulatorGUI>(true); // headless = true
        cpu = std::make_unique<Cpu>();
    }

    void TearDown() override {
        gui.reset();
        cpu.reset();
    }

    std::unique_ptr<MipsSimulatorGUI> gui;
    std::unique_ptr<Cpu> cpu;
};

// ────────────────────────────────────────────────
// Main Window and Layout Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, MainWindowLaunchesSuccessfully) {
    // Given: the GUI application is launched
    ASSERT_TRUE(gui != nullptr);
    
    // When: the application starts
    gui->initialize();
    
    // Then: the main window should be visible
    EXPECT_TRUE(gui->isMainWindowVisible());
    
    // And: the window title should contain "MIPS Assembly Simulator"
    EXPECT_TRUE(gui->getWindowTitle().find("MIPS Assembly Simulator") != std::string::npos);
    
    // And: the menu bar should be present
    EXPECT_TRUE(gui->hasMenuBar());
    
    // And: the toolbar should be present
    EXPECT_TRUE(gui->hasToolbar());
}

TEST_F(GUIInterfaceTest, MainWindowLayoutContainsEssentialPanels) {
    // Given: the main window is open
    gui->initialize();
    
    // Then: all essential panels should be visible
    EXPECT_TRUE(gui->isCodeEditorVisible());
    EXPECT_TRUE(gui->isRegisterViewerVisible());
    EXPECT_TRUE(gui->isMemoryViewerVisible());
    EXPECT_TRUE(gui->isPipelineViewerVisible());
    EXPECT_TRUE(gui->isConsoleOutputVisible());
}

// ────────────────────────────────────────────────
// Code Editor Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, CodeEditorAllowsAssemblyInput) {
    // Given: the code editor is focused
    gui->initialize();
    gui->focusCodeEditor();
    
    // When: I type assembly code
    std::string testCode = R"(addi $t0, $zero, 5
addi $t1, $zero, 10
add $t2, $t0, $t1)";
    
    gui->setCodeEditorText(testCode);
    
    // Then: the code should be displayed
    EXPECT_EQ(gui->getCodeEditorText(), testCode);
    
    // And: syntax highlighting should be applied
    EXPECT_TRUE(gui->isSyntaxHighlightingEnabled());
    
    // And: line numbers should be visible
    EXPECT_TRUE(gui->areLineNumbersVisible());
}

TEST_F(GUIInterfaceTest, CodeEditorSupportsFileOperations) {
    // Given: a test file exists
    std::string testContent = R"(addi $t0, $zero, 42
addi $v0, $zero, 1
syscall)";
    
    gui->initialize();
    
    // When: I load a file (simulated)
    gui->loadFile("test.asm", testContent);
    
    // Then: the content should be loaded
    EXPECT_EQ(gui->getCodeEditorText(), testContent);
    
    // And: the window title should show the filename
    EXPECT_TRUE(gui->getWindowTitle().find("test.asm") != std::string::npos);
}

// ────────────────────────────────────────────────
// Register Viewer Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, RegisterViewerDisplaysAllMIPSRegisters) {
    // Given: the register viewer is visible
    gui->initialize();
    
    // Then: it should display 32 registers
    EXPECT_EQ(gui->getRegisterCount(), 32);
    
    // And: each register should have proper formatting
    for (int i = 0; i < 32; ++i) {
        auto regInfo = gui->getRegisterInfo(i);
        EXPECT_FALSE(regInfo.name.empty());      // Name should not be empty
        EXPECT_TRUE(regInfo.hasDecimalValue);    // Should show decimal
        EXPECT_TRUE(regInfo.hasHexValue);        // Should show hex
    }
    
    // And: PC should be displayed separately
    EXPECT_TRUE(gui->isPCRegisterVisible());
}

TEST_F(GUIInterfaceTest, RegisterValuesUpdateDuringExecution) {
    // Given: a test program is loaded
    std::string program = R"(addi $t0, $zero, 5
addi $t1, $zero, 10
add $t2, $t0, $t1)";
    
    gui->initialize();
    gui->loadProgram(program);
    
    // When: I execute step by step
    gui->stepExecution();
    
    // Then: register $t0 should show value 5
    EXPECT_EQ(gui->getRegisterValue(8), 5u);  // $t0 is register 8
    
    gui->stepExecution();
    
    // Then: register $t1 should show value 10
    EXPECT_EQ(gui->getRegisterValue(9), 10u); // $t1 is register 9
    
    gui->stepExecution();
    
    // Then: register $t2 should show value 15
    EXPECT_EQ(gui->getRegisterValue(10), 15u); // $t2 is register 10
}

TEST_F(GUIInterfaceTest, RegisterViewerHighlightsChangedRegisters) {
    // Given: a program is executing
    gui->initialize();
    gui->loadProgram("addi $t0, $zero, 42");
    
    // When: a register value changes
    uint32_t oldValue = gui->getRegisterValue(8);
    gui->stepExecution();
    
    // Then: the changed register should be highlighted
    EXPECT_TRUE(gui->isRegisterHighlighted(8));
    
    // And: the highlight should fade (simulate time passing)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    gui->updateHighlights();
    
    // Note: In real implementation, this would test the fade-out animation
}

// ────────────────────────────────────────────────
// Memory Viewer Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, MemoryViewerDisplaysMemoryContents) {
    // Given: the memory viewer is visible
    gui->initialize();
    
    // Then: it should display memory properly
    EXPECT_TRUE(gui->isMemoryViewerVisible());
    EXPECT_TRUE(gui->memoryViewerShowsHexAddresses());
    EXPECT_TRUE(gui->memoryViewerShowsHexValues());
    EXPECT_TRUE(gui->memoryViewerShowsAsciiValues());
    EXPECT_TRUE(gui->memoryViewerSupportsScrolling());
    EXPECT_TRUE(gui->hasAddressJumpFeature());
}

TEST_F(GUIInterfaceTest, MemoryViewerUpdatesOnMemoryOperations) {
    // Given: a program with memory operations
    std::string program = R"(addi $t0, $zero, 100
sw $t0, 0($zero)
lw $t1, 0($zero))";
    
    gui->initialize();
    gui->loadProgram(program);
    
    // When: I execute step by step
    gui->stepExecution(); // addi $t0, $zero, 100
    gui->stepExecution(); // sw $t0, 0($zero)
    
    // Then: memory address 0 should show value 100
    EXPECT_EQ(gui->getMemoryValue(0), 100u);
    EXPECT_TRUE(gui->isMemoryAddressHighlighted(0));
}

// ────────────────────────────────────────────────
// Pipeline Visualization Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, PipelineVisualizationShowsAllStages) {
    // Given: pipeline mode is enabled
    gui->initialize();
    gui->setPipelineMode(true);
    
    // Then: it should display 5 pipeline stages
    auto stages = gui->getPipelineStages();
    EXPECT_EQ(stages.size(), 5u);
    
    EXPECT_EQ(stages[0].name, "IF");
    EXPECT_EQ(stages[1].name, "ID");
    EXPECT_EQ(stages[2].name, "EX");
    EXPECT_EQ(stages[3].name, "MEM");
    EXPECT_EQ(stages[4].name, "WB");
    
    // And: each stage should have visual representation
    for (const auto& stage : stages) {
        EXPECT_TRUE(stage.hasVisualRepresentation);
    }
}

TEST_F(GUIInterfaceTest, PipelineVisualizationUpdatesDuringExecution) {
    // Given: a program is loaded in pipeline mode
    std::string program = R"(addi $t0, $zero, 1
addi $t1, $zero, 2
addi $t2, $zero, 3
add $t3, $t0, $t1)";
    
    gui->initialize();
    gui->setPipelineMode(true);
    gui->loadProgram(program);
    
    // When: I step once
    gui->stepExecution();
    
    // Then: IF stage should show first instruction
    auto ifStage = gui->getPipelineStage("IF");
    EXPECT_TRUE(ifStage.instruction.find("addi $t0, $zero, 1") != std::string::npos);
    
    // When: I step again
    gui->stepExecution();
    
    // Then: pipeline should advance
    ifStage = gui->getPipelineStage("IF");
    auto idStage = gui->getPipelineStage("ID");
    
    EXPECT_TRUE(ifStage.instruction.find("addi $t1, $zero, 2") != std::string::npos);
    EXPECT_TRUE(idStage.instruction.find("addi $t0, $zero, 1") != std::string::npos);
}

// ────────────────────────────────────────────────
// Execution Controls Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, ExecutionControlButtonsAreFunctional) {
    // Given: a program is loaded
    gui->initialize();
    gui->loadProgram("addi $t0, $zero, 5");
    
    // Then: buttons should have correct initial state
    EXPECT_TRUE(gui->isRunButtonEnabled());
    EXPECT_TRUE(gui->isStepButtonEnabled());
    EXPECT_TRUE(gui->isResetButtonEnabled());
    EXPECT_FALSE(gui->isStopButtonEnabled());
}

TEST_F(GUIInterfaceTest, StepExecutionWorksCorrectly) {
    // Given: a program is loaded
    std::string program = R"(addi $t0, $zero, 5
addi $t1, $zero, 10)";
    
    gui->initialize();
    gui->loadProgram(program);
    
    uint32_t initialPC = gui->getPCValue();
    
    // When: I step once
    gui->stepExecution();
    
    // Then: exactly one instruction should execute
    EXPECT_EQ(gui->getPCValue(), initialPC + 1);
    EXPECT_EQ(gui->getRegisterValue(8), 5u); // $t0 = 5
}

TEST_F(GUIInterfaceTest, ResetFunctionalityWorks) {
    // Given: a program has been executed
    gui->initialize();
    gui->loadProgram("addi $t0, $zero, 42");
    gui->stepExecution();
    
    EXPECT_EQ(gui->getRegisterValue(8), 42u);
    EXPECT_GT(gui->getPCValue(), 0u);
    
    // When: I reset
    gui->reset();
    
    // Then: state should be reset
    EXPECT_EQ(gui->getRegisterValue(8), 0u);
    EXPECT_EQ(gui->getPCValue(), 0u);
    
    // And: pipeline should be cleared (if in pipeline mode)
    if (gui->isPipelineMode()) {
        auto stages = gui->getPipelineStages();
        for (const auto& stage : stages) {
            EXPECT_TRUE(stage.instruction.empty() || stage.instruction == "NOP");
        }
    }
}

// ────────────────────────────────────────────────
// Console Output Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, ConsoleDisplaysSyscallOutput) {
    // Given: a program with print syscall
    std::string program = R"(addi $v0, $zero, 1
addi $a0, $zero, 42
syscall)";
    
    gui->initialize();
    gui->loadProgram(program);
    
    // When: I execute the program
    gui->runProgram();
    
    // Then: console should display the output
    std::string consoleOutput = gui->getConsoleOutput();
    EXPECT_TRUE(consoleOutput.find("42") != std::string::npos);
}

// ────────────────────────────────────────────────
// Mode Switching Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, SwitchBetweenSingleCycleAndPipelineModes) {
    // Given: simulator is in single-cycle mode
    gui->initialize();
    EXPECT_FALSE(gui->isPipelineMode());
    
    // When: I switch to pipeline mode
    gui->setPipelineMode(true);
    
    // Then: pipeline visualization should be visible
    EXPECT_TRUE(gui->isPipelineViewerVisible());
    EXPECT_TRUE(gui->isPipelineMode());
    
    // And: mode indicator should show pipeline mode
    EXPECT_TRUE(gui->getModeIndicator().find("Pipeline") != std::string::npos);
}

TEST_F(GUIInterfaceTest, ModeSwitchingPreservesProgramState) {
    // Given: program is loaded and partially executed
    gui->initialize();
    gui->loadProgram("addi $t0, $zero, 42");
    gui->stepExecution();
    
    uint32_t registerValue = gui->getRegisterValue(8);
    uint32_t pcValue = gui->getPCValue();
    
    // When: I switch to pipeline mode
    gui->setPipelineMode(true);
    
    // Then: state should be preserved
    EXPECT_EQ(gui->getRegisterValue(8), registerValue);
    EXPECT_EQ(gui->getPCValue(), pcValue);
}

// ────────────────────────────────────────────────
// Error Handling Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, InvalidAssemblyCodeShowsErrors) {
    // Given: invalid assembly code
    std::string invalidCode = R"(invalid_instruction $t0, $t1
add $t0)";
    
    gui->initialize();
    
    // When: I attempt to load the program
    bool loadSuccess = gui->loadProgram(invalidCode);
    
    // Then: loading should fail
    EXPECT_FALSE(loadSuccess);
    
    // And: error messages should be displayed
    EXPECT_TRUE(gui->hasErrorMessages());
    
    // And: problematic lines should be highlighted
    EXPECT_TRUE(gui->hasHighlightedErrorLines());
}

// ────────────────────────────────────────────────
// Breakpoints Tests
// ────────────────────────────────────────────────

TEST_F(GUIInterfaceTest, SetBreakpointsInCodeEditor) {
    // Given: code editor contains a program
    std::string program = R"(addi $t0, $zero, 1
addi $t1, $zero, 2
add $t2, $t0, $t1)";
    
    gui->initialize();
    gui->setCodeEditorText(program);
    
    // When: I set a breakpoint at line 3
    gui->setBreakpoint(3);
    
    // Then: breakpoint should be set
    EXPECT_TRUE(gui->hasBreakpoint(3));
    EXPECT_TRUE(gui->isBreakpointVisible(3));
}

TEST_F(GUIInterfaceTest, ExecutionStopsAtBreakpoints) {
    // Given: program with breakpoint at line 3
    std::string program = R"(addi $t0, $zero, 1
addi $t1, $zero, 2
add $t2, $t0, $t1
addi $t3, $zero, 4)";
    
    gui->initialize();
    gui->loadProgram(program);
    gui->setBreakpoint(3);
    
    // When: I run the program
    gui->runProgram();
    
    // Then: execution should stop at line 3
    EXPECT_EQ(gui->getCurrentExecutionLine(), 3);
    EXPECT_TRUE(gui->isExecutionPaused());
    
    // And: I should be able to inspect state
    EXPECT_EQ(gui->getRegisterValue(8), 1u);  // $t0 = 1
    EXPECT_EQ(gui->getRegisterValue(9), 2u);  // $t1 = 2
    EXPECT_EQ(gui->getRegisterValue(10), 0u); // $t2 not executed yet
}
