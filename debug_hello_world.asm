# Test the exact string from instructions.asm
.text
main:
llo $a0, 99
trap print_int
la $a0, strings
trap print_string
trap exit

.data
data:
.word 255
.word 65535
.byte 123
.byte 111
strings:
.asciiz "Hello, World!"
