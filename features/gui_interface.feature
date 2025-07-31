# language: en

Feature: Graphical User Interface for MIPS Simulator
  As a computer science student or educator
  I want a graphical interface for the MIPS simulator
  So that I can visualize the execution process and debug programs more effectively

  Background:
    Given the GUI application is launched
    And the simulator is initialized

  # ────────────────────────────────────────────────
  # Main Window and Layout
  # ────────────────────────────────────────────────

  Scenario: Main window launches successfully
    When the application starts
    Then the main window should be visible
    And the window title should contain "MIPS Assembly Simulator"
    And the menu bar should be present
    And the toolbar should be present

  Scenario: Main window layout contains essential panels
    Given the main window is open
    Then the code editor panel should be visible
    And the register viewer panel should be visible
    And the memory viewer panel should be visible
    And the pipeline visualization panel should be visible
    And the console output panel should be visible

  # ────────────────────────────────────────────────
  # Code Editor Features
  # ────────────────────────────────────────────────

  Scenario: Code editor allows assembly input
    Given the code editor is focused
    When I type the following assembly code:
    """
    addi $t0, $zero, 5
    addi $t1, $zero, 10
    add $t2, $t0, $t1
    """
    Then the code should be displayed in the editor
    And syntax highlighting should be applied
    And line numbers should be visible

  Scenario: Code editor supports file operations
    Given a MIPS assembly file "test.asm" exists with content:
    """
    addi $t0, $zero, 42
    addi $v0, $zero, 1
    syscall
    """
    When I select "File" → "Open" from the menu
    And I choose the file "test.asm"
    Then the file content should be loaded into the editor
    And the window title should show "test.asm"

  Scenario: Code editor provides save functionality
    Given the code editor contains assembly code
    When I select "File" → "Save As" from the menu
    And I specify the filename "new_program.asm"
    Then the file should be saved to disk
    And the window title should update to show "new_program.asm"

  # ────────────────────────────────────────────────
  # Register Viewer
  # ────────────────────────────────────────────────

  Scenario: Register viewer displays all MIPS registers
    Given the register viewer is visible
    Then it should display 32 general-purpose registers
    And each register should show its name (e.g., "$t0", "$s0")
    And each register should show its decimal value
    And each register should show its hexadecimal value
    And the PC register should be displayed separately

  Scenario: Register values update during execution
    Given the following program is loaded:
    """
    addi $t0, $zero, 5
    addi $t1, $zero, 10
    add $t2, $t0, $t1
    """
    When I click the "Step" button
    Then register $t0 should show value 5
    When I click the "Step" button again
    Then register $t1 should show value 10
    When I click the "Step" button again
    Then register $t2 should show value 15

  Scenario: Register viewer highlights changed registers
    Given a program is executing
    When a register value changes
    Then the changed register should be highlighted
    And the highlight should fade after 2 seconds

  # ────────────────────────────────────────────────
  # Memory Viewer
  # ────────────────────────────────────────────────

  Scenario: Memory viewer displays memory contents
    Given the memory viewer is visible
    Then it should show memory addresses in hexadecimal
    And it should show memory contents in both hex and ASCII
    And it should support scrolling through memory ranges
    And it should have an address jump feature

  Scenario: Memory viewer updates during memory operations
    Given the following program is loaded:
    """
    addi $t0, $zero, 100
    sw $t0, 0($zero)
    lw $t1, 0($zero)
    """
    When I execute the program step by step
    Then after the "sw" instruction, memory address 0x00000000 should show value 100
    And the memory change should be highlighted

  Scenario: Memory viewer allows address navigation
    Given the memory viewer is open
    When I enter address "0x1000" in the address field
    And I press Enter
    Then the memory viewer should jump to address 0x1000
    And the current address should be highlighted

  # ────────────────────────────────────────────────
  # Pipeline Visualization
  # ────────────────────────────────────────────────

  Scenario: Pipeline visualization shows all 5 stages
    Given pipeline mode is enabled
    And the pipeline viewer is visible
    Then it should display 5 pipeline stages: IF, ID, EX, MEM, WB
    And each stage should have a visual representation
    And each stage should show its current instruction (if any)

  Scenario: Pipeline visualization updates during execution
    Given the following program is loaded in pipeline mode:
    """
    addi $t0, $zero, 1
    addi $t1, $zero, 2
    addi $t2, $zero, 3
    add $t3, $t0, $t1
    """
    When I click "Step" once
    Then the IF stage should show "addi $t0, $zero, 1"
    When I click "Step" again
    Then the IF stage should show "addi $t1, $zero, 2"
    And the ID stage should show "addi $t0, $zero, 1"

  Scenario: Pipeline visualization shows stage status
    Given pipeline mode is active
    When an instruction is in a specific stage
    Then that stage should be visually highlighted
    And the instruction details should be displayed
    And empty stages should be clearly marked as "BUBBLE" or "NOP"

  # ────────────────────────────────────────────────
  # Execution Controls
  # ────────────────────────────────────────────────

  Scenario: Execution control buttons are functional
    Given a program is loaded
    Then the "Run" button should be enabled
    And the "Step" button should be enabled
    And the "Reset" button should be enabled
    And the "Stop" button should be initially disabled

  Scenario: Step execution works correctly
    Given the following program is loaded:
    """
    addi $t0, $zero, 5
    addi $t1, $zero, 10
    """
    When I click the "Step" button
    Then exactly one instruction should be executed
    And the PC should advance by 1
    And the GUI should update to reflect the new state

  Scenario: Run execution works correctly
    Given a program with syscall exit is loaded:
    """
    addi $t0, $zero, 42
    addi $v0, $zero, 10
    syscall
    """
    When I click the "Run" button
    Then the program should execute until completion
    And the "Run" button should be disabled
    And the "Stop" button should be enabled during execution

  Scenario: Reset functionality works
    Given a program has been partially executed
    And registers contain non-zero values
    When I click the "Reset" button
    Then all registers should be reset to zero
    And the PC should be reset to 0
    And the memory should be reset
    And the pipeline should be cleared

  # ────────────────────────────────────────────────
  # Console Output
  # ────────────────────────────────────────────────

  Scenario: Console displays syscall output
    Given the following program is loaded:
    """
    addi $v0, $zero, 1
    addi $a0, $zero, 42
    syscall
    """
    When I execute the program
    Then the console should display "42"

  Scenario: Console supports input for read syscalls
    Given the following program is loaded:
    """
    addi $v0, $zero, 5
    syscall
    """
    When I execute the program
    Then the console should prompt for input
    When I type "123" and press Enter
    Then register $v0 should contain 123

  Scenario: Console displays execution messages
    Given a program is running
    When an instruction is executed
    Then the console should show execution progress
    And any errors should be displayed in red
    And successful operations should be displayed in green

  # ────────────────────────────────────────────────
  # Mode Switching
  # ────────────────────────────────────────────────

  Scenario: Switch between single-cycle and pipeline modes
    Given the simulator is in single-cycle mode
    When I select "View" → "Pipeline Mode" from the menu
    Then the pipeline visualization should become visible
    And the mode indicator should show "Pipeline Mode"
    And all execution should now use pipeline behavior

  Scenario: Mode switching preserves program state
    Given a program is loaded and partially executed in single-cycle mode
    When I switch to pipeline mode
    Then the register values should be preserved
    And the memory contents should be preserved
    And the PC value should be preserved

  # ────────────────────────────────────────────────
  # Error Handling and User Feedback
  # ────────────────────────────────────────────────

  Scenario: Invalid assembly code shows errors
    Given the code editor contains invalid assembly:
    """
    invalid_instruction $t0, $t1
    add $t0
    """
    When I attempt to load the program
    Then error messages should be displayed
    And the problematic lines should be highlighted
    And execution should be prevented

  Scenario: Runtime errors are handled gracefully
    Given the following program with a potential error:
    """
    lw $t0, 1000($zero)
    """
    When I execute the program
    And a memory access error occurs
    Then an error dialog should appear
    And the error details should be displayed
    And the execution should stop safely

  # ────────────────────────────────────────────────
  # Breakpoints and Debugging
  # ────────────────────────────────────────────────

  Scenario: Set breakpoints in code editor
    Given the code editor contains a program
    When I click in the left margin next to line 3
    Then a breakpoint should be set at line 3
    And a red dot should appear in the margin

  Scenario: Execution stops at breakpoints
    Given a program is loaded with a breakpoint at line 3
    When I click "Run"
    Then execution should stop at line 3
    And the current line should be highlighted
    And I should be able to inspect register and memory state

  Scenario: Remove breakpoints
    Given a breakpoint exists at line 3
    When I click on the breakpoint indicator
    Then the breakpoint should be removed
    And the red dot should disappear

  # ────────────────────────────────────────────────
  # Performance and Responsiveness
  # ────────────────────────────────────────────────

  Scenario: GUI remains responsive during execution
    Given a long-running program is loaded
    When I click "Run"
    Then the GUI should remain responsive
    And I should be able to click "Stop" to halt execution
    And the progress should be visible in real-time

  Scenario: Large programs load efficiently
    Given a MIPS assembly file with 1000+ instructions
    When I load the file
    Then the file should load within 2 seconds
    And the GUI should remain responsive
    And syntax highlighting should be applied correctly
