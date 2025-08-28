# Test Part 3: HI/LO registers and multiplication/division
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
llo $a0, 65530
lhi $a0, 65535
llo $a1, 65535
lhi $a1, 65535
mult $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
llo $a0, 65530
lhi $a0, 65535
llo $a1, 65535
lhi $a1, 65535
multu $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
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
