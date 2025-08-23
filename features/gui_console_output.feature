Feature: GUI Console Output
  As a student learning MIPS assembly
  I want to see program output in the GUI console
  So that I can verify my programs work correctly

  Scenario: Display string output in GUI console
    Given I have the GUI simulator initialized in headless mode
    And I load a program that prints a string "Test"
    When I execute the program
    Then the GUI console should contain "Test"

  Scenario: Display integer output in GUI console  
    Given I have the GUI simulator initialized in headless mode
    And I load a program that prints integer 42
    When I execute the program
    Then the GUI console should contain "42"

  Scenario: Display mixed string and integer output
    Given I have the GUI simulator initialized in headless mode
    And I load a program that prints "Result: " and integer 123
    When I execute the program  
    Then the GUI console should contain "Result: 123"

  Scenario: Clear console output between program runs
    Given I have the GUI simulator initialized in headless mode
    And I have executed a program that produced output
    When I reset the simulator
    Then the GUI console should be empty

  @ignore
  Scenario: Console handles MIPS instruction decode errors
    Given I have the GUI simulator initialized in headless mode
    And I load a program with invalid 32-bit instruction words
    When I execute the program
    Then the GUI console should contain error messages about decode failures

  @ignore
  Scenario: Console shows memory access operations
    Given I have the GUI simulator initialized in headless mode
    And I load a program that performs LW and SW operations
    When I execute the program in debug mode
    Then the GUI console should show memory access details

  @ignore
  Scenario: Console displays register file operations
    Given I have the GUI simulator initialized in headless mode
    And I load a program that modifies multiple registers
    When I execute the program step by step
    Then the GUI console should show register value changes

  @ignore
  Scenario: Console handles all supported syscalls
    Given I have the GUI simulator initialized in headless mode
    And I load a program that uses all 4 syscalls (print_int, print_string, read_int, exit)
    When I execute the program
    Then the GUI console should handle each syscall appropriately

  @ignore
  Scenario: Console performance with large output
    Given I have the GUI simulator initialized in headless mode
    And I load a program that generates extensive console output
    When I execute the program
    Then the GUI console should handle large amounts of text efficiently
