llo $a0, 0
lhi $a0, 0
trap print_int
addi $a1, $a0, 0
addi $a0, $a1, 0
trap print_int
beq $a0, $a1, label3
trap print_int
trap exit
label3:
trap print_int
trap exit
