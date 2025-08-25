# Test exact flow from instructions.asm
.text
main:
llo $a0, 42
trap print_int
jal label7
j end

label7:
addi $a0, $ra, 0
trap print_int
la $a1, strings
lbu $a0, 0 ($a1)
trap print_character
la $a0, strings
trap print_string
llo $a0, 10
trap print_character
# Missing jr $ra here - this is the bug!

end:
trap exit

.data
strings:
.asciiz "Hello, World!"
