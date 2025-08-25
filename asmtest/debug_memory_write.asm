.data
test_byte: .byte 255
test_half: .halfword 65535

.text
main:
    # 輸出初始狀態標記
    addi $a0, $zero, 99  # 99 = memory test marker
    addi $v0, $zero, 1
    syscall
    
    # 輸出位址標記  
    la $a1, test_byte
    addi $a0, $a1, 0    # copy address to $a0
    addi $v0, $zero, 1
    syscall
    
    # 從 test_byte 位址載入並輸出值
    lb $a0, 0($a1)
    addi $v0, $zero, 1
    syscall
    
    # 輸出 test_half 位址
    la $a1, test_half
    addi $a0, $a1, 0
    addi $v0, $zero, 1
    syscall
    
    # 從 test_half 位址載入並輸出值
    lh $a0, 0($a1)
    addi $v0, $zero, 1
    syscall
    
    # 結束
    addi $v0, $zero, 10
    syscall
