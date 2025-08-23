# MIPS Assembly Simulator - 快速參考

## 🚀 快速開始

### 編譯與執行
```powershell
# Windows - 編譯
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release

# 執行GUI版本
.\build\src\mips-sim-gui.exe

# 執行測試 (預期：85/85 通過)
ctest --test-dir build
```

### Linux編譯
```bash
cmake -B build -G "Ninja"
cmake --build build
./build/src/mips-sim-gui
```

## 🔧 支援的MIPS指令

### R型指令
- `add $rd, $rs, $rt` - 暫存器加法
- `sub $rd, $rs, $rt` - 暫存器減法  
- `sll $rd, $rt, shamt` - 邏輯左移

### I型指令
- `addi $rt, $rs, imm` - 立即數加法
- `lw $rt, offset($rs)` - 載入字組
- `sw $rt, offset($rs)` - 儲存字組
- `beq $rs, $rt, label` - 相等分支

### J型指令
- `j label` - 無條件跳躍

### 系統調用
```mips
addi $v0, $zero, 1    # 印出整數 ($a0 = 數值)
addi $v0, $zero, 4    # 印出字串 ($a0 = 地址)
addi $v0, $zero, 5    # 讀取整數 (結果存於 $v0)
addi $v0, $zero, 10   # 程式結束
syscall
```

## 📝 範例程式模板

### 基本算術
```mips
addi $t0, $zero, 10    # $t0 = 10
addi $t1, $zero, 20    # $t1 = 20
add $t2, $t0, $t1      # $t2 = 30

# 印出結果
addi $v0, $zero, 1     # print_int
add $a0, $zero, $t2    # 印出 30
syscall

# 程式結束
addi $v0, $zero, 10
syscall
```

### 字串印出
```mips
# 將字串儲存到記憶體
addi $t0, $zero, 0x48  # 'H'
addi $t1, $zero, 0x69  # 'i'
sll $t1, $t1, 8        # 移到位元組位置1
add $t0, $t0, $t1      # 組合 'Hi'
sw $t0, 0x1000($zero)  # 儲存到記憶體

# 印出字串
addi $v0, $zero, 4     # print_string
addi $a0, $zero, 0x1000
syscall

addi $v0, $zero, 10    # 結束
syscall
```

## 🎮 GUI快速操作

### 基本操作
- **F5** / 點選"Execute" - 執行程式
- **F10** / 點選"Step" - 單步執行
- **Ctrl+R** / 點選"Reset" - 重設模擬器
- **Ctrl+L** - 載入示範程式

### 視窗切換
- **檢視選單** → 切換各種視窗顯示
- **暫存器檢視器** - 查看32個MIPS暫存器
- **記憶體檢視器** - 查看記憶體內容
- **管線檢視器** - 觀察5階段管線

## 🧪 測試命令

```powershell
# 執行所有測試
ctest --test-dir build

# 快速測試
.\build\tests\unit_tests.exe --gtest_brief=1

# 特定測試類別
ctest --test-dir build -R "CoreInstructions"
ctest --test-dir build -R "Pipeline"
ctest --test-dir build -R "Syscall"
```

## 📁 重要文件位置

- **完整指南**：`MIPS_SIMULATOR_COMPLETE_GUIDE.md`
- **GUI手冊**：`docs/user-guides/GUI_USER_MANUAL.md`
- **架構說明**：`docs/architecture/ARCHITECTURE_DECISIONS.md`
- **範例程式**：`asmtest/` 目錄

## ⚠️ 常見問題

### Q: 程式執行沒有輸出
A: 確保使用正確的系統調用：
```mips
addi $v0, $zero, 1     # 設定系統調用號碼
addi $a0, $zero, 42    # 設定參數
syscall                # 執行系統調用
```

### Q: 字串印不出來
A: 必須先將字串儲存到記憶體，$a0指向字串地址

### Q: 程式一直執行不停
A: 確保程式結尾有結束系統調用：
```mips
addi $v0, $zero, 10
syscall
```

---

**快速參考版本**：2025.08.15  
**適用版本**：MIPS Simulator Production Ready Release
