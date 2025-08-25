# 測試標籤地址
la $a0, data
trap print_int
trap exit
data:
.word 123
