# 簡單的完整指令測試
.text
main:
    # 基本算術測試
    addi $t0, $zero, 10     # $t0 = 10
    addi $t1, $zero, 5      # $t1 = 5
    add $t2, $t0, $t1       # $t2 = 15
    
    # 邏輯運算測試  
    andi $t3, $t2, 7        # $t3 = 15 & 7 = 7
    
    # 記憶體測試
    addi $sp, $zero, 1000   # 設置stack pointer
    sw $t2, 0($sp)          # 存儲15到記憶體
    lw $t4, 0($sp)          # 載入回$t4
    
    # 分支測試
    beq $t2, $t4, success   # 如果相等就跳到success
    addi $v0, $zero, 999    # 錯誤值
    j end
    
success:
    addi $v0, $zero, 42     # 成功值
    
end:
    # $v0應該是42
