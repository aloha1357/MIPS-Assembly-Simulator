# Hello MIPS - 簡化版本測試
# 這是一個簡化的 "Hello MIPS" 測試，只輸出基本字串

# 在記憶體位址 0x1000 存儲簡短字串 "Hi"
# 第一個字組：存儲 "Hi\0\0" (4個字節)
addi $t0, $zero, 0x48       # 'H' = 0x48
addi $t1, $zero, 0x69       # 'i' = 0x69

# 組合成一個字組：'H' + 'i'<<8 + '\0'<<16 + '\0'<<24
sll $t1, $t1, 8            # 將 'i' 移到位元組位置 1
add $t0, $t0, $t1          # 組合 H + i = "Hi\0\0"
sw $t0, 0x1000($zero)      # 存儲到記憶體位址 0x1000

# 輸出字串 "Hi"
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # 字串起始位址
syscall                    # 輸出: "Hi"

# 輸出一個數字
addi $v0, $zero, 1         # syscall 1: print_int  
addi $a0, $zero, 123       # 測試數字 123
syscall

# 正常結束程式
addi $v0, $zero, 10        # syscall 10: exit
syscall
