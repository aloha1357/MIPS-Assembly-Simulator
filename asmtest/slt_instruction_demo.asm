# MIPS SLT Instruction Demo Program
# 
# This program demonstrates the SLT (Set Less Than) instruction
# which compares two signed values and sets the destination register
# to 1 if the first operand is less than the second, otherwise 0.

.text
    # Test Case 1: Positive numbers - 5 < 10 should return 1
    addi $t0, $zero, 5       # $t0 = 5
    addi $t1, $zero, 10      # $t1 = 10
    slt $t2, $t0, $t1        # $t2 = (5 < 10) ? 1 : 0  -> should be 1
    
    # Print result 1
    addi $v0, $zero, 1       # syscall 1: print_int
    add $a0, $zero, $t2      # Load result
    syscall                  # Should print: 1
    
    # Test Case 2: Reverse comparison - 10 < 5 should return 0
    slt $t3, $t1, $t0        # $t3 = (10 < 5) ? 1 : 0  -> should be 0
    
    # Print result 2
    addi $v0, $zero, 1       # syscall 1: print_int
    add $a0, $zero, $t3      # Load result
    syscall                  # Should print: 0
    
    # Test Case 3: Negative vs positive - (-5) < 5 should return 1
    addi $t4, $zero, -5      # $t4 = -5
    addi $t5, $zero, 5       # $t5 = 5
    slt $t6, $t4, $t5        # $t6 = (-5 < 5) ? 1 : 0  -> should be 1
    
    # Print result 3
    addi $v0, $zero, 1       # syscall 1: print_int
    add $a0, $zero, $t6      # Load result
    syscall                  # Should print: 1
    
    # Test Case 4: Equal values - 42 < 42 should return 0
    addi $t7, $zero, 42      # $t7 = 42
    addi $t8, $zero, 42      # $t8 = 42
    slt $t9, $t7, $t8        # $t9 = (42 < 42) ? 1 : 0  -> should be 0
    
    # Print result 4
    addi $v0, $zero, 1       # syscall 1: print_int
    add $a0, $zero, $t9      # Load result
    syscall                  # Should print: 0
    
    # Exit program
    addi $v0, $zero, 10      # syscall 10: exit
    syscall

# Expected Console Output:
# 1
# 0 
# 1
# 0
