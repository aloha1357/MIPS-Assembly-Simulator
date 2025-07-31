# Detailed debugging program for console issue
# This matches exactly what was failing in the tests

# Test the exact same sequence that was failing
addi $t0, $zero, 0x48       # 'H' = 0x48
addi $t1, $zero, 0x69       # 'i' = 0x69  
sll $t1, $t1, 8            # shift 'i' to byte position 1
add $t0, $t0, $t1          # combine H + i = should be 0x6948
sw $t0, 0x1000($zero)      # store "Hi\0\0" at address 0x1000

# Print the string
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # string address
syscall                    # Should print "Hi"

# Exit
addi $v0, $zero, 10        # syscall 10: exit
syscall
