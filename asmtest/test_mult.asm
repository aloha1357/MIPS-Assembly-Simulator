llo $a0, 5
lhi $a0, 0
llo $a1, 3
lhi $a1, 0
mult $a0, $a1
mflo $a0
trap print_int
trap exit
