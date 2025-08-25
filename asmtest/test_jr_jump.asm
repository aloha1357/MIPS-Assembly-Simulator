llo $a0, 1
trap print_int
llo $a0, 10
trap print_character
la $a0, target
trap print_int
llo $a0, 10
trap print_character
jr $a0
llo $a0, 999
trap print_int
llo $a0, 10
trap print_character
trap exit
target:
llo $a0, 888
trap print_int
llo $a0, 10
trap print_character
trap exit
