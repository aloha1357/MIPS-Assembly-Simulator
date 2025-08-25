# 測試 instructions.asm 的第一部分 (前30行)
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

# 正常結束，不跳躍到label1
trap exit
