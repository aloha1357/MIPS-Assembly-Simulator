# Test Part 4: More arithmetic operations
llo $a1, 10
lhi $a1, 0
llo $a2, 2
lhi $a2, 0
add $a0, $a1, $a2
trap print_int
llo $a1, 9
addu $a0, $a1, $a2
trap print_int
sub $a0, $a1, $a2
trap print_int
llo $a1, 8
subu $a0, $a1, $a2
trap print_int
llo $a1, 3
llo $a2, 7
and $a0, $a1, $a2
trap print_int
or $a0, $a1, $a2
trap print_int
xor $a0, $a1, $a2
trap print_int
nor $a0, $a1, $a2
trap print_int
lhi $a1, 65535
slt $a0, $a1, $a2
trap print_int
slt $a0, $a2, $a1
trap print_int
sltu $a0, $a1, $a2
trap print_int
sltu $a0, $a2, $a1
trap print_int
trap exit
