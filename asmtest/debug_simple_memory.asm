# 簡單記憶體測試 - 診斷 part6 問題
la $a1, data
addi $a0, $a1, 0  # 複製位址到 $a0 
trap print_int   # 印出資料段位址

lw $a0, 0($a1)  # 載入第一個字
trap print_int   # 應該印出 255

lw $a0, 4($a1)  # 載入第二個字  
trap print_int   # 應該印出 65535

trap exit

data:
.word 255
.word 65535