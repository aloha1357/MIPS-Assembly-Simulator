#include "Cpu.h"
#include "Memory.h"
#include "RegisterFile.h"
#include <gtest/gtest.h>
#include <map>
#include <string>

/**
 * @brief BDD-style test for core instructions feature
 *
 * This simulates the Cucumber scenarios until we get cucumber-cpp working
 */
class CoreInstructionsBDD : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<mips::Cpu>();
        // Background: Given an empty RegisterFile initialised to zeros
        // Background: And an empty Data Memory initialised to zeros
        cpu->reset();
    }

    void TearDown() override
    {
        cpu.reset();
    }

    // Helper methods for BDD-style assertions
    void given_register_contains(const std::string& regName, int value)
    {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register: " << regName;
        cpu->getRegisterFile().write(regNum, static_cast<uint32_t>(value));
    }

    void when_program_executed_for_cycles(const std::string& program, int cycles)
    {
        cpu->loadProgramFromString(program);
        cpu->run(cycles);
    }

    void then_register_should_equal(const std::string& regName, int expected)
    {
        int regNum = getRegisterNumber(regName);
        ASSERT_NE(regNum, -1) << "Invalid register: " << regName;

        uint32_t actual = cpu->getRegisterFile().read(regNum);
        EXPECT_EQ(actual, static_cast<uint32_t>(expected))
            << "Register " << regName << " (reg " << regNum << ") should equal " << expected
            << " but was " << actual;
    }

    void given_memory_address_contains(uint32_t address, uint32_t value)
    {
        cpu->getMemory().writeWord(address, value);
    }

    void then_memory_address_should_equal(uint32_t address, uint32_t expected)
    {
        uint32_t actual = cpu->getMemory().readWord(address);
        EXPECT_EQ(actual, expected)
            << "Memory address 0x" << std::hex << address << std::dec << " should contain 0x"
            << std::hex << expected << " but was 0x" << std::hex << actual;
    }

  private:
    std::unique_ptr<mips::Cpu> cpu;

    std::map<std::string, int> registerMap = {
        {"$zero", 0}, {"$at", 1},  {"$v0", 2},  {"$v1", 3},  {"$a0", 4},  {"$a1", 5},  {"$a2", 6},
        {"$a3", 7},   {"$t0", 8},  {"$t1", 9},  {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, {"$t5", 13},
        {"$t6", 14},  {"$t7", 15}, {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19}, {"$s4", 20},
        {"$s5", 21},  {"$s6", 22}, {"$s7", 23}, {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27},
        {"$gp", 28},  {"$sp", 29}, {"$fp", 30}, {"$ra", 31}};

    int getRegisterNumber(const std::string& regName)
    {
        auto it = registerMap.find(regName);
        if (it != registerMap.end())
        {
            return it->second;
        }
        return -1;
    }
};

// Scenario Outline: add writes the sum of two registers into rd
// Examples from the feature file
TEST_F(CoreInstructionsBDD, Add_t0_t1_to_t2_3_plus_5_equals_8)
{
    // Given register $t0 contains 3
    given_register_contains("$t0", 3);
    // And register $t1 contains 5
    given_register_contains("$t1", 5);

    // When the program "add $t2, $t0, $t1" is executed for 1 cycle
    when_program_executed_for_cycles("add $t2, $t0, $t1", 1);

    // Then register $t2 should equal 8
    then_register_should_equal("$t2", 8);
}

TEST_F(CoreInstructionsBDD, Add_s0_s1_to_s2_negative1_plus_1_equals_0)
{
    // Given register $s0 contains -1
    given_register_contains("$s0", -1);
    // And register $s1 contains 1
    given_register_contains("$s1", 1);

    // When the program "add $s2, $s0, $s1" is executed for 1 cycle
    when_program_executed_for_cycles("add $s2, $s0, $s1", 1);

    // Then register $s2 should equal 0
    then_register_should_equal("$s2", 0);
}

TEST_F(CoreInstructionsBDD, Add_a0_a1_to_v0_0_plus_0_equals_0)
{
    // Given register $a0 contains 0 (already zero from reset)
    given_register_contains("$a0", 0);
    // And register $a1 contains 0
    given_register_contains("$a1", 0);

    // When the program "add $v0, $a0, $a1" is executed for 1 cycle
    when_program_executed_for_cycles("add $v0, $a0, $a1", 1);

    // Then register $v0 should equal 0
    then_register_should_equal("$v0", 0);
}

// Next scenario: sub subtracts rt from rs and writes into rd
TEST_F(CoreInstructionsBDD, Sub_t0_t1_to_t2_5_minus_3_equals_2)
{
    // Given register $t0 contains 5
    given_register_contains("$t0", 5);
    // And register $t1 contains 3
    given_register_contains("$t1", 3);

    // When the program "sub $t2, $t0, $t1" is executed for 1 cycle
    when_program_executed_for_cycles("sub $t2, $t0, $t1", 1);

    // Then register $t2 should equal 2
    then_register_should_equal("$t2", 2);
}

TEST_F(CoreInstructionsBDD, Sub_s0_s1_to_s2_1_minus_1_equals_0)
{
    // Given register $s0 contains 1
    given_register_contains("$s0", 1);
    // And register $s1 contains 1
    given_register_contains("$s1", 1);

    // When the program "sub $s2, $s0, $s1" is executed for 1 cycle
    when_program_executed_for_cycles("sub $s2, $s0, $s1", 1);

    // Then register $s2 should equal 0
    then_register_should_equal("$s2", 0);
}

// Scenario Outline: addi adds sign-extended immediate to rs
// Examples from the feature file
TEST_F(CoreInstructionsBDD, Addi_t0_plus_10_to_t1_5_plus_10_equals_15)
{
    // Given register $t0 contains 5
    given_register_contains("$t0", 5);

    // When the program "addi $t1, $t0, 10" is executed for 1 cycle
    when_program_executed_for_cycles("addi $t1, $t0, 10", 1);

    // Then register $t1 should equal 15
    then_register_should_equal("$t1", 15);
}

TEST_F(CoreInstructionsBDD, Addi_s0_minus_4_to_s1_3_minus_4_equals_negative1)
{
    // Given register $s0 contains 3
    given_register_contains("$s0", 3);

    // When the program "addi $s1, $s0, -4" is executed for 1 cycle
    when_program_executed_for_cycles("addi $s1, $s0, -4", 1);

    // Then register $s1 should equal -1
    then_register_should_equal("$s1", -1);
}

// Scenario: lw loads word from memory into register
TEST_F(CoreInstructionsBDD, Lw_loads_word_from_memory_into_register)
{
    // Given data memory address 0x1000 contains value 0xDEADBEEF
    given_memory_address_contains(0x1000, 0xDEADBEEF);

    // When the program "lw $t0, 0x1000($zero)" is executed for 2 cycles
    when_program_executed_for_cycles("lw $t0, 0x1000($zero)", 2);

    // Then register $t0 should equal 0xDEADBEEF
    then_register_should_equal("$t0", static_cast<int>(0xDEADBEEF));
}

// Scenario: sw stores register value into memory
TEST_F(CoreInstructionsBDD, Sw_stores_register_value_into_memory)
{
    // Given register $t1 contains 0xCAFEBABE
    given_register_contains("$t1", static_cast<int>(0xCAFEBABE));

    // When the program "sw $t1, 0x2000($zero)" is executed for 2 cycles
    when_program_executed_for_cycles("sw $t1, 0x2000($zero)", 2);

    // Then data memory address 0x2000 should equal 0xCAFEBABE
    then_memory_address_should_equal(0x2000, 0xCAFEBABE);
}

// Control flow instruction BDD tests
// Simplified version - delay slot handling will be implemented later
TEST_F(CoreInstructionsBDD, Beq_taken_branch_basic)
{
    // Given register $t0 contains 1
    given_register_contains("$t0", 1);
    // And register $t1 contains 1
    given_register_contains("$t1", 1);

    // When the program with branch is executed
    std::string program = R"(
        beq $t0, $t1, target
        addi $v0, $zero, 0
        target:
        addi $v0, $zero, 42
    )";
    when_program_executed_for_cycles(program, 6);

    // Then register $v0 should equal 42
    then_register_should_equal("$v0", 42);
}

TEST_F(CoreInstructionsBDD, J_unconditional_jump_basic)
{
    // Given register $v0 contains 0 (initial state)
    given_register_contains("$v0", 0);

    // When the program with jump is executed
    std::string program = R"(
        j target
        addi $v0, $zero, 0
        target:
        addi $v0, $zero, 7
    )";
    when_program_executed_for_cycles(program, 6);

    // Then register $v0 should equal 7
    then_register_should_equal("$v0", 7);
}
