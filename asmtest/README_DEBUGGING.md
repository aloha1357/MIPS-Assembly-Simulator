# 指令測試檔案 - 使用方法
# 這些檔案用於除錯 assignment/test/instructions.asm 中的無限迴圈問題

# 測試順序：
# 1. test_instructions_part1.asm  - 基本設定和移位
# 2. test_instructions_part2.asm  - 跳躍和標籤  
# 3. test_instructions_part3.asm  - 乘法除法
# 4. test_instructions_part4.asm  - ALU運算
# 5. test_instructions_part5.asm  - 分支指令
# 6. test_instructions_part6.asm  - 記憶體操作 (可能的問題區域)
# 7. test_instructions_part7.asm  - JAL和字串 (可能的問題區域)

# 已知問題：
# - part6 記憶體操作沒有正確載入資料
# - part7 JAL指令和字串操作有問題

# 建議除錯步驟：
# 1. 逐一測試每個部分
# 2. 找出哪個部分導致問題
# 3. 在該部分內部使用二分法
# 4. 建立最小重現案例

# 執行指令：
# build\cli\mipsim.exe run asmtest\test_instructions_partX.asm --limit 200
