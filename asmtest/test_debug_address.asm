la $a1, data
trap print_int
lw $a0, 0 ($a1)
trap print_int
trap exit
data:
.word 12345
