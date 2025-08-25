la $a0, label1
trap print_int
llo $a0, 10
trap print_character
jr $a0
trap exit
label1:
llo $a0, 123
trap print_int
llo $a0, 10
trap print_character
trap exit
