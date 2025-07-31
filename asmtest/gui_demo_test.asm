# MIPS Assembly GUI Demo Program
# This program demonstrates the GUI functionality of the MIPS Simulator

# Load two test numbers
addi $t0, $zero, 15     # Load 15 into $t0
addi $t1, $zero, 25     # Load 25 into $t1

# Perform arithmetic operations
add $t2, $t0, $t1       # $t2 = 15 + 25 = 40
sub $t3, $t1, $t0       # $t3 = 25 - 15 = 10

# Store results in memory
sw $t2, 0($zero)        # Store 40 at memory address 0
sw $t3, 4($zero)        # Store 10 at memory address 4

# Load values back from memory
lw $s0, 0($zero)        # Load value from address 0 into $s0
lw $s1, 4($zero)        # Load value from address 4 into $s1

# Print the results using syscalls
addi $v0, $zero, 1      # syscall 1: print_int
add $a0, $zero, $s0     # Move first result to $a0
syscall                 # Print 40

addi $v0, $zero, 1      # syscall 1: print_int
add $a0, $zero, $s1     # Move second result to $a0  
syscall                 # Print 10

# Exit the program
addi $v0, $zero, 10     # syscall 10: exit
syscall
