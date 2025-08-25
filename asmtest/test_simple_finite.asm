lhi $a0, 0
llo $a0, 32
lhi $a1, 0
llo $a1, 77777
sw $a1, 0 ($a0)
lw $a2, 0 ($a0)
trap print_int
# Now check if data directive was written
lw $a3, 0 ($a0)
trap print_int
trap exit
.word 12345
