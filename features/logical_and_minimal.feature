# language: en

Feature: Minimal AND Instruction Implementation
  As a MIPS programmer
  I want to perform AND bitwise logical operations
  So that I can efficiently manipulate data at the bit level

  Background:
    Given I have a MIPS CPU simulator
    And the CPU is in a clean state with all registers set to zero
    And the program counter is at address 0

  # ====================================
  # Scenario 1: AND 邏輯指令測試 - ONLY ENABLED SCENARIO
  # ====================================

  Scenario Outline: AND 指令執行位元 AND 運算
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "and <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected>
    And register <source1> should still contain <value1>
    And register <source2> should still contain <value2>
    And the instruction should complete in exactly 1 CPU cycle

    Examples:
      | source1 | source2 | dest | value1     | value2     | expected   | test_purpose                |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000 | complementary bit pattern   |
      | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0x00000000 | alternating bit pattern     |
      | $t3     | $t4     | $t5  | 0xFFFFFFFF | 0xFFFFFFFF | 0xFFFFFFFF | all ones AND                |
      | $a0     | $a1     | $v0  | 0x12345678 | 0xFFFFF000 | 0x12345000 | practical mask operation    |
      | $t6     | $t7     | $t8  | 0x00000000 | 0xFFFFFFFF | 0x00000000 | zero value AND              |
      | $t0     | $t0     | $t0  | 0xDEADBEEF | 0xDEADBEEF | 0xDEADBEEF | register aliasing (self-AND)|
