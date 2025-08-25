llo $a0, 0
lhi $a0, 0
trap print_int
addi $a1, $a0, 0
addi $a0, $a1, 0
trap print_int
beq $a0, $a1, label3
j end
label3:
bne $a0, $a1, end
blez $a0, label4
j end
label4:
llo $a1, 1
bgtz $a1, label5
j end
label5:
addiu $a0, $a0, 1
trap print_int
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
trap exit
end:
trap exit
data:
.word 255
.word 65535
.byte 123
.byte 111
