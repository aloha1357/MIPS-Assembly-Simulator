# GUI Console Test Program - Enhanced Version
# This version creates a more comprehensive test for both string and integer output

# Test 1: Simple string "Hi"
# Store string "Hi\0" at memory address 0x1000 using correct byte order

# 'H' = 0x48, 'i' = 0x69, '\0' = 0x00
# For little-endian: word = 0x00006948 (H at lowest byte, then i, then \0)
addi $t0, $zero, 0x48       # 'H' in lowest byte
addi $t1, $zero, 0x69       # 'i' 
sll $t1, $t1, 8            # shift 'i' to byte position 1
add $t0, $t0, $t1          # combine H + i = 0x6948
sw $t0, 0x1000($zero)      # store "Hi\0\0" at address 0x1000

# Print the string "Hi"
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # load string address
syscall                    # Should print "Hi"

# Test 2: Print some integers to verify that part works
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 42        # value 42
syscall                    # Should print "42"

addi $v0, $zero, 1         # syscall 1: print_int  
addi $a0, $zero, 999       # value 999
syscall                    # Should print "999"

# Test 3: Another simple string "OK"
# Store string "OK\0" at address 0x2000
addi $t0, $zero, 0x4F       # 'O' = 0x4F
addi $t1, $zero, 0x4B       # 'K' = 0x4B
sll $t1, $t1, 8            # shift 'K' to position 1  
add $t0, $t0, $t1          # combine O + K = 0x4B4F
sw $t0, 0x2000($zero)      # store "OK\0\0" at address 0x2000

# Print the string "OK"
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x2000    # load string address
syscall                    # Should print "OK"

# Exit program properly
addi $v0, $zero, 10        # syscall 10: exit
syscall
