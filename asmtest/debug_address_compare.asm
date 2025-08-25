# 測試標籤位址是否正確 - 更詳細
llo $a0, 8       # 設定位址為 8  
trap print_int   # 應該印出 8

la $a0, test_data # 載入真實資料段位址
trap print_int    # 印出實際資料段位址

trap exit

test_data:
.word 42
