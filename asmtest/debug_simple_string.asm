# Simple test without subroutines  
.text
main:
llo $a0, 42
trap print_int
la $a0, hello_msg
trap print_string
llo $a0, 10
trap print_character
trap exit

.data
hello_msg:
.asciiz "Hello"
