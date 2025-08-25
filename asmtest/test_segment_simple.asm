lhi $a0, 65535
llo $a0, 65535
trap print_int
llo $a0, 10
trap print_character
srav $a0, $a0, $a0
trap print_int
llo $a0, 10
trap print_character
trap exit
