# Test exact sequence from instructions.asm lines 145-165
addi $a0, $zero, 1
addiu $a0, $a0, 6
andi $a0, $a0, 11
trap print_int
ori $a0, $a0, 17
trap print_int
xori $a0, $a0, 7
trap print_int
la $a1, data
lb $a0, 0 ($a1)
trap print_int
trap exit

data:
.word 255
.word 65535
.byte 123
.byte 111
