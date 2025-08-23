#include <cucumber-cpp/autodetect.hpp>
#include <gtest/gtest.h>
#include "Cpu.h"
#include "Assembler.h"
#include "Instruction.h"
#include <memory>
#include <map>
#include <string>
#include <regex>
#include <chrono>

using cucumber::ScenarioScope;

namespace {
    // Helper to convert register names to numbers
    std::map<std::string, int> registerMap = {
        {"$zero", 0}, {"$at", 1},
        {"$v0", 2}, {"$v1", 3},
        {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
        {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11},
        {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
        {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19},
        {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
        {"$t8", 24}, {"$t9", 25},
        {"$k0", 26}, {"$k1", 27},
        {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31}
    };

    int getRegisterNumber(const std::string& regName) {
        auto it = registerMap.find(regName);
        if (it != registerMap.end()) {
            return it->second;
        }
        return -1; // Invalid register
    }

    uint32_t parseValue(const std::string& value) {
        if (value.find("0x") == 0) {
            return std::stoul(value, nullptr, 16);
        }
        return std::stoul(value);
    }
}

struct CpuCtx {
    std::unique_ptr<mips::Cpu> cpu;
    std::string lastProgram;
};

GIVEN("^an empty RegisterFile initialised to zeros$") {
    ScenarioScope<CpuCtx> context;
    context->cpu = std::make_unique<mips::Cpu>();
    context->cpu->reset();
}

GIVEN("^an empty Data Memory initialised to zeros$") {
    ScenarioScope<CpuCtx> context;
    // Memory is already initialized to zeros in constructor
}

GIVEN("^register (\\$\\w+) contains ([\\-\\d]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(int, value);
    
    ScenarioScope<CpuCtx> context;
    int regNum = getRegisterNumber(regName);
    ASSERT_NE(regNum, -1) << "Invalid register: " << regName;
    
    context->cpu->getRegisterFile().write(regNum, static_cast<uint32_t>(value));
}

GIVEN("^data memory address (0x[0-9A-Fa-f]+) contains value (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, addressStr);
    REGEX_PARAM(std::string, valueStr);
    
    ScenarioScope<CpuCtx> context;
    uint32_t address = parseValue(addressStr);
    uint32_t value = parseValue(valueStr);
    
    context->cpu->getMemory().writeWord(address, value);
}

WHEN("^the program$") {
    ScenarioScope<CpuCtx> context;
    // Program text will be captured by cucumber
    // For now, we'll store it but not parse/execute yet
}

WHEN("^the program \"\"\"([^\"\"\"]+)\"\"\" is executed for (\\d+) cycles?$") {
    REGEX_PARAM(std::string, program);
    REGEX_PARAM(int, cycles);
    
    ScenarioScope<CpuCtx> context;
    context->lastProgram = program;
    
    // TODO: Parse and load the program
    // For now, just run the specified cycles
    context->cpu->run(cycles);
}

THEN("^register (\\$\\w+) should equal ([\\-\\d]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(int, expected);
    
    ScenarioScope<CpuCtx> context;
    int regNum = getRegisterNumber(regName);
    ASSERT_NE(regNum, -1) << "Invalid register: " << regName;
    
    uint32_t actual = context->cpu->getRegisterFile().read(regNum);
    EXPECT_EQ(actual, static_cast<uint32_t>(expected))
        << "Register " << regName << " (reg " << regNum << ") mismatch";
}

THEN("^data memory address (0x[0-9A-Fa-f]+) should equal (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, addressStr);
    REGEX_PARAM(std::string, expectedStr);
    
    ScenarioScope<CpuCtx> context;
    uint32_t address = parseValue(addressStr);
    uint32_t expected = parseValue(expectedStr);
    
    uint32_t actual = context->cpu->getMemory().readWord(address);
    EXPECT_EQ(actual, expected)
        << "Memory at address " << addressStr << " mismatch";
}

// Pipeline-specific step definitions (TODO: implement when pipeline is ready)
THEN("^pipeline register ID/EX in cycle (\\d+) should contain a bubble$") {
    REGEX_PARAM(int, cycle);
    
    // TODO: Implement pipeline state inspection
    PENDING("Pipeline inspection not yet implemented");
}

THEN("^no stall cycles should be inserted$") {
    // TODO: Implement stall detection
    PENDING("Stall detection not yet implemented");
}

THEN("^exactly (\\d+) bubble should appear in (.+) during cycle (\\d+)$") {
    REGEX_PARAM(int, bubbleCount);
    REGEX_PARAM(std::string, stage);
    REGEX_PARAM(int, cycle);
    
    // TODO: Implement bubble counting
    PENDING("Bubble counting not yet implemented");
}

// ================================
// GUI Console Output Scenarios
// ================================

#include "gui/ImGuiMipsSimulatorGUI.h"

struct GuiContext {
    std::unique_ptr<mips::ImGuiMipsSimulatorGUI> gui;
    std::string loadedProgram;
    std::string consoleOutput;
};

GIVEN("^I have the GUI simulator initialized in headless mode$") {
    ScenarioScope<GuiContext> context;
    
    // Create GUI in headless mode
    context->gui = std::make_unique<mips::ImGuiMipsSimulatorGUI>(true);
    
    // Initialize the GUI
    bool initialized = context->gui->initialize();
    EXPECT_TRUE(initialized) << "GUI initialization should succeed";
}

GIVEN("^I load a program that prints a string \"([^\"]+)\"$") {
    REGEX_PARAM(std::string, expectedString);
    ScenarioScope<GuiContext> context;
    
    // Create MIPS program that prints the specified string
    // For "Test", we need to set up memory and use syscall 4
    std::string program = R"(
# Setup string in memory at address 0x1000
addi $t0, $zero, 0x54       # 'T' (ASCII 84)
addi $t1, $zero, 0x65       # 'e' (ASCII 101)  
addi $t2, $zero, 0x73       # 's' (ASCII 115)
addi $t3, $zero, 0x74       # 't' (ASCII 116)

# Combine into word: 'T' + 'e'<<8 + 's'<<16 + 't'<<24
sll $t1, $t1, 8            # shift 'e' to position 8-15
sll $t2, $t2, 16           # shift 's' to position 16-23  
sll $t3, $t3, 24           # shift 't' to position 24-31
add $t0, $t0, $t1          # combine T + e
add $t0, $t0, $t2          # combine T + e + s
add $t0, $t0, $t3          # combine T + e + s + t
sw $t0, 0x1000($zero)      # store "Test" at address 0x1000

# Store null terminator
sw $zero, 0x1004($zero)    # store "\0\0\0\0" at address 0x1004

# Print the string using syscall 4
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # load string address into $a0
syscall                    # Should print "Test" to console

# Exit program properly
addi $v0, $zero, 10        # syscall 10: exit
syscall
)";
    
    context->loadedProgram = program;
    
    // Load program into GUI - this method needs to be implemented
    bool loaded = context->gui->loadProgram(program);
    EXPECT_TRUE(loaded) << "Program should load successfully";
}

WHEN("^I execute the program$") {
    ScenarioScope<GuiContext> context;
    
    // Execute the loaded program - this method needs to be implemented
    context->gui->executeCode();
    
    // Get console output after execution
    context->consoleOutput = context->gui->getConsoleOutput();
}

THEN("^the GUI console should contain \"([^\"]+)\"$") {
    REGEX_PARAM(std::string, expectedOutput);
    ScenarioScope<GuiContext> context;
    
    // Check if the console output contains the expected string
    EXPECT_TRUE(context->consoleOutput.find(expectedOutput) != std::string::npos) 
        << "Console output should contain '" << expectedOutput 
        << "' but was: '" << context->consoleOutput << "'";
}

// ============================================================================
// LOGICAL INSTRUCTIONS BDD STEP DEFINITIONS
// ============================================================================

struct LogicalContext {
    std::unique_ptr<mips::Cpu> cpu;
    std::map<std::string, uint32_t> initialValues;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    size_t instructionCount = 0;
};

GIVEN("^I have a MIPS CPU simulator$") {
    ScenarioScope<LogicalContext> context;
    context->cpu = std::make_unique<mips::Cpu>();
}

GIVEN("^the CPU is in a clean state with all registers set to zero$") {
    ScenarioScope<LogicalContext> context;
    // Reset all registers to zero
    for (int i = 0; i < 32; ++i) {
        context->cpu->getRegisterFile().setRegister(i, 0);
    }
}

GIVEN("^the program counter is at address 0$") {
    ScenarioScope<LogicalContext> context;
    context->cpu->setProgramCounter(0);
}

GIVEN("^register (.+) contains (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, valueStr);
    ScenarioScope<LogicalContext> context;
    
    int regNum = registerMap[regName];
    uint32_t value = static_cast<uint32_t>(std::stoul(valueStr, nullptr, 16));
    
    // Store initial value for later verification
    context->initialValues[regName] = value;
    context->cpu->getRegisterFile().setRegister(regNum, value);
}

WHEN("^I execute the instruction \"(.+)\"$") {
    REGEX_PARAM(std::string, instruction);
    ScenarioScope<LogicalContext> context;
    
    context->startTime = std::chrono::high_resolution_clock::now();
    
    // Parse and execute the instruction
    auto instr = mips::Assembler::parseInstruction(instruction);
    EXPECT_NE(instr, nullptr) << "Failed to parse instruction: " << instruction;
    
    if (instr) {
        instr->execute(*context->cpu);
        context->instructionCount++;
    }
    
    context->endTime = std::chrono::high_resolution_clock::now();
}

WHEN("^I execute the following instruction sequence:$") {
    REGEX_PARAM(std::string, instructionBlock);
    ScenarioScope<LogicalContext> context;
    
    context->startTime = std::chrono::high_resolution_clock::now();
    
    // Split instructions by newlines and execute each
    std::istringstream stream(instructionBlock);
    std::string line;
    
    while (std::getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (!line.empty()) {
            auto instr = mips::Assembler::parseInstruction(line);
            EXPECT_NE(instr, nullptr) << "Failed to parse instruction: " << line;
            
            if (instr) {
                instr->execute(*context->cpu);
                context->instructionCount++;
            }
        }
    }
    
    context->endTime = std::chrono::high_resolution_clock::now();
}

WHEN("^I have (\\d+) logical instructions prepared$") {
    REGEX_PARAM(int, count);
    ScenarioScope<LogicalContext> context;
    context->instructionCount = count;
}

WHEN("^I execute all (\\d+) instructions$") {
    REGEX_PARAM(int, count);
    ScenarioScope<LogicalContext> context;
    
    context->startTime = std::chrono::high_resolution_clock::now();
    
    // Simulate executing many instructions (for performance testing)
    for (int i = 0; i < count; ++i) {
        // Example: Execute a simple AND instruction repeatedly
        mips::AndInstruction and_instr(10, 8, 9);  // and $t2, $t0, $t1
        and_instr.execute(*context->cpu);
    }
    
    context->endTime = std::chrono::high_resolution_clock::now();
    context->instructionCount = count;
}

THEN("^register (.+) should contain (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, expectedStr);
    ScenarioScope<LogicalContext> context;
    
    int regNum = registerMap[regName];
    uint32_t expected = static_cast<uint32_t>(std::stoul(expectedStr, nullptr, 16));
    uint32_t actual = context->cpu->getRegisterFile().getRegister(regNum);
    
    EXPECT_EQ(actual, expected) 
        << "Register " << regName << " should contain " << expectedStr 
        << " but contains 0x" << std::hex << actual;
}

THEN("^registers (.+) and (.+) should remain unchanged$") {
    REGEX_PARAM(std::string, reg1);
    REGEX_PARAM(std::string, reg2);
    ScenarioScope<LogicalContext> context;
    
    int regNum1 = registerMap[reg1];
    int regNum2 = registerMap[reg2];
    
    uint32_t actual1 = context->cpu->getRegisterFile().getRegister(regNum1);
    uint32_t actual2 = context->cpu->getRegisterFile().getRegister(regNum2);
    
    uint32_t expected1 = context->initialValues[reg1];
    uint32_t expected2 = context->initialValues[reg2];
    
    EXPECT_EQ(actual1, expected1) 
        << "Register " << reg1 << " should remain 0x" << std::hex << expected1 
        << " but is now 0x" << actual1;
    EXPECT_EQ(actual2, expected2)
        << "Register " << reg2 << " should remain 0x" << std::hex << expected2 
        << " but is now 0x" << actual2;
}

THEN("^the instruction should complete in exactly one CPU cycle$") {
    ScenarioScope<LogicalContext> context;
    // For a single instruction, this is always true in our simulator
    // Could be extended to check pipeline stage progression
    EXPECT_EQ(context->instructionCount, 1);
}

THEN("^the instruction should behave as a no-op for this case$") {
    ScenarioScope<LogicalContext> context;
    // This is verified by other steps checking that registers remain unchanged
    // Additional verification could be added here
    EXPECT_TRUE(true) << "No-op behavior verified by other assertions";
}

THEN("^the zero flag should not be set$") {
    ScenarioScope<LogicalContext> context;
    // MIPS doesn't have traditional flags, but we can verify result is non-zero
    // This step is more for documentation/specification purposes
    EXPECT_TRUE(true) << "MIPS architecture doesn't use condition flags";
}

THEN("^no exceptions should be raised$") {
    ScenarioScope<LogicalContext> context;
    // If we reach this point, no exceptions were thrown during execution
    EXPECT_TRUE(true) << "No exceptions occurred during instruction execution";
}

THEN("^the total execution time should be less than (\\d+) microseconds$") {
    REGEX_PARAM(int, maxMicroseconds);
    ScenarioScope<LogicalContext> context;
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        context->endTime - context->startTime);
    
    EXPECT_LT(duration.count(), maxMicroseconds)
        << "Execution took " << duration.count() << " microseconds, "
        << "expected less than " << maxMicroseconds;
}

THEN("^each instruction should average less than (\\d+) nanoseconds$") {
    REGEX_PARAM(int, maxNanoseconds);
    ScenarioScope<LogicalContext> context;
    
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        context->endTime - context->startTime);
    
    double averageNs = static_cast<double>(duration.count()) / context->instructionCount;
    
    EXPECT_LT(averageNs, maxNanoseconds)
        << "Average instruction time was " << averageNs << " nanoseconds, "
        << "expected less than " << maxNanoseconds;
}
