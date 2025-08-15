# language: en

Feature: SLT Comparison Instruction Implementation (Scenario 5)
  As a MIPS programmer
  I want to perform signed less-than comparisons
  So that I can implement conditional logic and sorting algorithms

  Background:
    Given I have a MIPS CPU simulator
    And the CPU is in a clean state with all registers set to zero
    And the program counter is at address 0

  # ====================================
  # Scenario 5: SLT Comparison Instruction - 唯一啟用的 Scenario
  # ====================================

  Scenario Outline: SLT instruction sets register based on signed comparison
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "slt <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected>
    And register <source1> should remain unchanged at <value1>
    And register <source2> should remain unchanged at <value2>
    And the instruction should complete in exactly 1 CPU cycle

    Examples:
      | source1 | source2 | dest | value1 | value2 | expected |
      | $t0     | $t1     | $t2  | 5      | 10     | 1        |
      | $s0     | $s1     | $s2  | 10     | 5      | 0        |
      | $t3     | $t4     | $t5  | -5     | 5      | 1        |
      | $a0     | $a1     | $v0  | 5      | -5     | 0        |
      | $t6     | $t7     | $t8  | -10    | -5     | 1        |
      | $t0     | $t1     | $t2  | 42     | 42     | 0        |

  Scenario: SLT instruction with register aliasing
    Given register $t0 contains 100
    When I execute the instruction "slt $t0, $t0, $t0"
    Then register $t0 should contain 0
    And this demonstrates self-comparison always returns false

  Scenario: SLT instruction with boundary values
    Given register $t0 contains 0x7FFFFFFF
    And register $t1 contains 0x80000000
    When I execute the instruction "slt $t2, $t0, $t1"
    Then register $t2 should contain 0
    And this demonstrates positive vs negative boundary case
