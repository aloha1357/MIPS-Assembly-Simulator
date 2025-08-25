# 檢查標籤映射是否正確
llo $a0, 99   # 應該印出 99，確認程式能運行
trap print_int

la $a0, mydata
trap print_int  # 如果正確應該印出 8，錯誤會印出 2

trap exit

mydata:
.word 42
