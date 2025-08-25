# 手動記憶體測試
# 先寫入一個值到記憶體
addi $t0, $zero, 888    # 要寫入的值
sw $t0, 100($zero)      # 寫入位址 100 (使用正確格式)

# 然後讀取並輸出
lw $a0, 100($zero)      # 從位址 100 讀取 (使用正確格式)
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall
