# 純標籤位址測試 - 不做記憶體讀取
la $a0, test_label
trap print_int
trap exit

test_label:
.word 999
