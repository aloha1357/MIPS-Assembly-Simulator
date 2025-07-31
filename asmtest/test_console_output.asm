# GUI Console Test Program
# Tests if string and integer printing work in the GUI

# Setup string "Test\0" at memory address 0x1000
addi $t0, $zero, 0x54       # 'T' (ASCII 84)
addi $t1, $zero, 0x65       # 'e' (ASCII 101)  
addi $t2, $zero, 0x73       # 's' (ASCII 115)
addi $t3, $zero, 0x74       # 't' (ASCII 116)

# Combine into word: 'T' + 'e'<<8 + 's'<<16 + 't'<<24
sll $t1, $t1, 8            # shift 'e' to position 8-15
sll $t2, $t2, 16           # shift 's' to position 16-23  
sll $t3, $t3, 24           # shift 't' to position 24-31
add $t0, $t0, $t1          # combine T + e
add $t0, $t0, $t2          # combine T + e + s
add $t0, $t0, $t3          # combine T + e + s + t
sw $t0, 0x1000($zero)      # store "Test" at address 0x1000

# Store null terminator in next word  
sw $zero, 0x1004($zero)    # store "\0\0\0\0" at address 0x1004

# Print the string using syscall 4
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # load string address into $a0
syscall                    # Should print "Test" to console

# Print space character (ASCII 32)
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 32        # space character
syscall

# Print integer 42
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 42        # value to print  
syscall                    # Should print "42" to console

# Print newline character (ASCII 10)
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 10        # newline character
syscall

# Exit program properly
addi $v0, $zero, 10        # syscall 10: exit
syscall
