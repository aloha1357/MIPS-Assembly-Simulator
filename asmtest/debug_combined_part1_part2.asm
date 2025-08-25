# 組合測試：Part1 + Part2 (測試是否有組合問題)
# Part1內容
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

# Part2內容 (跳躍部分)
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
