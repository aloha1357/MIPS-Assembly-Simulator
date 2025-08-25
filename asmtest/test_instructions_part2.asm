# 測試 instructions.asm 的跳躍部分
lhi $a0, 65535
llo $a0, 65535
srav $a0, $a0, $a0
trap print_int
la $a0, label1
trap print_int
jr $a0
trap exit
label1:
trap print_int
la $a0, label2
trap print_int
jalr $a0
trap exit
label2:
trap print_int
addi $a0, $ra, 0
trap print_int
trap exit
