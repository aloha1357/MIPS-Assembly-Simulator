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
end:
trap exit
