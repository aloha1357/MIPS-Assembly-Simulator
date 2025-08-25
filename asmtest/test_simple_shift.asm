lhi $a0, 1
llo $a0, 0
trap print_int
sll $a0, $a0, 1
trap print_int
trap exit
