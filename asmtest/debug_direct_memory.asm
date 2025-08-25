# 最簡單的記憶體測試
lw $a0, 20($zero)    # 直接從位址 20 載入 (使用正確格式)
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

test_val:
.word 999
