# True infinite loop for testing step limit and timeout
# This program is designed to never terminate on its own
# Use --limit or --timeout to stop it

addi $t0, $zero, 0     # Initialize counter

infinite_loop:
    addi $t0, $t0, 1    # Increment counter
    j infinite_loop     # Jump back forever - no exit syscall
