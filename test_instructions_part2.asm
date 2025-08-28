# Test Part 2: Jump and branch operations  
la $a0, label1
trap print_int
jr $a0
trap exit
label1:
trap print_int
la $a0, label2
trap print_int
jalr $a0
trap exit
label2:
trap print_int
addi $a0, $ra, 0
trap print_int
trap exit
