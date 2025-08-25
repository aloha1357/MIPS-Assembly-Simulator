# 完整指令測試 - 混合格式
# 算術指令測試
lhi $t0, 0
llo $t0, 10     # $t0 = 10
lhi $t1, 0  
llo $t1, 5      # $t1 = 5

add $t2, $t0, $t1   # $t2 = 15
sub $t3, $t0, $t1   # $t3 = 5

# 輸出算術結果
addi $a0, $t2, 0    # 將$t2移動到$a0
trap print_int      # 應該輸出15

addi $a0, $t3, 0    # 將$t3移動到$a0  
trap print_int      # 應該輸出5

# 邏輯指令測試
lhi $t0, 0
llo $t0, 255    # $t0 = 255
lhi $t1, 0
llo $t1, 240    # $t1 = 240

and $t2, $t0, $t1   # $t2 = 240
or $t3, $t0, $t1    # $t3 = 255

# 輸出邏輯結果
addi $a0, $t2, 0
trap print_int      # 應該輸出240

addi $a0, $t3, 0
trap print_int      # 應該輸出255

# 程序結束
trap exit
