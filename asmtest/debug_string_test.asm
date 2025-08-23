# Simple string test to debug console output issue

# Method 1: Store individual characters more simply
# Store "Hi!" at address 0x1000

# Store 'H' (0x48) at byte 0
addi $t0, $zero, 0x48       # 'H' = 72 = 0x48
sw $t0, 0x1000($zero)       # Store 'H' as a word

# Store 'i' (0x69) at next word  
addi $t0, $zero, 0x69       # 'i' = 105 = 0x69
sw $t0, 0x1004($zero)       # Store 'i' as a word

# Store null terminator
sw $zero, 0x1008($zero)     # Store 0 as null terminator

# Try to print string
addi $v0, $zero, 4          # syscall 4: print_string
addi $a0, $zero, 0x1000     # string address
syscall

# Also try printing integer to verify basic output works
addi $v0, $zero, 1          # syscall 1: print_int  
addi $a0, $zero, 123        # test value
syscall

# Exit
addi $v0, $zero, 10         # syscall 10: exit
syscall
