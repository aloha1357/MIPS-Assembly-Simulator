#include <gtest/gtest.h>
#include "Cpu.h"
#include "RegisterFile.h"
#include <map>
#include <string>

/**
 * @brief BDD-style test for core instructions feature
 * 
 * This simulates the Cucumber scenarios until we get cucumber-cpp working
 */
class CoreInstructionsBDD : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        // Background: Given an empty RegisterFile initialised to zeros
        // Background: And an empty Data Memory initialised to zeros
        cpu->reset();
    }

    void TearDown() override {
        cpu.reset();
    }

    // Helper methods for BDD-style assertions
    void given_register_contains(const std::string& regName, int value) {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register: " << regName;
        cpu->getRegisterFile().write(regNum, static_cast<uint32_t>(value));
    }

    void when_program_executed_for_cycles(const std::string& program, int cycles) {
        cpu->loadProgramFromString(program);
        cpu->run(cycles);
    }

    void then_register_should_equal(const std::string& regName, int expected) {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register: " << regName;
        
        uint32_t actual = cpu->getRegisterFile().read(regNum);
        EXPECT_EQ(actual, static_cast<uint32_t>(expected))
            << "Register " << regName << " (reg " << regNum << ") should equal " 
            << expected << " but was " << actual;
    }

private:
    std::unique_ptr<mips::Cpu> cpu;
    
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
        return -1;
    }
};

// Scenario Outline: add writes the sum of two registers into rd
// Examples from the feature file
TEST_F(CoreInstructionsBDD, Add_t0_t1_to_t2_3_plus_5_equals_8) {
    // Given register $t0 contains 3
    given_register_contains("$t0", 3);
    // And register $t1 contains 5  
    given_register_contains("$t1", 5);
    
    // When the program "add $t2, $t0, $t1" is executed for 1 cycle
    when_program_executed_for_cycles("add $t2, $t0, $t1", 1);
    
    // Then register $t2 should equal 8
    then_register_should_equal("$t2", 8);
}

TEST_F(CoreInstructionsBDD, Add_s0_s1_to_s2_negative1_plus_1_equals_0) {
    // Given register $s0 contains -1
    given_register_contains("$s0", -1);
    // And register $s1 contains 1
    given_register_contains("$s1", 1);
    
    // When the program "add $s2, $s0, $s1" is executed for 1 cycle
    when_program_executed_for_cycles("add $s2, $s0, $s1", 1);
    
    // Then register $s2 should equal 0
    then_register_should_equal("$s2", 0);
}

TEST_F(CoreInstructionsBDD, Add_a0_a1_to_v0_0_plus_0_equals_0) {
    // Given register $a0 contains 0 (already zero from reset)
    given_register_contains("$a0", 0);
    // And register $a1 contains 0
    given_register_contains("$a1", 0);
    
    // When the program "add $v0, $a0, $a1" is executed for 1 cycle
    when_program_executed_for_cycles("add $v0, $a0, $a1", 1);
    
    // Then register $v0 should equal 0
    then_register_should_equal("$v0", 0);
}
