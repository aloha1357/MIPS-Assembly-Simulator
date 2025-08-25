# 帶輸出的完整測試
.text
main:
    # 測試基本運算
    addi $t0, $zero, 10
    addi $t1, $zero, 5
    add $v0, $t0, $t1       # $v0 = 15 (這會是輸出)
    
    # 程序結束
