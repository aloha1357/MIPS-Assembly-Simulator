# Test lines 151-200 (memory operations and strings)
.text
main:
llo $a0, 20  # Test value first
trap print_int
la $a1, data
lb $a0, 0 ($a1)
trap print_int
lbu $a0, 0 ($a1)
trap print_int
lh $a0, 0 ($a1)
trap print_int
lhu $a0, 0 ($a1)
trap print_int
lh $a0, 4 ($a1)
trap print_int
lw $a0, 0 ($a1)
trap print_int
lb $a0, 0 ($a1)
trap print_int
llo $a0, 259
sb $a0, 8 ($a1)
lbu $a0, 8 ($a1)
trap print_int
llo $a0, 1
lhi $a0, 1
trap print_int
sh $a0, 8 ($a1)
lhu $a0, 8 ($a1)
trap print_int
lhi $a0, 32
sw $a0, 4 ($a1)
lw $a0, 4 ($a1)
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
end:
trap exit

.data
data:
.word 255
.word 65535
.byte 123
.byte 111
strings:
.asciiz "Hello, World!"
