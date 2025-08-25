# 完整指令測試 - 簡化版（不使用.data段和syscall）
# 測試所有基本MIPS指令功能

.text
main:
    # ====================================
    # R-type 算術指令測試
    # ====================================
    addi $t0, $zero, 10     # $t0 = 10
    addi $t1, $zero, 5      # $t1 = 5
    
    add $t2, $t0, $t1       # $t2 = 15
    sub $t3, $t0, $t1       # $t3 = 5
    addu $t4, $t0, $t1      # $t4 = 15 (unsigned)
    subu $t5, $t0, $t1      # $t5 = 5 (unsigned)
    
    # ====================================
    # R-type 邏輯指令測試
    # ====================================
    addi $t0, $zero, 255    # $t0 = 255 (0xFF)
    addi $t1, $zero, 240    # $t1 = 240 (0xF0)
    
    and $t2, $t0, $t1       # $t2 = 240
    or $t3, $t0, $t1        # $t3 = 255
    xor $t4, $t0, $t1       # $t4 = 15
    nor $t5, $t0, $t1       # $t5 = ~255
    
    # ====================================
    # R-type 比較指令測試
    # ====================================
    addi $t0, $zero, 5      # $t0 = 5
    addi $t1, $zero, 10     # $t1 = 10
    
    slt $t2, $t0, $t1       # $t2 = 1 (5 < 10)
    slt $t3, $t1, $t0       # $t3 = 0 (10 not < 5)
    sltu $t4, $t0, $t1      # $t4 = 1 (5 < 10 unsigned)
    
    # ====================================
    # R-type 位移指令測試
    # ====================================
    addi $t0, $zero, 8      # $t0 = 8
    sll $t1, $t0, 2         # $t1 = 32 (8 << 2)
    srl $t2, $t1, 1         # $t2 = 16 (32 >> 1)
    sra $t3, $t0, 1         # $t3 = 4 (8 >> 1 arithmetic)
    
    # ====================================
    # R-type 變數位移指令測試
    # ====================================
    addi $t0, $zero, 16     # $t0 = 16
    addi $t1, $zero, 2      # $t1 = 2 (shift amount)
    
    sllv $t2, $t0, $t1      # $t2 = 64 (16 << 2)
    srlv $t3, $t0, $t1      # $t3 = 4 (16 >> 2)
    srav $t4, $t0, $t1      # $t4 = 4 (16 >> 2 arithmetic)
    
    # ====================================
    # R-type 乘除法指令測試
    # ====================================
    addi $t0, $zero, 6      # $t0 = 6
    addi $t1, $zero, 4      # $t1 = 4
    
    mult $t0, $t1           # HI:LO = 6 * 4 = 24
    mflo $t2                # $t2 = LO = 24
    mfhi $t3                # $t3 = HI = 0
    
    div $t0, $t1            # LO = 6/4 = 1, HI = 6%4 = 2
    mflo $t4                # $t4 = 1 (quotient)
    mfhi $t5                # $t5 = 2 (remainder)
    
    # ====================================
    # I-type 立即數指令測試
    # ====================================
    addi $t0, $zero, 100    # $t0 = 100
    addiu $t1, $zero, 200   # $t1 = 200
    andi $t2, $t0, 15       # $t2 = 100 & 15 = 4
    ori $t3, $t0, 15        # $t3 = 100 | 15 = 111
    xori $t4, $t0, 15       # $t4 = 100 ^ 15 = 107
    
    slti $t5, $t0, 150      # $t5 = 1 (100 < 150)
    sltiu $t6, $t0, 50      # $t6 = 0 (100 not < 50)
    
    lui $t7, 0x1234         # $t7 = 0x12340000
    
    # ====================================
    # I-type 記憶體指令測試
    # ====================================
    addi $sp, $zero, 1000   # 設置堆疊指針
    addi $t0, $zero, 0x12345678  # 測試數據
    
    sw $t0, 0($sp)          # 存儲word
    lw $t1, 0($sp)          # 載入word
    
    sb $t0, 4($sp)          # 存儲byte
    lb $t2, 4($sp)          # 載入byte (sign-extended)
    lbu $t3, 4($sp)         # 載入byte (zero-extended)
    
    sh $t0, 8($sp)          # 存儲halfword
    lh $t4, 8($sp)          # 載入halfword (sign-extended)
    lhu $t5, 8($sp)         # 載入halfword (zero-extended)
    
    # ====================================
    # I-type 條件分支指令測試
    # ====================================
    addi $t0, $zero, 5      # $t0 = 5
    addi $t1, $zero, 5      # $t1 = 5
    
    beq $t0, $t1, branch1   # 相等，應該跳躍
    addi $t2, $zero, 999    # 不應該執行
    
branch1:
    addi $t2, $zero, 1      # $t2 = 1 (分支成功)
    
    bne $t0, $t1, branch2   # 不相等，不應該跳躍
    addi $t3, $zero, 2      # 應該執行
    j branch2_skip
    
branch2:
    addi $t3, $zero, 999    # 不應該執行
    
branch2_skip:
    addi $t0, $zero, 0      # $t0 = 0 for BLEZ test
    blez $t0, branch3       # <= 0，應該跳躍
    addi $t4, $zero, 999    # 不應該執行
    
branch3:
    addi $t4, $zero, 3      # $t4 = 3 (分支成功)
    
    addi $t0, $zero, 1      # $t0 = 1 for BGTZ test
    bgtz $t0, branch4       # > 0，應該跳躍
    addi $t5, $zero, 999    # 不應該執行
    
branch4:
    addi $t5, $zero, 4      # $t5 = 4 (分支成功)
    
    # ====================================
    # R-type 跳躍指令測試
    # ====================================
    jal subroutine          # 調用子程序
    addi $t6, $zero, 5      # 返回後執行
    
    # ====================================
    # J-type 跳躍指令測試
    # ====================================
    j end_test              # 無條件跳躍到結束
    
subroutine:
    addi $t7, $zero, 6      # $t7 = 6 (子程序執行)
    jr $ra                  # 返回
    
end_test:
    # 程序結束，所有暫存器應該有預期值
    # $t2=1, $t3=2, $t4=3, $t5=4, $t6=5, $t7=6
    add $v0, $t2, $t3       # $v0 = 1 + 2 = 3
    add $v0, $v0, $t4       # $v0 = 3 + 3 = 6  
    add $v0, $v0, $t5       # $v0 = 6 + 4 = 10
    add $v0, $v0, $t6       # $v0 = 10 + 5 = 15
    add $v0, $v0, $t7       # $v0 = 15 + 6 = 21
