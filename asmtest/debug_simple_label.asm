.data
test_byte: .byte 255

.text
main:
    # 輸出初始狀態標記
    addi $a0, $zero, 88  # 88 = debug marker
    addi $v0, $zero, 1
    syscall
    
    # 嘗試獲取 test_byte 位址
    la $a1, test_byte
    addi $a0, $a1, 0    # copy address to $a0
    addi $v0, $zero, 1
    syscall
    
    # 結束
    addi $v0, $zero, 10
    syscall
