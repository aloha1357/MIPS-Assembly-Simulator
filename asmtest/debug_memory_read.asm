# 測試記憶體讀取
la $a1, mydata
addi $a0, $a1, 0  # 複製位址到 $a0
trap print_int    # 印出位址
lw $a0, 0($a1)   # 讀取資料
trap print_int    # 印出資料
trap exit

mydata:
.word 42
