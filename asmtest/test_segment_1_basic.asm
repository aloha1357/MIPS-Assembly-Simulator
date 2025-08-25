# 測試段落1：基本LHI/LLO操作
lhi $a0, 1
llo $a0, 0
trap print_int
trap exit
