# MIPS Assembly System Call Demo
# This program demonstrates the newly implemented system calls

# Print "Hello, MIPS!"
addi $v0, $zero, 4      # syscall 4: print_string
addi $a0, $zero, 0x1000 # string address
syscall

# Print integer 42
addi $v0, $zero, 1      # syscall 1: print_int  
addi $a0, $zero, 42     # value to print
syscall

# Print newline character (ASCII 10)
addi $v0, $zero, 1      # syscall 1: print_int
addi $a0, $zero, 10     # newline character
syscall

# Read an integer from user
addi $v0, $zero, 5      # syscall 5: read_int
syscall
# Result will be in $v0

# Print the read integer
add $a0, $v0, $zero     # move read value to $a0
addi $v0, $zero, 1      # syscall 1: print_int
syscall

# Exit program  
addi $v0, $zero, 10     # syscall 10: exit
syscall
