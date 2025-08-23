# MIPS Assembly Simulator - GUI 完整使用手冊

**更新日期：2025年7月31日**  
**版本：1.0 - 完整功能版本**

## 🚀 快速開始

### 啟動應用程式
```powershell
# 1. 編譯專案（如果尚未編譯）
cmake -B build -G "Ninja"
cmake --build build

# 2. 啟動GUI版本
.\build\src\mips-sim-gui.exe
```

### 第一次使用
1. 啟動後會看到完整的MIPS模擬器界面
2. 左側是代碼編輯器，可以輸入MIPS彙編程式碼
3. 右側是各種檢視窗口（寄存器、記憶體、管線、輸出）
4. 頂部有工具列按鈕可執行程式碼

## 📱 界面佈局說明

### 主選單列
- **檔案（File）**：新建、開啟、儲存程式檔案
- **檢視（View）**：控制各個窗口的顯示/隱藏
- **執行（Run）**：程式執行相關功能
- **說明（Help）**：關於資訊和使用說明

### 工具列按鈕
```
[▶️ 執行] [⏭️ 單步] [🔄 重設] [📊 管線模式]
```
- **▶️ 執行（Execute）**：完整執行當前程式碼
- **⏭️ 單步（Step）**：逐條指令執行
- **🔄 重設（Reset）**：重設CPU和記憶體狀態
- **📊 管線模式（Pipeline）**：切換單週期/管線執行模式

### 主要窗口區域

#### 1. 代碼編輯器（左側）
- **功能**：輸入和編輯MIPS彙編程式碼
- **容量**：8192字元緩衝區
- **支援**：基本文字編輯功能（複製、貼上、剪下）
- **語法**：MIPS彙編語言

#### 2. 寄存器檢視器（右上）
- **顯示**：所有32個MIPS寄存器的當前值
- **格式**：十進位 / 十六進位
- **寄存器名稱**：
  - `$0 ($zero)`：永久為0
  - `$1 ($at)`：彙編器暫存器
  - `$2-$3 ($v0-$v1)`：返回值
  - `$4-$7 ($a0-$a3)`：參數
  - `$8-$15 ($t0-$t7)`：暫存器
  - `$16-$23 ($s0-$s7)`：保存寄存器
  - `$24-$25 ($t8-$t9)`：暫存器
  - `$26-$27 ($k0-$k1)`：核心寄存器
  - `$28 ($gp)`：全域指標
  - `$29 ($sp)`：堆疊指標
  - `$30 ($fp)`：框架指標
  - `$31 ($ra)`：返回地址

#### 3. 記憶體檢視器（右中）
- **顯示**：記憶體內容的十六進位檢視
- **範圍**：4KB記憶體空間（0x00000000 - 0x00000FFF）
- **格式**：每行顯示16個位元組
- **更新**：程式執行時即時更新

#### 4. 管線檢視器（右下）
- **階段顯示**：
  - **IF（Instruction Fetch）**：指令擷取
  - **ID（Instruction Decode）**：指令解碼
  - **EX（Execute）**：執行
  - **MEM（Memory）**：記憶體存取
  - **WB（Write Back）**：寫回
- **狀態**：顯示每個階段當前處理的指令

#### 5. 控制台輸出（底部）
- **功能**：顯示程式執行結果和系統訊息
- **支援輸出**：
  - `syscall 1`：print_int - 輸出整數
  - `syscall 4`：print_string - 輸出字串
  - 錯誤訊息和警告
  - 程式執行狀態

## 💻 支援的MIPS指令

### R-Type指令
```assembly
add $rd, $rs, $rt    # $rd = $rs + $rt
sub $rd, $rs, $rt    # $rd = $rs - $rt
```

### I-Type指令
```assembly
addi $rt, $rs, imm   # $rt = $rs + immediate
lw $rt, offset($rs)  # $rt = Memory[$rs + offset]
sw $rt, offset($rs)  # Memory[$rs + offset] = $rt
beq $rs, $rt, label  # if ($rs == $rt) jump to label
```

### J-Type指令
```assembly
j label              # jump to label
```

### 系統調用
```assembly
syscall              # 執行系統調用
```

## 🔧 系統調用詳細說明

### syscall 1 - print_int
```assembly
addi $v0, $zero, 1    # 設定系統調用號碼為1
addi $a0, $zero, 42   # 設定要印出的整數
syscall               # 執行，會在控制台顯示 "42"
```

### syscall 4 - print_string
```assembly
# 首先需要在記憶體中準備字串
addi $v0, $zero, 4    # 設定系統調用號碼為4
addi $a0, $zero, 100  # 字串的記憶體地址
syscall               # 執行，會印出該地址的字串
```

### syscall 5 - read_int
```assembly
addi $v0, $zero, 5    # 設定系統調用號碼為5
syscall               # 執行，從使用者讀取整數
# 結果會儲存在 $v0 中
```

### syscall 10 - exit
```assembly
addi $v0, $zero, 10   # 設定系統調用號碼為10
syscall               # 結束程式執行
```

## 📝 程式撰寫指南

### 基本程式結構
```assembly
# 程式開始
main:
    # 你的程式碼在這裡
    addi $t0, $zero, 5
    addi $t1, $zero, 3
    add $t2, $t0, $t1
    
    # 印出結果
    addi $v0, $zero, 1     # print_int
    add $a0, $zero, $t2    # 要印的值
    syscall
    
    # 結束程式
    addi $v0, $zero, 10    # exit
    syscall
```

### 記憶體操作範例
```assembly
# 儲存和載入資料
main:
    addi $t0, $zero, 100   # 值 100
    addi $t1, $zero, 200   # 記憶體地址 200
    
    sw $t0, 0($t1)         # 將 100 儲存到地址 200
    lw $t2, 0($t1)         # 從地址 200 載入到 $t2
    
    # 印出載入的值
    addi $v0, $zero, 1
    add $a0, $zero, $t2
    syscall
    
    # 結束
    addi $v0, $zero, 10
    syscall
```

### 分支跳轉範例
```assembly
# 條件分支範例
main:
    addi $t0, $zero, 5
    addi $t1, $zero, 5
    
    beq $t0, $t1, equal    # 如果相等就跳到 equal
    
    # 不相等的情況
    addi $v0, $zero, 1
    addi $a0, $zero, 0     # 印出 0
    syscall
    j end
    
equal:
    # 相等的情況
    addi $v0, $zero, 1
    addi $a0, $zero, 1     # 印出 1
    syscall
    
end:
    addi $v0, $zero, 10    # 結束程式
    syscall
```

## 🎮 使用流程

### 標準使用流程
1. **啟動程式**：執行 `mips-sim-gui.exe`
2. **輸入程式碼**：在左側編輯器輸入MIPS彙編程式碼
3. **選擇執行模式**：單週期或管線模式
4. **執行程式**：
   - 點擊「▶️ 執行」按鈕完整執行
   - 或點擊「⏭️ 單步」按鈕逐步執行
5. **檢視結果**：
   - 在控制台查看輸出結果
   - 在寄存器檢視器查看寄存器變化
   - 在記憶體檢視器查看記憶體變化
6. **重設或修改**：點擊「🔄 重設」清除狀態，修改程式碼重新執行

### 調試流程
1. **單步執行**：使用「⏭️ 單步」按鈕逐條執行指令
2. **觀察狀態**：每執行一步後檢查：
   - 寄存器值的變化
   - 記憶體內容的變化
   - 管線階段的狀態
3. **發現問題**：如果程式行為不如預期，檢查：
   - 指令語法是否正確
   - 寄存器使用是否恰當
   - 記憶體地址是否對齊
   - 系統調用參數是否正確

## 🐛 常見問題與解決

### Q1: 程式執行後沒有輸出
**可能原因**：
- 沒有使用 `syscall 1` 或 `syscall 4` 輸出
- 忘記設定 `$v0` 系統調用號碼
- 忘記設定 `$a0` 參數值

**解決方法**：
```assembly
# 確保這三行都有
addi $v0, $zero, 1    # 系統調用號碼
addi $a0, $zero, 42   # 要輸出的值
syscall               # 執行系統調用
```

### Q2: 程式執行一直卡住
**可能原因**：
- 沒有使用 `syscall 10` 結束程式
- 無窮迴圈

**解決方法**：
```assembly
# 程式結尾一定要有
addi $v0, $zero, 10
syscall
```

### Q3: 記憶體存取錯誤
**可能原因**：
- 記憶體地址沒有4位元組對齊
- 存取超出4KB記憶體範圍

**解決方法**：
- 使用4的倍數地址（0, 4, 8, 12...）
- 確保地址在0-4095範圍內

### Q4: 寄存器值不如預期
**可能原因**：
- `$zero` 寄存器永遠是0，不能修改
- 寄存器編號錯誤

**解決方法**：
- 檢查寄存器名稱是否正確
- 不要嘗試寫入 `$zero` 寄存器

## ⚠️ 已知限制與測試問題

### GUI Console測試限制
**目前狀況**：
- GUI console的單元測試雖然通過，但與實際GUI操作存在差異
- 測試使用基礎GUI類別，但實際使用ImGui界面
- console輸出的驗證機制需要改進

**建議測試方式**：
1. **手動測試**：直接在GUI中執行程式，檢查console輸出
2. **整合測試**：使用實際GUI環境進行測試
3. **視覺驗證**：觀察GUI界面的實際輸出結果

**待改進項目**：
- GUI console測試方法需重新設計
- 需要更接近實際使用場景的測試策略
- 考慮實施截圖比對或UI自動化測試

## 📚 技術參考

### 編譯環境
- **編譯器**：GCC 13.2.0 (C++17)
- **建置系統**：CMake 3.29.2 + Ninja
- **GUI框架**：SDL2 + OpenGL + Dear ImGui 1.90.4

### 相關文件
- [架構決策文件](../architecture/ARCHITECTURE_DECISIONS.md)
- [核心指令說明](../architecture/01_core_instructions.md)
- [管線架構說明](../architecture/02_pipeline.md)
- [專案狀態報告](../CURRENT_PROJECT_STATUS_2025.md)

---

**注意**：本手冊基於MIPS Assembly Simulator v1.0版本撰寫，如有功能更新，請參考最新的專案狀態報告。
