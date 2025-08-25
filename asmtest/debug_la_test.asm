# 測試 la 指令
la $a0, test_data   # 獲取 test_data 的位址
addi $v0, $zero, 1
syscall

lw $a0, 0($a0)      # 從該位址載入值
addi $v0, $zero, 1  
syscall

addi $v0, $zero, 10
syscall

test_data:
.word 555
