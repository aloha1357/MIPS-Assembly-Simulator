# language: en

Feature: Pipeline behaviour and hazard handling
  To achieve cycle‑accurate simulation, the CPU must insert bubbles or
  forward results exactly as documented in the MIPS architecture.

  Background:
    Given an empty RegisterFile initialised to zeros
    And   an empty Data Memory initialised to zeros

  # ────────────────────────────────────────────────
  # 1. Load‑use hazard stall (no forwarding possible)
  # ────────────────────────────────────────────────

  @ignore
  Scenario: lw followed by dependent add must stall 1 cycle
    When the program
    """
    lw   $t0, 0($a0)    # load word from address in $a0
    add  $t1, $t0, $t2  # RAW dependency on $t0
    """
    is executed for 3 cycles
    Then pipeline register ID/EX in cycle 2 should contain a bubble
    And  register $t1 should equal (value at [$a0] + $t2) after 3 cycles

  # ────────────────────────────────────────────────
  # 2. ALU‑ALU dependency resolved via EX‑forwarding
  # ────────────────────────────────────────────────

  @ignore
  Scenario: add followed by sub with forwarding completes in 2 cycles
    Given register $t0 contains 10
    And   register $t1 contains 3
    When  the program
    """
    add $t2, $t0, $t1   # produces 13
    sub $t3, $t2, $t1   # should forward 13 from EX stage
    """
    is executed for 2 cycles
    Then register $t3 should equal 10
    And  no stall cycles should be inserted

  # ────────────────────────────────────────────────
  # 3. Branch taken flushes following instruction
  # ────────────────────────────────────────────────

  @ignore
  Scenario: beq taken causes a single bubble flush
    Given register $s0 contains 5
    And   register $s1 contains 5
    When  the program
    """
    beq $s0, $s1, target  # branch taken
    addi $v0, $zero, 0    # should be flushed
    target:
    addi $v0, $zero, 99
    """
    is executed for 5 cycles
    Then register $v0 should equal 99
    And  exactly 1 bubble should appear in IF/ID during cycle 3

  # ────────────────────────────────────────────────
  # 4. Structural hazard — single memory port contention
  # ────────────────────────────────────────────────

  @ignore
  Scenario: simultaneous lw and sw cause MEM stage stall
    Given data memory address 0x1000 contains 0xAAAABBBB
    And   register $t1 contains 0xCCCCDDDD
    When  the program
    """
    lw  $t0, 0x1000($zero)
    sw  $t1, 0x1000($zero)
    """
    is executed for 3 cycles
    Then data memory address 0x1000 should equal 0xCCCCDDDD
    And  exactly 1 bubble should appear in MEM stage during cycle 3
