# 測試 JAL/JALR 修復
jal test_jal

addi $a0, $zero, 111
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

test_jal:
addi $a0, $zero, 222
addi $v0, $zero, 1
syscall

jr $ra
