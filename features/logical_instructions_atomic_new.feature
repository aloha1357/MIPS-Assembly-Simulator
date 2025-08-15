# language: en

Feature: Atomic Logical Instructions Implementation
  As a MIPS programmer
  I want to perform bitwise logical operations
  So that I can efficiently manipulate data at the bit level

  Background:
    Given I have a MIPS CPU simulator
    And the CPU is in a clean state with all registers set to zero
    And the program counter is at address 0

  # ====================================
  # Scenario 1: AND Logical Instruction
  # ====================================

  Scenario Outline: AND instruction performs bitwise AND operation
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "and <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected>
    And register <source1> should remain unchanged at <value1>
    And register <source2> should remain unchanged at <value2>
    And the instruction should complete in exactly 1 CPU cycle

    Examples:
      | source1 | source2 | dest | value1     | value2     | expected   |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000 |
      | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0x00000000 |
      | $t3     | $t4     | $t5  | 0xFFFFFFFF | 0xFFFFFFFF | 0xFFFFFFFF |
      | $a0     | $a1     | $v0  | 0x12345678 | 0xFFFFF000 | 0x12345000 |
      | $t6     | $t7     | $t8  | 0x00000000 | 0xFFFFFFFF | 0x00000000 |

  Scenario: AND instruction register aliasing test
    Given register $t0 contains 0xDEADBEEF
    When I execute the instruction "and $t0, $t0, $t0"
    Then register $t0 should contain 0xDEADBEEF
    And the instruction should behave as identity operation

  # ====================================
  # Scenario 2: OR Logical Instruction
  # ====================================

  Scenario Outline: OR instruction performs bitwise OR operation
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "or <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected>
    And source registers should remain unchanged

    Examples:
      | source1 | source2 | dest | value1     | value2     | expected   |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0xFFFFFFFF |
      | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0xFFFFFFFF |
      | $t3     | $t4     | $t5  | 0x00000000 | 0x00000000 | 0x00000000 |
      | $a0     | $a1     | $v0  | 0x12340000 | 0x00005678 | 0x12345678 |
      | $t6     | $t7     | $t8  | 0xDEADBEEF | 0x00000000 | 0xDEADBEEF |

  # ====================================
  # Scenario 3: XOR Logical Instruction
  # ====================================

  Scenario Outline: XOR instruction performs bitwise XOR operation
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "xor <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected>

    Examples:
      | source1 | source2 | dest | value1     | value2     | expected   |
      | $t0     | $t1     | $t2  | 0xAAAAAAAA | 0x55555555 | 0xFFFFFFFF |
      | $s0     | $s1     | $s2  | 0x12345678 | 0x12345678 | 0x00000000 |
      | $t3     | $t4     | $t5  | 0xDEADBEEF | 0x00000000 | 0xDEADBEEF |
      | $a0     | $a1     | $v0  | 0xFFFFFFFF | 0xAAAAAAAA | 0x55555555 |
      | $t6     | $t7     | $t8  | 0x0F0F0F0F | 0xF0F0F0F0 | 0xFFFFFFFF |

  Scenario: XOR instruction encryption application test
    Given register $t0 contains 0x12345678
    And register $t1 contains 0xABCDEF00
    When I execute the instruction "xor $t2, $t0, $t1"
    And I execute the instruction "xor $t3, $t2, $t1"
    Then register $t3 should contain 0x12345678
    And this should demonstrate XOR reversibility

  # ====================================
  # Scenario 4: NOR Logical Instruction
  # ====================================

  Scenario Outline: NOR instruction performs bitwise NOR operation
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "nor <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected>

    Examples:
      | source1 | source2 | dest | value1     | value2     | expected   |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000 |
      | $s0     | $s1     | $s2  | 0x00000000 | 0x00000000 | 0xFFFFFFFF |
      | $t3     | $t4     | $t5  | 0xAAAAAAAA | 0x00000000 | 0x55555555 |
      | $a0     | $a1     | $v0  | 0xFFFFFFFF | 0xFFFFFFFF | 0x00000000 |

  Scenario: NOR instruction as universal NOT operation
    Given register $t0 contains 0xDEADBEEF
    And register $zero contains 0x00000000
    When I execute the instruction "nor $t1, $t0, $zero"
    Then register $t1 should contain 0x21524110
    And this should be equivalent to NOT operation

  # ====================================
  # Scenario 5: Complex Logical Operations
  # ====================================

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
    And register $t5 should contain 0x688AA886

  # ====================================
  # Scenario 6: Performance Tests
  # ====================================

  Scenario: Logical instructions performance benchmark
    Given I have 1000 logical instructions prepared
    When I execute all 1000 instructions
    Then total execution time should be less than 10 microseconds
    And average instruction execution time should be less than 10 nanoseconds
