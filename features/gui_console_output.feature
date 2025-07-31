Feature: GUI Console Output
  As a student learning MIPS assembly
  I want to see program output in the GUI console
  So that I can verify my programs work correctly

  Scenario: Display string output in GUI console
    Given I have the GUI simulator initialized in headless mode
    And I load a program that prints a string "Test"
    When I execute the program
    Then the GUI console should contain "Test"

  @ignore
  Scenario: Display integer output in GUI console  
    Given I have the GUI simulator initialized in headless mode
    And I load a program that prints integer 42
    When I execute the program
    Then the GUI console should contain "42"

  @ignore
  Scenario: Display mixed string and integer output
    Given I have the GUI simulator initialized in headless mode
    And I load a program that prints "Result: " and integer 123
    When I execute the program  
    Then the GUI console should contain "Result: 123"

  @ignore
  Scenario: Clear console output between program runs
    Given I have the GUI simulator initialized in headless mode
    And I have executed a program that produced output
    When I reset the simulator
    Then the GUI console should be empty
