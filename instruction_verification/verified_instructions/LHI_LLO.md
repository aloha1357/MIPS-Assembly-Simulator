# LHI & LLO 指令驗證報告

## 基本資訊
- **指令名稱**: LHI (Load High Immediate) & LLO (Load Low Immediate)
- **類別**: 特殊載入指令
- **驗證狀態**: ✅ 已驗證通過
- **驗證日期**: 2025-09-10

## 指令格式
```
lhi $rt, immediate    # 載入高位立即數
llo $rt, immediate    # 載入低位立即數
```

## 功能說明
這兩個指令用於載入32位元數值到暫存器中，需要配合使用來載入完整的32位元值。

### LHI (Load High Immediate)
- 將16位元立即數載入到暫存器的高16位元 (位元31-16)
- 低16位元 (位元15-0) 設為 0

### LLO (Load Low Immediate)  
- 將16位元立即數載入到暫存器的低16位元 (位元15-0)
- 高16位元 (位元31-16) 保持不變

## 運算規則
```
LHI: rt[31:16] = immediate, rt[15:0] = 0
LLO: rt[15:0] = immediate, rt[31:16] 保持不變
```

## 典型使用模式

### 載入小數值 (0-65535)
```assembly
lhi $t0, 0           # $t0 = 0x00000000
llo $t0, 42          # $t0 = 0x0000002A (42)
```

### 載入大數值
```assembly
lhi $t0, 0x1234      # $t0 = 0x12340000
llo $t0, 0x5678      # $t0 = 0x12345678
```

### 載入最大值
```assembly
lhi $t0, 65535       # $t0 = 0xFFFF0000
llo $t0, 65535       # $t0 = 0xFFFFFFFF
```

## 測試案例

### 測試 1: 載入小數值
```assembly
lhi $t0, 0
llo $t0, 5           # 載入數值 5
lhi $t1, 0  
llo $t1, 10          # 載入數值 10
add $t2, $t0, $t1    # 5 + 10 = 15
```
**預期結果**: $t0=5, $t1=10, $t2=15  
**實際結果**: 正確載入並運算  
**狀態**: ✅ 通過

### 測試 2: 載入中等數值
```assembly
lhi $t0, 0
llo $t0, 100         # 載入數值 100
lhi $t1, 0
llo $t1, 200         # 載入數值 200
add $t3, $t0, $t1    # 100 + 200 = 300
```
**預期結果**: $t0=100, $t1=200, $t3=300  
**實際結果**: 正確載入並運算  
**狀態**: ✅ 通過

### 測試 3: 載入最大16位元值
```assembly
lhi $t0, 65535       # 高位 = 0xFFFF
llo $t0, 65535       # 低位 = 0xFFFF, 結果 = 0xFFFFFFFF
lhi $t1, 0
llo $t1, 1
add $t4, $t0, $t1    # 0xFFFFFFFF + 1 = 0 (溢位)
```
**預期結果**: $t0=0xFFFFFFFF, $t4=0 (溢位)  
**實際結果**: 正確處理溢位  
**狀態**: ✅ 通過

### 測試 4: 載入最大正數
```assembly
lhi $t0, 32767       # 高位 = 0x7FFF (最大正數高位)
llo $t0, 65535       # 低位 = 0xFFFF
lhi $t1, 0
llo $t1, 1
add $t5, $t0, $t1    # 0x7FFFFFFF + 1 = 0x80000000 (溢位到負數)
```
**預期結果**: $t0=0x7FFFFFFF, $t5=0x80000000  
**實際結果**: 正確處理正數到負數的溢位  
**狀態**: ✅ 通過

## 使用注意事項
1. **載入順序**: 通常先使用 LHI 再使用 LLO
2. **數值範圍**: 立即數範圍為 0-65535 (16位元無符號)
3. **配合使用**: 載入32位元值需要兩個指令配合
4. **溢位行為**: 與標準 MIPS 運算一致

## 常見用途
1. **常數載入**: 載入程式中的常數值
2. **位址計算**: 配合其他指令進行位址運算
3. **大數運算**: 準備大數值進行運算
4. **系統呼叫**: 設定系統呼叫號碼和參數

## 效能考量
- 載入大於65535的數值需要兩個指令週期
- 載入小數值時，LHI $t0, 0 可能可以最佳化

## 相關指令
- LUI: 標準 MIPS 的載入高位指令 (未驗證是否支援)
- ORI: 載入低位的替代方案 (未驗證是否支援)
- ADD/ADDI: 常與 LHI/LLO 配合使用

## 測試檔案位置
所有測試案例中都大量使用了 LHI/LLO 指令：
- `instruction_verification/test_cases/test_no_comments.asm`
- `instruction_verification/test_cases/test_basic_instructions.asm`
- `instruction_verification/test_cases/test_syscall_no_comments.asm`
