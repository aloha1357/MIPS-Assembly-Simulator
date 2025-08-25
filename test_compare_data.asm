la $a0, data
lw $a1, 0 ($a0)
trap print_int
lw $a1, 4 ($a0)
trap print_int
trap exit
data:
.word 255
.word 123
