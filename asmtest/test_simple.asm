# Very simple test - just print a number
addi $v0, $zero, 1      # syscall 1: print_int
addi $a0, $zero, 42     # value to print
syscall

addi $v0, $zero, 10     # syscall 10: exit
syscall
