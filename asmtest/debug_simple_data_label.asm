# 最簡單的資料段測試
la $a0, mydata
trap print_int
trap exit

mydata:
.word 123
