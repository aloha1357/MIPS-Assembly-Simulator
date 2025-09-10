# 18個已驗證指令測試指南

## 快速測試指令

### 算術運算 (3個)
```powershell
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_add.asm    # 輸出: 15
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_sub.asm    # 輸出: 5
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_addi.asm   # 輸出: 13
```

### 邏輯運算 (3個)
```powershell
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_and.asm    # 輸出: 8
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_or.asm     # 輸出: 12
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_xor.asm    # 輸出: 8
```

### 移位指令 (2個)
```powershell
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_sll.asm    # 輸出: 20
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_srl.asm    # 輸出: 5
```

### 記憶體操作 (3個)
```powershell
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_la.asm     # 輸出: 42
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_lw.asm     # 輸出: 100
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_sw.asm     # 輸出: 200
```

### 分支跳躍 (3個)
```powershell
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_beq.asm    # 輸出: 99
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_bne.asm    # 輸出: 88
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_j.asm      # 輸出: 77
```

### 特殊載入 (2個)
```powershell
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_lhi.asm    # 輸出: 65536
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_llo.asm    # 輸出: 255
```

### 系統指令 (2個)
```powershell
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_trap.asm   # 輸出: 42
.\build\src\clean_runner.exe instruction_verification\individual_tests\test_syscall.asm # 輸出: 123
```

## 全部測試腳本
```powershell
# 批次測試所有指令
$tests = @(
    "test_add.asm", "test_sub.asm", "test_addi.asm",
    "test_and.asm", "test_or.asm", "test_xor.asm",
    "test_sll.asm", "test_srl.asm",
    "test_la.asm", "test_lw.asm", "test_sw.asm",
    "test_beq.asm", "test_bne.asm", "test_j.asm",
    "test_lhi.asm", "test_llo.asm",
    "test_trap.asm", "test_syscall.asm"
)

foreach ($test in $tests) {
    Write-Host "測試: $test"
    .\build\src\clean_runner.exe "instruction_verification\individual_tests\$test"
    Write-Host "---"
}
```

## 總計
- **18個已驗證指令**
- **18個無註解測試檔案**
- **每個指令都有獨立測試**
- **所有測試都能正常輸出**
