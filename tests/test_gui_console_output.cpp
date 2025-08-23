#include "gui/MipsSimulatorGUI.h" // Use base class instead of ImGui for testing
#include <gtest/gtest.h>
#include <memory>

namespace mips
{

class GuiConsoleOutputTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Create base GUI in headless mode for testing (avoids SDL2 dependency)
        gui = std::make_unique<MipsSimulatorGUI>(true);

        // Initialize should succeed
        initialized = gui->initialize();
    }

    void TearDown() override
    {
        if (gui && initialized)
        {
            gui->shutdown();
        }
    }

    std::unique_ptr<MipsSimulatorGUI> gui;
    bool                              initialized = false;
};

// Test 1: Scenario - Test that base GUI console functionality exists
TEST_F(GuiConsoleOutputTest, BaseGuiConsoleMethodsExist)
{
    // Given: GUI simulator initialized in headless mode
    ASSERT_TRUE(initialized) << "GUI should initialize successfully";

    // When: We test basic console methods
    gui->clearConsole();
    gui->addConsoleText("Test message");

    // Then: Console should contain the added text
    std::string consoleOutput = gui->getConsoleOutput();
    EXPECT_TRUE(consoleOutput.find("Test message") != std::string::npos)
        << "Console output should contain 'Test message' but was: '" << consoleOutput << "'";
}

// Test 1: Scenario - Display string output in GUI console (BDD Test - should FAIL initially)
TEST_F(GuiConsoleOutputTest, DisplayStringOutputInGuiConsole_BDD)
{
    // Given: GUI simulator initialized in headless mode
    ASSERT_TRUE(initialized) << "GUI should initialize successfully";

    // And: Load a program that prints integer 42 (simpler than string for now)
    std::string program = R"(
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 42        # value to print  
syscall                    # Should print "42" to console
addi $v0, $zero, 10        # syscall 10: exit
syscall
)";

    bool loaded = gui->loadProgram(program);
    EXPECT_TRUE(loaded) << "Program should load successfully";

    // When: Execute the program
    gui->runProgram(); // This method should execute the program and capture syscall output

    // Then: GUI console should contain "42" from the syscall 1 output
    std::string consoleOutput = gui->getConsoleOutput();
    EXPECT_TRUE(consoleOutput.find("42") != std::string::npos)
        << "Console output should contain '42' from syscall 1 but was: '" << consoleOutput << "'";
}

} // namespace mips
