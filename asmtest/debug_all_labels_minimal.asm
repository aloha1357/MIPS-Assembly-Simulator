# 測試所有標籤的最小版本
lhi $a0, 1
llo $a0, 0
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
la $a0, end
trap print_int
j end
label3:
trap print_int
label4:
trap print_int
label5:
trap print_int
label7:
trap print_int
end:
trap exit
data:
.word 255
strings:
.asciiz "Hello, World!"
