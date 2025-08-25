# 簡單標籤測試
addi $a0, $zero, 85  # 85 = debug marker
addi $v0, $zero, 1
syscall

la $a1, test_byte
addi $a0, $a1, 0    # copy address to $a0
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

test_byte:
.byte 255
