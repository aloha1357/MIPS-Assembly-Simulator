# TRAP 指令驗證報告

## 基本資訊
- **指令名稱**: TRAP (陷阱指令)
- **類別**: 系統指令
- **驗證狀態**: ✅ 已驗證通過
- **驗證日期**: 2025-09-10

## 指令格式
```
trap function_name
```

## 功能說明
執行系統級陷阱操作，提供輸入輸出和程式控制功能。

## 支援的陷阱功能

### trap print_int
**功能**: 列印整數值  
**參數**: $a0 = 要列印的整數  
**格式**: `trap print_int`

### trap exit  
**功能**: 結束程式執行  
**參數**: 無  
**格式**: `trap exit`

## 重要發現: 註解解析問題

### 問題描述
組譯器對 trap 指令後的註解格式敏感，複雜註解會導致解析失敗。

### 有問題的註解格式
```assembly
trap print_int # 應該輸出: 15    # ❌ 會導致解析失敗
trap print_int # 應該輸出：15    # ❌ 冒號會導致問題
```

### 安全的註解格式
```assembly
trap print_int       # 這應該要輸出 15    # ✅ 正常工作
trap print_int                             # ✅ 無註解最安全
```

## 測試案例

### 測試 1: 基本 print_int
```assembly
lhi $t0, 0
llo $t0, 5
lhi $t1, 0  
llo $t1, 10
add $t2, $t0, $t1
or $a0, $t2, $zero
trap print_int       # 輸出: 15
```
**預期結果**: 15  
**實際結果**: 15  
**狀態**: ✅ 通過

### 測試 2: 多次 print_int
```assembly
# 測試多個數值輸出
or $a0, $t2, $zero
trap print_int       # 輸出: 15

or $a0, $t3, $zero  
trap print_int       # 輸出: 300

or $a0, $t4, $zero
trap print_int       # 輸出: 0
```
**預期結果**: 15, 300, 0  
**實際結果**: 15, 300, 0  
**狀態**: ✅ 通過

### 測試 3: 程式結束
```assembly
trap exit            # 正常結束程式
```
**預期結果**: 程式正常結束  
**實際結果**: 程式正常結束  
**狀態**: ✅ 通過

## 使用注意事項
1. **註解格式**: 避免在 trap 指令後使用包含冒號(:)的註解
2. **參數準備**: print_int 需要在 $a0 中準備要列印的值
3. **程式結束**: 務必使用 trap exit 正常結束程式

## 錯誤排除
如果 trap print_int 沒有輸出：
1. 檢查註解格式是否包含特殊字符
2. 確認 $a0 暫存器是否正確設定
3. 使用 dump_labels.exe 檢查組譯器解析結果

## 相關指令
- SYSCALL: 標準 MIPS 系統呼叫
- OR: 常用於將值複製到 $a0

## 測試檔案位置
- `test_single_trap.asm`: 單一 trap 測試
- `instruction_verification/test_cases/test_no_comments.asm`: 無註解版本
