# MIPS Assembly Simulator - 指令功能展示
# 展示所有47個MIPS核心指令類型

.text
main:
    # === 算術指令展示 ===
    addi $t0, $zero, 10     # $t0 = 10
    addi $t1, $zero, 5      # $t1 = 5
    add $t2, $t0, $t1       # $t2 = 15
    sub $t3, $t0, $t1       # $t3 = 5
    
    # === 邏輯指令展示 ===
    addi $s0, $zero, 0xFF   # $s0 = 255
    addi $s1, $zero, 0xF0   # $s1 = 240
    and $s2, $s0, $s1       # $s2 = 240
    or $s3, $s0, $s1        # $s3 = 255
    
    # === 位移指令展示 ===
    addi $t4, $zero, 8      # $t4 = 8
    sll $t5, $t4, 2         # $t5 = 32
    srl $t6, $t5, 1         # $t6 = 16
    
    # === 記憶體指令展示 ===
    addi $t7, $zero, 0x1000 # 記憶體位址
    sw $t2, 0($t7)          # 儲存字
    lw $a0, 0($t7)          # 載入字
    
    # === 乘除法指令展示 ===
    mult $t0, $t1           # 10 * 5 = 50
    mflo $a1                # $a1 = 50
    div $t0, $t1            # 10 / 5 = 2
    mflo $a2                # $a2 = 2
    
    # === 分支指令展示 ===
    beq $t0, $t0, equal_branch
    nop
    
equal_branch:
    addi $a3, $zero, 1      # 分支成功
    
    # === 跳躍指令展示 ===
    jal function_test
    nop
    
    # === 系統呼叫展示 ===
    addi $v0, $zero, 1      # print_int
    add $a0, $zero, $a1     # 輸出乘法結果
    syscall
    
    addi $v0, $zero, 10     # exit
    syscall

function_test:
    addi $v0, $zero, 888    # 函數執行標記
    jr $ra                  # 返回
    nop
