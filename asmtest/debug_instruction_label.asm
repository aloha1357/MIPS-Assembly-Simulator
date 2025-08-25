# 測試指令標籤
la $a0, test_label  # 獲取指令標籤位址
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

test_label:
addi $a0, $zero, 333
addi $v0, $zero, 1
syscall
