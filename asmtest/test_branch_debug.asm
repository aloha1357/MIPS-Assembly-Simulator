llo $a0, 0
lhi $a0, 0
llo $a1, 0
lhi $a1, 0
beq $a0, $a1, label3
llo $a0, 999
trap print_int
j end
label3:
llo $a0, 111
trap print_int
bne $a0, $a1, label4
llo $a0, 888
trap print_int
j end
label4:
llo $a0, 222
trap print_int
end:
trap exit
