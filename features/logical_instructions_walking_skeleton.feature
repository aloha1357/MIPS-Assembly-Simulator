Feature: Logical Instructions Walking Skeleton
  As a MIPS programmer
  I want to perform bitwise logical operations
  So that I can manipulate data at the bit level

  Background:
    Given I have a MIPS CPU simulator
    And the CPU is in a clean state with all registers set to zero
    And the program counter is at address 0

  @walking-skeleton
  Scenario: AND instruction basic functionality (Walking Skeleton)
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F
    When I execute the instruction "and $t2, $t0, $t1"
    Then register $t2 should contain 0x00000000
    And register $t0 should remain unchanged at 0xF0F0F0F0
    And register $t1 should remain unchanged at 0x0F0F0F0F

  @ignore
  Scenario: OR instruction basic functionality
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F
    When I execute the instruction "or $t2, $t0, $t1"
    Then register $t2 should contain 0xFFFFFFFF

  @ignore
  Scenario: XOR instruction basic functionality
    Given register $t0 contains 0xAAAAAAAA
    And register $t1 contains 0x55555555
    When I execute the instruction "xor $t2, $t0, $t1"
    Then register $t2 should contain 0xFFFFFFFF

  @ignore
  Scenario: NOR instruction basic functionality
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F
    When I execute the instruction "nor $t2, $t0, $t1"
    Then register $t2 should contain 0x00000000
