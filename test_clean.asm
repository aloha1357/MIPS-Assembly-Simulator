llo $a0, 1
trap print_int
llo $a0, 2
trap print_int
la $a0, label1
trap print_int
label1:
llo $a0, 777
trap print_int
trap exit
