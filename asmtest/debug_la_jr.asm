# 測試 la 和 jr 的組合
llo $a0, 99
trap print_int  # 印出 99，確認程式運行

la $a0, target
trap print_int  # 印出標籤地址

jr $a0          # 跳轉到標籤
trap exit       # 不應該執行

target:
llo $a0, 88
trap print_int  # 應該印出 88
trap exit
