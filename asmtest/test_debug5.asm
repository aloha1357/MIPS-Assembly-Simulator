lhi $a0, 1
llo $a0, 0
trap print_int
sll $a0, $a0, 1
trap print_int
srl $a0, $a0, 2
trap print_int
lhi $a0, 65535
llo $a0, 65535
trap print_int
sra $a0, $a0, 1
trap print_int
srl $a0, $a0, 1
trap print_int
lhi $a0, 0
trap print_int
trap exit
