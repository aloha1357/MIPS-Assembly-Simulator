# 🚀 開發者快速參考

## 常用指令
```powershell
# 🔨 建置 & 測試
cmake --build build --config Debug
ctest --test-dir build --output-on-failure

# 🧪 單一測試
ctest --test-dir build -R "CoreInstructionsBDD" -V

# 🏃 執行模擬器
.\build\src\Debug\mips-sim.exe
```

## 🔄 BDD 開發流程

### 1. RED - 撰寫失敗測試
```cpp
TEST_F(CoreInstructionsBDD, NewInstruction_Test) {
    given_register_contains("$t0", 5);
    when_program_executed_for_cycles("newop $t1, $t0, 10", 1);
    then_register_should_equal("$t1", 15);  // 會失敗
}
```

### 2. GREEN - 最小實現
- 添加指令類別到 `Instruction.h`
- 實現 `execute()` 方法在 `Instruction.cpp`
- 更新 `Assembler.cpp` 解析邏輯

### 3. REFACTOR - 重構清理
- 消除重複程式碼
- 改善命名和結構
- 確保所有測試仍通過

## 📁 檔案速查

| 檔案 | 用途 |
|------|------|
| `src/Cpu.h` | 主要 CPU 介面 |
| `src/Instruction.h` | 指令類別定義 |
| `src/Assembler.cpp` | 組譯器解析邏輯 |
| `tests/test_bdd_core_instructions.cpp` | BDD 測試 |
| `features/*.feature` | Gherkin 規格檔案 |

## 🎯 新指令實現檢查清單

- [ ] 定義指令類別 (繼承適當基礎類別)
- [ ] 實現 `execute()` 方法
- [ ] 更新組譯器解析
- [ ] 撰寫 BDD 測試案例
- [ ] 確認所有測試通過
- [ ] 更新文件和註釋

## ⚠️ 常見陷阱

- **忘記包含標頭檔**: 編譯錯誤時檢查 #include
- **暫存器編號錯誤**: `$t0` = 8, `$s0` = 16, `$v0` = 2  
- **類型轉換警告**: 使用 `static_cast<uint32_t>()` 轉換
- **記憶體對齊**: 字組存取必須 4 位元組對齊

## 🏷️ 暫存器對照表
```
$zero/$0 = 0    $v0-$v1 = 2-3     $a0-$a3 = 4-7
$t0-$t7 = 8-15  $s0-$s7 = 16-23   $t8-$t9 = 24-25
$k0-$k1 = 26-27 $gp = 28          $sp = 29 
$fp = 30        $ra = 31
```
