lhi $a0, 0
llo $a0, 24
# Write 255
lhi $a1, 0
llo $a1, 255
sw $a1, 0 ($a0)
# Write 123  
lhi $a1, 0
llo $a1, 123
sw $a1, 4 ($a0)
lw $a2, 0 ($a0)
trap print_int
lw $a2, 4 ($a0)
trap print_int
trap exit
