# Hello MIPS - 基礎字串輸出測試
# 這個程式展示如何在 MIPS 組合語言中輸出 "Hello MIPS!" 字串

# 在記憶體位址 0x1000 存儲字串 "Hello MIPS!"
# 我們需要將字符逐個組合成字組 (words)

# 第一個字組：存儲 "Hell" (4個字符)
addi $t0, $zero, 0x48       # 'H' = 0x48
addi $t1, $zero, 0x65       # 'e' = 0x65
addi $t2, $zero, 0x6C       # 'l' = 0x6C  
addi $t3, $zero, 0x6C       # 'l' = 0x6C

# 組合成一個字組：'H' + 'e'<<8 + 'l'<<16 + 'l'<<24
sll $t1, $t1, 8            # 將 'e' 移到位元組位置 1
sll $t2, $t2, 16           # 將 'l' 移到位元組位置 2  
sll $t3, $t3, 24           # 將 'l' 移到位元組位置 3
add $t0, $t0, $t1          # 組合 H + e
add $t0, $t0, $t2          # 組合 H + e + l
add $t0, $t0, $t3          # 組合 H + e + l + l = "Hell"
sw $t0, 0x1000($zero)      # 存儲 "Hell" 到記憶體位址 0x1000

# 第二個字組：存儲 "o MI" (4個字符)
addi $t0, $zero, 0x6F       # 'o' = 0x6F
addi $t1, $zero, 0x20       # ' ' = 0x20 (空格)
addi $t2, $zero, 0x4D       # 'M' = 0x4D
addi $t3, $zero, 0x49       # 'I' = 0x49

# 組合成一個字組
sll $t1, $t1, 8            # 將 ' ' 移到位元組位置 1
sll $t2, $t2, 16           # 將 'M' 移到位元組位置 2
sll $t3, $t3, 24           # 將 'I' 移到位元組位置 3
add $t0, $t0, $t1          # 組合 o + 空格
add $t0, $t0, $t2          # 組合 o + 空格 + M
add $t0, $t0, $t3          # 組合 o + 空格 + M + I = "o MI"
sw $t0, 0x1004($zero)      # 存儲 "o MI" 到記憶體位址 0x1004

# 第三個字組：存儲 "PS!" + null terminator (4個字符)
addi $t0, $zero, 0x50       # 'P' = 0x50
addi $t1, $zero, 0x53       # 'S' = 0x53
addi $t2, $zero, 0x21       # '!' = 0x21
# $t3 保持為 0，作為 null terminator

# 組合成一個字組
sll $t1, $t1, 8            # 將 'S' 移到位元組位置 1
sll $t2, $t2, 16           # 將 '!' 移到位元組位置 2
# null terminator 已經在位元組位置 3 (值為 0)
add $t0, $t0, $t1          # 組合 P + S
add $t0, $t0, $t2          # 組合 P + S + ! = "PS!\0"
sw $t0, 0x1008($zero)      # 存儲 "PS!\0" 到記憶體位址 0x1008

# 現在輸出完整的字串 "Hello MIPS!"
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # 字串起始位址
syscall                    # 輸出: "Hello MIPS!"

# 輸出換行符號讓輸出更好看
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 10        # ASCII 10 = '\n' (換行)
syscall

# 輸出一個簡單的數字測試
addi $v0, $zero, 1         # syscall 1: print_int  
addi $a0, $zero, 2025      # 年份 2025
syscall

# 再次輸出換行
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 10        # '\n'
syscall

# 正常結束程式
addi $v0, $zero, 10        # syscall 10: exit
syscall
