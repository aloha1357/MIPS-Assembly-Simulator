# 測試 instructions.asm 的開頭部分
lhi $a0, 1
llo $a0, 0
trap print_int     # 應該印出 65536
sll $a0, $a0, 1
trap print_int     # 應該印出 131072
trap exit
