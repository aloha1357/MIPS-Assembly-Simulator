# 測試指令數計算
llo $a0, 1      # 指令 0
llo $a0, 2      # 指令 1  
llo $a0, 3      # 指令 2
la $a0, data    # 指令 3 - 載入data標籤位址
trap print_int  # 指令 4 - 應該印出 20 (5*4)
trap exit       # 指令 5

data:
.word 999
