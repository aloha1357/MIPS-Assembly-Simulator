la $a1, data
lw $a0, 0 ($a1)
trap print_int
trap exit
data:
.word 12345
