# 測試資料段位址
la $a0, test_data
trap print_int
trap exit

test_data:
.word 999
