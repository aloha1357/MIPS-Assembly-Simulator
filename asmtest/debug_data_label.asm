.data
data: .byte 255

.text
main:
    addi $a0, $zero, 89  # 89 = debug marker
    addi $v0, $zero, 1
    syscall
    
    la $a1, data
    addi $a0, $a1, 0    # copy address to $a0
    addi $v0, $zero, 1
    syscall
    
    addi $v0, $zero, 10
    syscall
