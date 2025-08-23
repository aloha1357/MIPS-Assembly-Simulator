Feature: Core Logical Instructions
  As a MIPS programmer
  I want to perform bitwise logical operations
  So that I can manipulate data efficiently at the bit level

  Background:
    Given I have a MIPS CPU simulator
    And the CPU is in a clean state with all registers set to zero
    And the program counter is at address 0

  Scenario Outline: Logical AND instruction with different bit patterns
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "and <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected_result>
    And registers <source1> and <source2> should remain unchanged
    And the instruction should complete in exactly one CPU cycle

    Examples:
      | source1 | source2 | dest | value1     | value2     | expected_result |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000     |
      | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0x00000000     |
      | $t3     | $t4     | $t5  | 0xFFFFFFFF | 0xFFFFFFFF | 0xFFFFFFFF     |
      | $a0     | $a1     | $v0  | 0x12345678 | 0xFFFFF000 | 0x12345000     |
      | $t6     | $t7     | $t8  | 0x00000000 | 0xFFFFFFFF | 0x00000000     |

  Scenario: Logical OR instruction combines bits correctly
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F
    When I execute the instruction "or $t2, $t0, $t1"
    Then register $t2 should contain 0xFFFFFFFF
    And the zero flag should not be set
    And no exceptions should be raised

  Scenario: Logical XOR instruction toggles bits
    Given register $s0 contains 0xAAAAAAAA
    And register $s1 contains 0x55555555
    When I execute the instruction "xor $s2, $s0, $s1"
    Then register $s2 should contain 0xFFFFFFFF

  Scenario: Logical NOR instruction performs negated OR
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F
    When I execute the instruction "nor $t2, $t0, $t1"
    Then register $t2 should contain 0x00000000

  Scenario: Complex logical operation sequence
    Given register $t0 contains 0x12345678
    And register $t1 contains 0x87654321
    When I execute the following instruction sequence:
      """
      and $t2, $t0, $t1
      or  $t3, $t0, $t1
      xor $t4, $t0, $t1
      nor $t5, $t0, $t1
      """
    Then register $t2 should contain 0x02244220
    And register $t3 should contain 0x97755779
    And register $t4 should contain 0x95511559
    And register $t5 should contain 0x68AA8886

  Scenario: Logical operations with register aliasing
    Given register $t0 contains 0xFFFF0000
    When I execute the instruction "and $t0, $t0, $t0"
    Then register $t0 should contain 0xFFFF0000
    And the instruction should behave as a no-op for this case

  Scenario: Performance requirement verification
    Given I have 1000 logical instructions prepared
    When I execute all 1000 instructions
    Then the total execution time should be less than 10 microseconds
    And each instruction should average less than 10 nanoseconds
