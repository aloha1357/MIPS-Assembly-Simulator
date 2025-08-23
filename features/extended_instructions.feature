# language: en

Feature: Extended MIPS Instruction Set
  To provide a more complete MIPS simulation, the simulator should support
  additional common MIPS instructions beyond the basic 7.

  Background:
    Given an empty RegisterFile initialised to zeros
    And   an empty Data Memory initialised to zeros

  # ────────────────────────────────────────────────
  # Logical Operations (R-type)
  # ────────────────────────────────────────────────

  @ignore
  Scenario Outline: and performs bitwise AND operation
    Given register <rs> contains <rs_val>
    And   register <rt> contains <rt_val>
    When  the program
    """
    and <rd>, <rs>, <rt>
    """
    is executed for 1 cycle
    Then  register <rd> should equal <expected>

    Examples:
      | rs | rt | rd | rs_val | rt_val | expected |
      | $t0| $t1| $t2| 0xFF00 | 0x0F0F | 0x0F00   |
      | $s0| $s1| $s2| 0xFFFF | 0x0000 | 0x0000   |

  @ignore
  Scenario Outline: or performs bitwise OR operation
    Given register <rs> contains <rs_val>
    And   register <rt> contains <rt_val>
    When  the program
    """
    or <rd>, <rs>, <rt>
    """
    is executed for 1 cycle
    Then  register <rd> should equal <expected>

    Examples:
      | rs | rt | rd | rs_val | rt_val | expected |
      | $t0| $t1| $t2| 0xFF00 | 0x00FF | 0xFFFF   |
      | $s0| $s1| $s2| 0x0000 | 0x0000 | 0x0000   |

  # ────────────────────────────────────────────────
  # Shift Operations
  # ────────────────────────────────────────────────

  @ignore
  Scenario Outline: sll performs logical left shift
    Given register <rt> contains <rt_val>
    When  the program
    """
    sll <rd>, <rt>, <shamt>
    """
    is executed for 1 cycle
    Then  register <rd> should equal <expected>

    Examples:
      | rt | rd | rt_val | shamt | expected |
      | $t0| $t1| 0x0001 |   4   | 0x0010   |
      | $s0| $s1| 0x00FF |   8   | 0xFF00   |

  @ignore
  Scenario Outline: srl performs logical right shift
    Given register <rt> contains <rt_val>
    When  the program
    """
    srl <rd>, <rt>, <shamt>
    """
    is executed for 1 cycle
    Then  register <rd> should equal <expected>

    Examples:
      | rt | rd | rt_val | shamt | expected |
      | $t0| $t1| 0x0010 |   4   | 0x0001   |
      | $s0| $s1| 0xFF00 |   8   | 0x00FF   |

  # ────────────────────────────────────────────────
  # Comparison Operations
  # ────────────────────────────────────────────────

  Scenario Outline: slt sets rd to 1 if rs < rt (signed)
    Given register <rs> contains <rs_val>
    And   register <rt> contains <rt_val>
    When  the program
    """
    slt <rd>, <rs>, <rt>
    """
    is executed for 1 cycle
    Then  register <rd> should equal <expected>

    Examples:
      | rs | rt | rd | rs_val | rt_val | expected |
      | $t0| $t1| $t2|   5    |   10   |    1     |
      | $s0| $s1| $s2|   10   |   5    |    0     |
      | $a0| $a1| $v0|  -5    |   5    |    1     |

  Scenario Outline: slti sets rt to 1 if rs < immediate (signed)
    Given register <rs> contains <rs_val>
    When  the program
    """
    slti <rt>, <rs>, <imm>
    """
    is executed for 1 cycle
    Then  register <rt> should equal <expected>

    Examples:
      | rs | rt | rs_val | imm | expected |
      | $t0| $t1|   5    |  10 |    1     |
      | $s0| $s1|   10   |   5 |    0     |
      | $a0| $v0|  -5    |   0 |    1     |

  # ────────────────────────────────────────────────
  # Additional Branch Operations
  # ────────────────────────────────────────────────

  @ignore
  Scenario: bne branches when registers are not equal
    Given register $t0 contains 5
    And   register $t1 contains 3
    When  the program
    """
    bne $t0, $t1, target
    addi $v0, $zero, 1      # should be skipped
    target:
    addi $v0, $zero, 42
    """
    is executed for 4 cycles
    Then  register $v0 should equal 42

  @ignore
  Scenario: bgtz branches when register is greater than zero
    Given register $t0 contains 5
    When  the program
    """
    bgtz $t0, target
    addi $v0, $zero, 1      # should be skipped
    target:
    addi $v0, $zero, 42
    """
    is executed for 4 cycles
    Then  register $v0 should equal 42

  # ────────────────────────────────────────────────
  # Immediate Logical Operations
  # ────────────────────────────────────────────────

  Scenario Outline: andi performs bitwise AND with immediate
    Given register <rs> contains <rs_val>
    When  the program
    """
    andi <rt>, <rs>, <imm>
    """
    is executed for 1 cycle
    Then  register <rt> should equal <expected>

    Examples:
      | rs | rt | rs_val | imm    | expected |
      | $t0| $t1| 0xFFFF | 0x00FF | 0x00FF   |
      | $s0| $s1| 0xF0F0 | 0x0F0F | 0x0000   |

  @ignore
  Scenario Outline: ori performs bitwise OR with immediate
    Given register <rs> contains <rs_val>
    When  the program
    """
    ori <rt>, <rs>, <imm>
    """
    is executed for 1 cycle
    Then  register <rt> should equal <expected>

    Examples:
      | rs | rt | rs_val | imm    | expected |
      | $t0| $t1| 0xFF00 | 0x00FF | 0xFFFF   |
      | $s0| $s1| 0x0000 | 0x1234 | 0x1234   |
