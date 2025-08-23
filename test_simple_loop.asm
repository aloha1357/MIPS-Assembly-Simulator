# test_simple_loop.asm - very simple loop for testing CLI
# Just a few instructions without branches

.text
main:
    addi $t0, $zero, 42     # Set $t0 to 42
    addi $v0, $zero, 1      # syscall 1 = print integer
    add $a0, $t0, $zero     # copy $t0 to $a0 for printing
    syscall                 # print the number
    
    addi $v0, $zero, 10     # syscall 10 = exit
    syscall                 # exit program
