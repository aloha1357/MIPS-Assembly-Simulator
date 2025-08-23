# language: en

Feature: System Call Support
  To provide a complete MIPS simulation experience, the simulator must support
  standard MIPS system calls for I/O operations and program control.

  Background:
    Given an empty RegisterFile initialised to zeros
    And   an empty Data Memory initialised to zeros
    And   the system call handler is ready

  # ────────────────────────────────────────────────
  # Basic I/O System Calls
  # ────────────────────────────────────────────────

  Scenario: print_int outputs integer to console
    Given register $v0 contains 1
    And   register $a0 contains 42
    When  the program
    """
    syscall
    """
    is executed for 1 cycle
    Then  the console output should contain "42"

  Scenario: print_string outputs string to console
    Given register $v0 contains 4
    And   register $a0 contains address 0x1000
    And   data memory starting at 0x1000 contains string "Hello World"
    When  the program
    """
    syscall
    """
    is executed for 1 cycle
    Then  the console output should contain "Hello World"

  Scenario: read_int gets integer from user input
    Given register $v0 contains 5
    And   the console input buffer contains "123"
    When  the program
    """
    syscall
    """
    is executed for 1 cycle
    Then  register $v0 should equal 123

  Scenario: exit terminates program execution
    Given register $v0 contains 10
    When  the program
    """
    syscall
    """
    is executed for 1 cycle
    Then  the program should terminate successfully

  # ────────────────────────────────────────────────
  # 32-bit Instruction Decoding
  # ────────────────────────────────────────────────

  Scenario: decode 32-bit R-type instruction
    Given the 32-bit instruction word 0x01094020  # add $t0, $t0, $t1
    When  the instruction is decoded
    Then  the opcode should be 0
    And   the rs field should be 8   # $t0
    And   the rt field should be 9   # $t1  
    And   the rd field should be 8   # $t0
    And   the function code should be 32  # add

  Scenario: decode 32-bit I-type instruction
    Given the 32-bit instruction word 0x2109000A  # addi $t1, $t0, 10
    When  the instruction is decoded
    Then  the opcode should be 8     # addi
    And   the rs field should be 8   # $t0
    And   the rt field should be 9   # $t1
    And   the immediate should be 10

  Scenario: decode 32-bit J-type instruction
    Given the 32-bit instruction word 0x08000010  # j 0x40
    When  the instruction is decoded
    Then  the opcode should be 2     # j
    And   the jump target should be 0x40

  # ────────────────────────────────────────────────
  # Binary Program Loading
  # ────────────────────────────────────────────────

  Scenario: load binary program from file
    Given a binary file "test.bin" containing MIPS machine code
    When  the program is loaded from binary
    Then  the instruction memory should contain decoded instructions
    And   the program counter should be set to 0x00400000  # text segment start

  # ────────────────────────────────────────────────
  # Debugging and Tracing
  # ────────────────────────────────────────────────

  Scenario: single step execution with debugging
    Given the program
    """
    addi $t0, $zero, 5
    addi $t1, $t0, 3
    add  $t2, $t0, $t1
    """
    When  single step execution is enabled
    And   the program is executed step by step
    Then  each instruction should be traceable
    And   register states should be inspectable after each step
    And   memory states should be inspectable after each step
