la $a0, label1
trap print_int
jr $a0
trap exit
label1:
llo $a0, 999
trap print_int
trap exit
