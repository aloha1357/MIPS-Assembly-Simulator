la $a1, strings
lbu $a0, 0 ($a1)
trap print_character
la $a0, strings
trap print_string
llo $a0, 10
trap print_character
trap exit
strings:
.asciiz "Hello, World!"
