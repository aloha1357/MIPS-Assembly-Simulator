# 完整指令測試 - 使用簡化指令格式
# 測試所有支持的指令

# 算術指令測試
lhi $t0, 0
llo $t0, 10     # $t0 = 10
lhi $t1, 0  
llo $t1, 5      # $t1 = 5

add $t2, $t0, $t1   # $t2 = 15
sub $t3, $t0, $t1   # $t3 = 5
addu $t4, $t0, $t1  # $t4 = 15
subu $t5, $t0, $t1  # $t5 = 5

# 輸出算術結果
mov $a0, $t2
trap print_int  # 應該輸出15
mov $a0, $t3
trap print_int  # 應該輸出5

# 邏輯指令測試
lhi $t0, 0
llo $t0, 255    # $t0 = 255
lhi $t1, 0
llo $t1, 240    # $t1 = 240

and $t2, $t0, $t1   # $t2 = 240
or $t3, $t0, $t1    # $t3 = 255
xor $t4, $t0, $t1   # $t4 = 15

# 輸出邏輯結果
mov $a0, $t2
trap print_int  # 應該輸出240
mov $a0, $t3
trap print_int  # 應該輸出255

# 比較指令測試
lhi $t0, 0
llo $t0, 5      # $t0 = 5
lhi $t1, 0
llo $t1, 10     # $t1 = 10

slt $t2, $t0, $t1   # $t2 = 1 (5 < 10)
sltu $t3, $t0, $t1  # $t3 = 1 (5 < 10 unsigned)

# 輸出比較結果
mov $a0, $t2
trap print_int  # 應該輸出1
mov $a0, $t3  
trap print_int  # 應該輸出1

# 位移指令測試
lhi $t0, 0
llo $t0, 8      # $t0 = 8

sll $t1, $t0, 2     # $t1 = 32 (8 << 2)
srl $t2, $t1, 1     # $t2 = 16 (32 >> 1)

# 輸出位移結果
mov $a0, $t1
trap print_int  # 應該輸出32
mov $a0, $t2
trap print_int  # 應該輸出16

# 記憶體指令測試
lhi $sp, 0
llo $sp, 1000   # 設置stack pointer
lhi $t0, 0x1234
llo $t0, 0x5678 # $t0 = 0x12345678

sw $t0, 0($sp)  # 存儲到記憶體
lw $t1, 0($sp)  # 載入回來

# 輸出記憶體測試結果
mov $a0, $t1
trap print_int  # 應該輸出原始值

# 程序結束
trap exit
