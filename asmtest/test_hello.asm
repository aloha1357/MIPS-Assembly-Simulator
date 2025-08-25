# Simple MIPS program for testing CLI
.text
main:
    # Print "Hello World!"
    addi $v0, $zero, 4      # syscall 4: print_string
    addi $a0, $zero, 0x1000 # string address
    syscall                 
    
    # Print the number 42
    addi $v0, $zero, 1      # syscall 1: print_int
    addi $a0, $zero, 42     # value to print
    syscall
    
    # Exit
    addi $v0, $zero, 10     # syscall 10: exit
    syscall

.data
0x1000: .asciiz "Hello World!"
