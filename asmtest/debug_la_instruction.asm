# 檢查 la 指令是否正常工作
# 測試不使用資料段的 la 指令

la $a0, label1    # 載入標籤位址
trap print_int    # 印出位址

j label1

label1:
trap print_int    # 印出位址（再次確認）
trap exit
