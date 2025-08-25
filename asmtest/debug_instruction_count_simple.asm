# 測試指令數計算
llo $a0, 1      # 指令 0
llo $a0, 2      # 指令 1  
llo $a0, 3      # 指令 2
trap exit       # 指令 3
# 所以 instructionAddress = 4
# actualDataStart = 4 * 4 = 16

data:
.word 999
