# 將資料段放在前面
mydata:
.word 42

llo $a0, 99
trap print_int

la $a0, mydata
trap print_int

trap exit
