llo $a0, 1
trap print_int
llo $a0, 10
trap print_character
llo $a0, 2
trap print_int
llo $a0, 10
trap print_character
la $a0, end_label
trap print_int
llo $a0, 10
trap print_character
llo $a0, 4
trap print_int
llo $a0, 10
trap print_character
trap exit
end_label:
llo $a0, 999
trap print_int
llo $a0, 10
trap print_character
trap exit
