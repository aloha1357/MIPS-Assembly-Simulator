.text
main:
    addi $a0, $zero, 77
    addi $v0, $zero, 1
    syscall
    
    # 直接硬編碼測試
    addi $a0, $zero, 60  # 我們期望的 test_byte 位址
    addi $v0, $zero, 1
    syscall
    
    addi $v0, $zero, 10
    syscall
