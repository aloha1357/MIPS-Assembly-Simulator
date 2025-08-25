la $a0, label2
trap print_int
jalr $a0
trap exit
label2:
llo $a0, 888
trap print_int
addi $a0, $ra, 0
trap print_int
trap exit
