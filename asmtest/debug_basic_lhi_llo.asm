# 調試分段：基本LHI/LLO測試
lhi $a0, 1
llo $a0, 0
trap print_int
trap exit
