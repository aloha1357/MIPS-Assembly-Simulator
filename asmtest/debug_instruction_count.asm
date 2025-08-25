# 測試指令數量計算
llo $a0, 1       # 指令1
llo $a0, 2       # 指令2 
llo $a0, 3       # 指令3
llo $a0, 4       # 指令4
la $a0, test_data # 指令5 - 載入資料段位址
trap print_int   # 指令6 - 印出位址 (應該是 6*4=24)
trap exit        # 指令7

test_data:
.word 999
