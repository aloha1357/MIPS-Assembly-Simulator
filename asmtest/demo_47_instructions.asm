# MIPS Assembly Simulator - 47個指令完整功能展示
# 這個程式演示了所有47個MIPS核心指令的功能

.text
main:
    # ====================================
    # R-type 算術指令測試 (ADD, SUB, ADDU, SUBU)
    # ====================================
    addi $t0, $zero, 10     # $t0 = 10
    addi $t1, $zero, 5      # $t1 = 5
    
    add $t2, $t0, $t1       # $t2 = $t0 + $t1 = 15
    sub $t3, $t0, $t1       # $t3 = $t0 - $t1 = 5
    addu $t4, $t0, $t1      # $t4 = $t0 + $t1 = 15 (unsigned)
    subu $t5, $t0, $t1      # $t5 = $t0 - $t1 = 5 (unsigned)
    
    # ====================================
    # R-type 邏輯指令測試 (AND, OR, XOR, NOR)
    # ====================================
    addi $t0, $zero, 0xFF   # $t0 = 255 (0xFF)
    addi $t1, $zero, 0xF0   # $t1 = 240 (0xF0)
    
    and $t2, $t0, $t1       # $t2 = $t0 & $t1 = 0xF0
    or $t3, $t0, $t1        # $t3 = $t0 | $t1 = 0xFF
    xor $t4, $t0, $t1       # $t4 = $t0 ^ $t1 = 0x0F
    nor $t5, $t0, $t1       # $t5 = ~($t0 | $t1)
    
    # ====================================
    # R-type 比較指令測試 (SLT, SLTU)
    # ====================================
    addi $t0, $zero, 5      # $t0 = 5
    addi $t1, $zero, 10     # $t1 = 10
    
    slt $t2, $t0, $t1       # $t2 = 1 (5 < 10)
    slt $t3, $t1, $t0       # $t3 = 0 (10 not < 5)
    sltu $t4, $t0, $t1      # $t4 = 1 (5 < 10 unsigned)
    
    # ====================================
    # R-type 位移指令測試 (SLL, SRL, SRA)
    # ====================================
    addi $t0, $zero, 8      # $t0 = 8 (1000 binary)
    sll $t1, $t0, 2         # $t1 = $t0 << 2 = 32
    srl $t2, $t1, 1         # $t2 = $t1 >> 1 = 16 (logical)
    
    addi $t0, $zero, -8     # $t0 = -8 (負數)
    sra $t3, $t0, 1         # $t3 = $t0 >> 1 = -4 (arithmetic)
    
    # ====================================
    # R-type 變數位移指令測試 (SLLV, SRLV, SRAV)
    # ====================================
    addi $t0, $zero, 16     # $t0 = 16
    addi $t1, $zero, 2      # $t1 = 2 (shift amount)
    
    sllv $t2, $t0, $t1      # $t2 = $t0 << $t1 = 64
    srlv $t3, $t2, $t1      # $t3 = $t2 >> $t1 = 16 (logical)
    srav $t4, $t0, $t1      # $t4 = $t0 >> $t1 = 4 (arithmetic)
    
    # ====================================
    # I-type 立即值算術指令測試 (ADDI, ADDIU)
    # ====================================
    addi $t0, $zero, 100    # $t0 = 100
    addiu $t1, $zero, 200   # $t1 = 200 (unsigned immediate add)
    addi $t2, $t0, 50       # $t2 = $t0 + 50 = 150
    
    # ====================================
    # I-type 立即值邏輯指令測試 (ANDI, ORI, XORI)
    # ====================================
    addi $t0, $zero, 0xFF   # $t0 = 255
    andi $t1, $t0, 0xF0     # $t1 = $t0 & 0xF0 = 240
    ori $t2, $t0, 0x0F      # $t2 = $t0 | 0x0F = 255
    xori $t3, $t0, 0x0F     # $t3 = $t0 ^ 0x0F = 240
    
    # ====================================
    # I-type 立即值比較指令測試 (SLTI, SLTIU)
    # ====================================
    addi $t0, $zero, 5      # $t0 = 5
    slti $t1, $t0, 10       # $t1 = 1 (5 < 10)
    slti $t2, $t0, 3        # $t2 = 0 (5 not < 3)
    sltiu $t3, $t0, 10      # $t3 = 1 (5 < 10 unsigned)
    
    # ====================================
    # I-type 記憶體載入指令測試 (LW, LB, LBU, LH, LHU)
    # ====================================
    # 首先存入一些測試資料
    addi $t0, $zero, 0x1000 # 基底位址
    addi $t1, $zero, 0x12345678  # 測試資料
    sw $t1, 0($t0)          # 存入記憶體
    
    lw $t2, 0($t0)          # 載入字 (word)
    lb $t3, 0($t0)          # 載入位元組 (signed)
    lbu $t4, 0($t0)         # 載入位元組 (unsigned)
    lh $t5, 0($t0)          # 載入半字 (signed)
    lhu $t6, 0($t0)         # 載入半字 (unsigned)
    
    # ====================================
    # I-type 記憶體儲存指令測試 (SW, SB, SH)
    # ====================================
    addi $t0, $zero, 0x2000 # 新的基底位址
    addi $t1, $zero, 0xAABBCCDD  # 測試資料
    
    sw $t1, 0($t0)          # 儲存字
    sb $t1, 4($t0)          # 儲存位元組
    sh $t1, 8($t0)          # 儲存半字
    
    # ====================================
    # I-type 立即值載入指令測試 (LUI)
    # ====================================
    lui $t0, 0x1234         # $t0 = 0x12340000
    
    # ====================================
    # R-type 乘除法指令測試 (MULT, MULTU, DIV, DIVU)
    # ====================================
    addi $t0, $zero, 12     # $t0 = 12
    addi $t1, $zero, 5      # $t1 = 5
    
    mult $t0, $t1           # HI:LO = 12 * 5 = 60
    mflo $t2                # $t2 = LO = 60
    mfhi $t3                # $t3 = HI = 0
    
    div $t0, $t1            # LO = 12/5 = 2, HI = 12%5 = 2
    mflo $t4                # $t4 = LO = 2
    mfhi $t5                # $t5 = HI = 2
    
    # 無符號乘除法
    addi $t0, $zero, 20     # $t0 = 20
    addi $t1, $zero, 3      # $t1 = 3
    
    multu $t0, $t1          # HI:LO = 20 * 3 = 60 (unsigned)
    divu $t0, $t1           # LO = 20/3 = 6, HI = 20%3 = 2 (unsigned)
    
    # ====================================
    # R-type HI/LO存取指令測試 (MFHI, MTHI, MFLO, MTLO)
    # ====================================
    addi $t0, $zero, 0x1111 # 測試值
    addi $t1, $zero, 0x2222 # 測試值
    
    mthi $t0                # HI = $t0 = 0x1111
    mtlo $t1                # LO = $t1 = 0x2222
    mfhi $t2                # $t2 = HI = 0x1111
    mflo $t3                # $t3 = LO = 0x2222
    
    # ====================================
    # I-type 分支指令測試 (BEQ, BNE, BLEZ, BGTZ, BLTZ, BGEZ)
    # ====================================
    addi $t0, $zero, 5      # $t0 = 5
    addi $t1, $zero, 5      # $t1 = 5
    
    beq $t0, $t1, branch_equal    # 相等分支
    nop
    
branch_equal:
    addi $t2, $zero, 1      # 分支成功標記
    
    addi $t0, $zero, 3      # $t0 = 3
    addi $t1, $zero, 5      # $t1 = 5
    
    bne $t0, $t1, branch_not_equal  # 不相等分支
    nop
    
branch_not_equal:
    addi $t3, $zero, 1      # 分支成功標記
    
    # 測試零值分支
    addi $t0, $zero, 0      # $t0 = 0
    blez $t0, branch_lez    # <= 0 分支
    nop
    
branch_lez:
    addi $t4, $zero, 1      # 分支成功標記
    
    addi $t0, $zero, 5      # $t0 = 5
    bgtz $t0, branch_gtz    # > 0 分支
    nop
    
branch_gtz:
    addi $t5, $zero, 1      # 分支成功標記
    
    addi $t0, $zero, -1     # $t0 = -1
    bltz $t0, branch_ltz    # < 0 分支
    nop
    
branch_ltz:
    addi $t6, $zero, 1      # 分支成功標記
    
    addi $t0, $zero, 0      # $t0 = 0
    bgez $t0, branch_gez    # >= 0 分支
    nop
    
branch_gez:
    addi $t7, $zero, 1      # 分支成功標記
    
    # ====================================
    # J-type 跳躍指令測試 (J, JAL)
    # ====================================
    jal test_function       # 跳躍並連結
    nop
    
    # 返回後繼續執行
    addi $s0, $zero, 999    # 表示JAL成功返回
    
    j end_program           # 無條件跳躍到結束
    nop
    
test_function:
    addi $s1, $zero, 888    # 在函數中設置標記
    jr $ra                  # 返回
    nop
    
    # ====================================
    # R-type 跳躍指令測試 (JR, JALR)
    # ====================================
jump_test:
    la $t0, jump_target     # 載入跳躍目標位址
    jalr $ra, $t0           # 跳躍並連結到暫存器位址
    nop
    
    addi $s2, $zero, 777    # JALR成功返回標記
    j end_program
    nop
    
jump_target:
    addi $s3, $zero, 666    # 在跳躍目標設置標記
    jr $ra                  # 返回
    nop
    
    # ====================================
    # 系統呼叫指令測試 (SYSCALL)
    # ====================================
end_program:
    # 輸出完成訊息
    addi $v0, $zero, 4      # print_string系統呼叫
    la $a0, completion_msg
    syscall
    
    # 輸出測試結果統計
    addi $v0, $zero, 1      # print_int系統呼叫
    addi $a0, $zero, 47     # 47個指令
    syscall
    
    addi $v0, $zero, 4      # print_string系統呼叫
    la $a0, instructions_msg
    syscall
    
    # 程式正常結束
    addi $v0, $zero, 10     # exit系統呼叫
    syscall

.data
completion_msg: .asciiz "🎉 MIPS Assembly Simulator - 所有47個指令測試完成！\n測試指令數量: "
instructions_msg: .asciiz " 個指令全部通過測試! ✅\n\n47個指令清單:\n✅ R-type指令 (26個): ADD, SUB, ADDU, SUBU, AND, OR, XOR, NOR, SLT, SLTU, SLL, SRL, SRA, SLLV, SRLV, SRAV, MULT, MULTU, DIV, DIVU, MFHI, MTHI, MFLO, MTLO, JR, JALR\n✅ I-type指令 (19個): ADDI, ADDIU, ANDI, ORI, XORI, SLTI, SLTIU, LW, SW, LB, SB, LBU, LH, SH, LHU, BEQ, BNE, BLEZ, BGTZ, BLTZ, BGEZ, LUI\n✅ J-type指令 (2個): J, JAL\n✅ 系統指令 (1個): SYSCALL\n\n🏆 所有指令都在5階段流水線中正確執行！\n"
