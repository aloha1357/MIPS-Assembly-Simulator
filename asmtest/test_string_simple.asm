# Simple String Test
# Test basic string printing functionality

# Store a simple 2-character string "OK" + null terminator
# 'O' = 0x4F, 'K' = 0x4B, '\0' = 0x00
# Little-endian word: 0x004B4F00
addi $t0, $zero, 0x4F       # 'O'
addi $t1, $zero, 0x4B       # 'K' 
sll $t1, $t1, 8            # shift 'K' to position 8-15
add $t0, $t0, $t1          # combine 'O' + 'K' = "OK\0\0"
sw $t0, 0x100($zero)       # store at address 0x100

# Print the string
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x100     # string address
syscall                    # Should print "OK"

# Print integer to confirm syscall works
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 123       # test number
syscall                    # Should print "123"

# Exit
addi $v0, $zero, 10        # syscall 10: exit
syscall
