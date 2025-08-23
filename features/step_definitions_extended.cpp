#include <cucumber-cpp/autodetect.hpp>
#include <cassert>
#include <cstdint>

using cucumber::ScenarioScope;

/**
 * @file step_definitions_extended.cpp
 * @brief Step definitions for extended MIPS instructions (SLT, SLTI, etc.)
 * 
 * This file implements step definitions for extended MIPS instruction scenarios
 * following BDD (Behavior-Driven Development) methodology with atomic implementation.
 * 
 * Development Phase: A - Step Definition Phase
 * Current Scenario: SLT (Set Less Than) instruction implementation
 * 
 * According to BDD A→B→C cycle:
 * A. Define steps (current phase) - all functions return empty, tests should fail
 * B. Implement production code to pass tests
 * C. Refactor and regression test
 */

/**
 * @brief Test context for extended instruction scenarios
 */
struct ExtendedInstructionContext {
    // TODO: Add CPU simulator instance
    // TODO: Add register file access
    // TODO: Add memory access
    
    ExtendedInstructionContext() {
        // TODO: Initialize CPU simulator in Phase B
    }
    
    void resetCpu() {
        // TODO: Implement CPU reset in Phase B
    }
    
    void setRegister(const std::string& regName, int32_t value) {
        // TODO: Implement register setting in Phase B
    }
    
    int32_t getRegister(const std::string& regName) {
        // TODO: Implement register reading in Phase B
        return 0; // Placeholder - should cause test failures
    }
    
    void executeProgram(const std::string& program, int cycles) {
        // TODO: Implement program execution in Phase B
    }
};

// ====================================================================
// Background Steps - CPU Initialization
// ====================================================================

GIVEN("^an empty RegisterFile initialised to zeros$") {
    ScenarioScope<ExtendedInstructionContext> context;
    context->resetCpu();
    // TODO: Verify all registers are zero in Phase B
}

GIVEN("^an empty Data Memory initialised to zeros$") {
    ScenarioScope<ExtendedInstructionContext> context;
    // TODO: Initialize memory to zeros in Phase B
}

// ====================================================================
// SLT Instruction Step Definitions  
// ====================================================================

GIVEN("^register (\\$\\w+) contains (-?\\d+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(int32_t, value);
    
    ScenarioScope<ExtendedInstructionContext> context;
    context->setRegister(regName, value);
    // TODO: Implement register setting in Phase B
}

WHEN("^the program$") {
    ScenarioScope<ExtendedInstructionContext> context;
    // TODO: Parse multi-line program string in Phase B
    // This step should be followed by a DocString containing the program
}

WHEN("^is executed for (\\d+) cycle$") {
    REGEX_PARAM(int, cycles);
    
    ScenarioScope<ExtendedInstructionContext> context;
    // TODO: Execute the previously parsed program for specified cycles
    // TODO: Implement SLT instruction execution in Phase B
}

THEN("^register (\\$\\w+) should equal (-?\\d+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(int32_t, expected);
    
    ScenarioScope<ExtendedInstructionContext> context;
    int32_t actual = context->getRegister(regName);
    
    // TODO: Implement proper register verification in Phase B
    // For now, this should fail with actual=0, expected=1 for first test case
    EXPECT_EQ(expected, actual) << "Register " << regName 
                                << " should contain " << expected 
                                << " but contains " << actual;
}

// ====================================================================
// Additional Step Definitions for Future Instructions
// ====================================================================

// The following steps are placeholders for other instructions that will be 
// implemented in subsequent BDD cycles. They are currently empty to maintain
// the "only implement what's needed" principle.

// TODO: Add SLTI instruction steps when that scenario is enabled
// TODO: Add BNE instruction steps when that scenario is enabled  
// TODO: Add BGTZ instruction steps when that scenario is enabled
// TODO: Add ANDI/ORI instruction steps when those scenarios are enabled

// ====================================================================
// Phase A Summary
// ====================================================================

/*
 * PHASE A COMPLETION CHECKLIST:
 * 
 * ✅ Only SLT scenario enabled in feature file (others marked @ignore)
 * ✅ Step definitions created with empty implementations  
 * ✅ All functions return default/empty values
 * ✅ Tests should fail with "expected vs actual" errors
 * ⚠️  Next: Run cucumber/tests to verify RED state
 * 
 * Expected Test Results:
 * - Tests should FAIL due to unimplemented functionality
 * - Failure should be about wrong values (0 vs 1), not compilation errors
 * - This confirms step definitions are correctly defined
 * 
 * Next Phase B Tasks:
 * 1. Add SLT instruction class to Instruction.h
 * 2. Implement SLT::execute() method
 * 3. Add SLT to InstructionDecoder (function code 0x2A)
 * 4. Implement CPU simulation in step definitions
 * 5. Run tests to achieve GREEN state
 */
