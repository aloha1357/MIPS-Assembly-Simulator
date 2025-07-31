# Very simple console test
# Just test if integer printing works correctly

# Print integer 42
addi $v0, $zero, 1      # syscall 1: print_int  
addi $a0, $zero, 42     # value to print
syscall                 # Should print "42"

# Print integer 999  
addi $v0, $zero, 1      # syscall 1: print_int
addi $a0, $zero, 999    # value to print
syscall                 # Should print "999"

# Exit
addi $v0, $zero, 10     # syscall 10: exit
syscall
