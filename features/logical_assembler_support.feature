# language: en

Feature: Logical Instructions Assembler Support
  As a MIPS programmer
  I want the assembler to parse logical instructions
  So that I can write programs using AND, OR, XOR, NOR operations

  Background:
    Given I have a MIPS assembler
    And the assembler is in a clean state

  # ====================================
  # Scenario 1: AND 指令組譯器支援 - ONLY ENABLED SCENARIO
  # ====================================

  Scenario: Assembler should parse AND instruction correctly
    Given I have MIPS assembly code "and $t0, $t1, $t2"
    When I assemble the code
    Then the assembler should create an AND instruction
    And the instruction should have destination register $t0
    And the instruction should have source register $t1
    And the instruction should have source register $t2
    And no errors should be reported

  # ====================================
  # 其他 Scenario 暫時停用，等待第一個完成
  # ====================================

  @ignore
  Scenario: Assembler should parse OR instruction correctly
    Given I have MIPS assembly code "or $s0, $s1, $s2"
    When I assemble the code
    Then the assembler should create an OR instruction

  @ignore
  Scenario: Assembler should parse XOR instruction correctly
    Given I have MIPS assembly code "xor $a0, $a1, $a2"
    When I assemble the code
    Then the assembler should create an XOR instruction

  @ignore
  Scenario: Assembler should parse NOR instruction correctly
    Given I have MIPS assembly code "nor $v0, $v1, $t0"
    When I assemble the code
    Then the assembler should create a NOR instruction
