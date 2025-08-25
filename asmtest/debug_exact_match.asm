# 完全匹配格式測試
la $a1, test_val
addi $a0, $a1, 0
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

test_val:
.word 123
