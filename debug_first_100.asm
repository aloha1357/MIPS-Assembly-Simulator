lhi $a0, 1
llo $a0, 0
trap print_int
sll $a0, $a0, 1
trap print_int
srl $a0, $a0, 2
trap print_int
lhi $a0, 65535
llo $a0, 65535
trap print_int
sra $a0, $a0, 1
trap print_int
srl $a0, $a0, 1
trap print_int
lhi $a0, 0
trap print_int
sra $a0, $a0, 1
trap print_int
llo $a1, 1
sllv $a0, $a0, $a1
trap print_int
srlv $a0, $a0, $a1
trap print_int
lhi $a0, 65535
llo $a0, 65535
trap print_int
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
llo $a0, 0
lhi $a0, 1
trap print_int
mthi $a0
lhi $a0, 0
mfhi $a0
trap print_int
llo $a0, 1234
lhi $a0, 5678
trap print_int
mtlo $a0
lhi $a0, 0
mflo $a0
trap print_int
llo $a0, 65530
lhi $a0, 65535
llo $a1, 65535
lhi $a1, 65535
mult $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
llo $a0, 65530
lhi $a0, 65535
llo $a1, 65535
lhi $a1, 65535
multu $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
llo $a0, 1234
lhi $a0, 0
llo $a1, 65526
lhi $a1, 65535
div $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
llo $a0, 1234
lhi $a0, 0
llo $a1, 65526
lhi $a1, 65535
divu $a0, $a1
mflo $a0
trap print_int
mfhi $a0
trap print_int
llo $a1, 10
lhi $a1, 0
llo $a2, 2
lhi $a2, 0
add $a0, $a1, $a2
trap print_int
llo $a1, 9
addu $a0, $a1, $a2
trap print_int
trap exit
