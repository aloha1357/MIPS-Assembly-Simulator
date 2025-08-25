lhi $a0, 65535
llo $a0, 65535
trap print_int
srav $a0, $a0, $a0
trap print_int
la $a0, label1
trap print_int
jr $a0
trap exit
label1:
trap print_int
trap exit
