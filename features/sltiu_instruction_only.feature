# language: en

Feature: SLTIU - Set Less Than Immediate Unsigned Instruction
  As a MIPS programmer
  I want to compare a register value with an unsigned immediate value
  So that I can implement conditional logic with immediate operands

  Background:
    Given an empty RegisterFile initialised to zeros
    And   an empty Data Memory initialised to zeros

  Scenario Outline: SLTIU sets register to 1 if rs < immediate (unsigned comparison)
    Given register <rs> contains <rs_val>
    When  the program
    """
    sltiu <rt>, <rs>, <imm>
    """
    is executed for 1 cycle
    Then  register <rt> should equal <expected>
    And   register <rs> should still equal <rs_val>

    Examples:
      | rs  | rt  | rs_val     | imm   | expected | description                    |
      | $t0 | $t1 | 5          | 10    | 1        | Small positive < larger        |
      | $s0 | $s1 | 10         | 5     | 0        | Larger positive >= smaller     |
      | $t2 | $t3 | 0          | 1     | 1        | Zero < any positive            |
      | $a0 | $v0 | 42         | 42    | 0        | Equal values (not less than)   |
      | $t4 | $t5 | 0xFFFFFFFF | 5     | 0        | Max unsigned > small positive  |
      | $s2 | $s3 | 5          | 65535 | 1        | Small < max immediate (16-bit) |

  Scenario: SLTIU should not modify source register
    Given register $t0 contains 0xDEADBEEF
    When  the program
    """
    sltiu $t1, $t0, 100
    """
    is executed for 1 cycle
    Then  register $t0 should equal 0xDEADBEEF
    And   register $t1 should equal 0

  Scenario: SLTIU with zero immediate
    Given register $t0 contains 1
    When  the program
    """
    sltiu $t1, $t0, 0
    """
    is executed for 1 cycle
    Then  register $t1 should equal 0
    And   register $t0 should equal 1
