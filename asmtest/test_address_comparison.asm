la $a1, data
trap print_int
lhi $a0, 0
llo $a0, 20
lw $a1, 0 ($a0)
trap print_int
trap exit
data:
.word 12345
