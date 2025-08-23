# MIPS Assembly System Call Demo
# This program demonstrates the newly implemented system calls

# First, let's store a string in memory at address 0x1000
# "Hi!\0" = 0x00216948 (little-endian: H=0x48, i=0x69, !=0x21, \0=0x00)
addi $t0, $zero, 0x48       # 'H' 
addi $t1, $zero, 0x69       # 'i'
addi $t2, $zero, 0x21       # '!'
# Combine into one word: 'H' + 'i'<<8 + '!'<<16 + '\0'<<24
sll $t1, $t1, 8            # shift 'i' to position 8-15
sll $t2, $t2, 16           # shift '!' to position 16-23
add $t0, $t0, $t1          # combine H + i
add $t0, $t0, $t2          # combine H + i + !
sw $t0, 0x1000($zero)      # store "Hi!\0" at address 0x1000

# Now print the string
addi $v0, $zero, 4      # syscall 4: print_string
addi $a0, $zero, 0x1000 # string address
syscall                 # Should print "Hi!"

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
