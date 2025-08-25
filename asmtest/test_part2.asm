lhi $a0, 0
llo $a1, 1
sllv $a0, $a0, $a1
trap print_int
srlv $a0, $a0, $a1
trap print_int
lhi $a0, 65535
llo $a0, 65535
trap print_int
srav $a0, $a0, $a0
trap print_int
trap exit
