# 簡化的跳轉測試
la $a0, target_label
addi $v0, $zero, 1
syscall

jr $a0

target_label:
addi $a0, $zero, 999
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall
