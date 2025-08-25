lhi $a0, 0
llo $a0, 20
lhi $a1, 0
llo $a1, 12345
sw $a1, 0 ($a0)
lw $a2, 0 ($a0)
trap print_int
trap exit
