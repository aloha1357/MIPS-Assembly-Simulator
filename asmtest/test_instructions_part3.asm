llo $a0, 0
lhi $a0, 1
trap print_int
mthi $a0
lhi $a0, 0
mfhi $a0
trap print_int
llo $a0, 1234
lhi $a0, 5678
trap print_int
mtlo $a0
lhi $a0, 0
mflo $a0
trap print_int
trap exit
