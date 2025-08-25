# 測試資料段位址計算 - 修正版本
llo $a0, 5  # 設定 $a0 = 5
trap print_int  # 應該印出 5

la $a1, test_data  # 載入資料段位址到 $a1
addi $a0, $a1, 0   # 複製位址到 $a0 以便印出
trap print_int  # 印出資料段位址

lw $a0, 0($a1)  # 從資料段載入值
trap print_int  # 應該印出 42

trap exit

test_data:
.word 42
