# 測試前瞻邏輯
# 這個標籤應該被識別為指令標籤
code_label:
llo $a0, 1
trap print_int

# 這個標籤應該被識別為資料標籤  
data_label:
.word 42

la $a0, code_label
trap print_int   # 應該印出指令位址（1？）

la $a0, data_label  
trap print_int   # 應該印出資料位址（0x10000000？）

trap exit
