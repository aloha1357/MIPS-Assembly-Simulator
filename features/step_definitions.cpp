#include <cucumber-cpp/autodetect.hpp>
#include <gtest/gtest.h>
#include "Cpu.h"
#include <memory>
#include <map>
#include <string>
#include <regex>

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
