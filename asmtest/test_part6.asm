llo $a0, 1234
lhi $a0, 0
llo $a1, 65526
lhi $a1, 65535
div $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
llo $a0, 1234
lhi $a0, 0
llo $a1, 65526
lhi $a1, 65535
divu $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
trap exit
