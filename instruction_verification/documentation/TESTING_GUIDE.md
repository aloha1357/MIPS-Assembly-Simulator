# 指令驗證測試運行指南

## 測試環境
- **作業系統**: Windows
- **Shell**: PowerShell
- **編譯工具**: clean_runner.exe, dump_labels.exe
- **工作目錄**: C:\Users\aloha\Documents\GitHub\itcpp2526finalproject-go46lif-1

## 快速測試所有已驗證指令

### 1. 基本算術運算測試
```powershell
.\build\src\clean_runner.exe instruction_verification\test_cases\test_no_comments.asm
```
**預期輸出**: 15, 300, 0, 2147483648

### 2. 多種基礎指令測試
```powershell  
.\build\src\clean_runner.exe instruction_verification\test_cases\test_basic_instructions.asm
```
**預期輸出**: 15, 5, 13, 0, 15, 15, 20, 5

### 3. 分支跳躍測試
```powershell
.\build\src\clean_runner.exe instruction_verification\test_cases\test_branch_jump.asm
```
**預期輸出**: 99, 88

### 4. 記憶體操作測試
```powershell
.\build\src\clean_runner.exe instruction_verification\test_cases\test_memory_ops.asm
```
**預期輸出**: 0, 77, 200

### 5. 系統呼叫測試
```powershell
.\build\src\clean_runner.exe instruction_verification\test_cases\test_syscall_no_comments.asm
```
**預期輸出**: 25, 35

## 全自動測試腳本

### 建立測試腳本
建立檔案 `run_verification_tests.ps1`：

```powershell
# 指令驗證自動測試腳本
Write-Host "開始 MIPS 指令驗證測試..." -ForegroundColor Green

$testCases = @(
    @{Name="基本算術運算"; File="test_no_comments.asm"; Expected="15,300,0,2147483648"},
    @{Name="多種基礎指令"; File="test_basic_instructions.asm"; Expected="15,5,13,0,15,15,20,5"},
    @{Name="分支跳躍"; File="test_branch_jump.asm"; Expected="99,88"},
    @{Name="記憶體操作"; File="test_memory_ops.asm"; Expected="0,77,200"},
    @{Name="系統呼叫"; File="test_syscall_no_comments.asm"; Expected="25,35"}
)

$passCount = 0
$totalCount = $testCases.Count

foreach ($test in $testCases) {
    Write-Host "`n測試: $($test.Name)" -ForegroundColor Yellow
    Write-Host "檔案: $($test.File)"
    Write-Host "預期輸出: $($test.Expected)"
    
    $result = .\build\src\clean_runner.exe "instruction_verification\test_cases\$($test.File)" 2>&1
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ 測試通過" -ForegroundColor Green
        $passCount++
    } else {
        Write-Host "❌ 測試失敗" -ForegroundColor Red
        Write-Host "錯誤輸出: $result"
    }
}

Write-Host "`n測試總結:" -ForegroundColor Cyan
Write-Host "通過: $passCount / $totalCount"
Write-Host "成功率: $(($passCount / $totalCount * 100).ToString('F1'))%"

if ($passCount -eq $totalCount) {
    Write-Host "🎉 所有測試通過！" -ForegroundColor Green
} else {
    Write-Host "⚠️  有測試失敗，請檢查相關指令" -ForegroundColor Red
}
```

### 執行自動測試
```powershell
.\run_verification_tests.ps1
```

## 除錯工具使用

### 檢查組譯器解析
```powershell
.\build\src\dump_labels.exe instruction_verification\test_cases\test_no_comments.asm
```

### 檢查問題檔案的解析
```powershell
# 對比有問題的檔案
.\build\src\dump_labels.exe test_single_trap.asm
.\build\src\dump_labels.exe instruction_verification\test_cases\test_no_comments.asm
```

## 新指令驗證流程

### 1. 建立測試檔案
在 `instruction_verification/test_cases/` 目錄下建立新的測試檔案：
```assembly
.text
main:
    # 測試新指令
    # 注意：避免複雜註解
    
    trap exit
```

### 2. 執行測試
```powershell
.\build\src\clean_runner.exe instruction_verification\test_cases\新測試檔案.asm
```

### 3. 驗證解析
```powershell
.\build\src\dump_labels.exe instruction_verification\test_cases\新測試檔案.asm
```

### 4. 記錄結果
在 `verified_instructions/` 目錄下建立 `指令名稱.md` 文檔

## 問題排除指南

### 問題 1: trap print_int 沒有輸出
**可能原因**:
- 註解格式問題
- $a0 暫存器未正確設定
- 組譯器解析錯誤

**檢查步驟**:
1. 使用 dump_labels.exe 檢查解析結果
2. 確認 trap 指令是否被正確識別
3. 簡化或移除註解

### 問題 2: 程式執行錯誤
**可能原因**:
- 語法錯誤
- 暫存器使用錯誤
- 記憶體位址問題

**檢查步驟**:
1. 檢查組譯器輸出
2. 驗證指令格式
3. 確認暫存器名稱正確

### 問題 3: 意外的輸出結果
**可能原因**:
- 溢位行為
- 暫存器值未正確設定
- 邏輯錯誤

**檢查步驟**:
1. 手動計算預期結果
2. 檢查中間步驟的暫存器值
3. 驗證指令執行順序

## 測試檔案命名規範
- `test_指令類別_描述.asm` - 一般測試
- `test_single_指令名稱.asm` - 單一指令測試
- `test_no_comments.asm` - 無註解版本
- `debug_特定問題.asm` - 問題除錯檔案

## 文檔更新流程
1. 在 `verified_instructions/` 建立指令文檔
2. 更新 `verified_instructions/README.md` 索引
3. 更新主要的 `VERIFICATION_REPORT.md`
4. 記錄任何發現的問題或限制
