llo $a0, 65530
lhi $a0, 65535
llo $a1, 65535
lhi $a1, 65535
mult $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
trap exit
