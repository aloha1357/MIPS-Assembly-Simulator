# 簡單寄存器測試
addi $a0, $zero, 777    # 設定 $a0 = 777
addi $v0, $zero, 1      # print_int
syscall

addi $v0, $zero, 10     # exit
syscall
