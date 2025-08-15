# 🚀 邏輯指令實作快速開始檢查清單

**目標**: 在接下來 2-3 天內實作 AND, OR, XOR, NOR 指令  
**當前狀態**: 9/47 指令完成 (19%)  
**目標狀態**: 13/47 指令完成 (27%)

## ✅ 立即行動項目 (今天就做)

### 🔥 最高優先級 - 馬上開始
- [ ] **發送 TA 確認郵件** - 確認可使用的函式庫
- [ ] **備份當前工作** - 建立 `feature/logical-instructions` 分支
- [ ] **設定測試環境** - 確認所有測試都通過

### 📋 第一天目標 - AND 指令 (2小時)
- [ ] **Step 1** (30分鐘): 在 `Instruction.h` 添加 `AndInstruction` 類別
- [ ] **Step 2** (45分鐘): 在 `Instruction.cpp` 實作 AND 邏輯
- [ ] **Step 3** (15分鐘): 添加到 `InstructionDecoder.cpp`
- [ ] **Step 4** (15分鐘): 添加到 `Assembler.cpp`
- [ ] **Step 5** (15分鐘): 編譯測試，修正錯誤

```powershell
# 快速測試命令
cmake --build build --config Debug
.\build\tests\unit_tests.exe --gtest_filter="*And*"
```

## 📅 每日計劃

### Day 1: AND 指令實作
**時間分配**: 2小時  
**成功標準**: AND 指令功能完整，所有相關測試通過

**檢查清單**:
- [ ] 類別宣告完成
- [ ] 執行邏輯實作
- [ ] 解碼器支援
- [ ] 彙編器支援
- [ ] 基本測試通過
- [ ] 進行代碼審查

### Day 2: OR 指令實作  
**時間分配**: 1.5小時  
**成功標準**: OR 指令功能完整

**檢查清單**:
- [ ] 複製 AND 指令模式
- [ ] 修改為 OR 邏輯 (`|` 運算子)
- [ ] 添加功能碼 0x25
- [ ] 彙編器關鍵字 "or"
- [ ] 測試通過

### Day 3: XOR 指令實作
**時間分配**: 1.5小時  
**成功標準**: XOR 指令功能完整，包括可逆性測試

**檢查清單**:
- [ ] XOR 邏輯實作 (`^` 運算子)
- [ ] 功能碼 0x26
- [ ] 自我 XOR 測試 (結果為 0)
- [ ] 可逆性測試 (加密/解密)
- [ ] 與零 XOR 測試 (恆等運算)

### Day 4: NOR 指令實作
**時間分配**: 1.5小時  
**成功標準**: NOR 指令功能完整，可作為 NOT 運算

**檢查清單**:
- [ ] NOR 邏輯實作 (`~(a | b)`)
- [ ] 功能碼 0x27
- [ ] NOT 運算測試 (NOR with zero)
- [ ] 德摩根定律驗證
- [ ] 邊界值測試

### Day 5: 整合與驗證
**時間分配**: 1小時  
**成功標準**: 所有邏輯指令整合完成，通過完整測試套件

**檢查清單**:
- [ ] 所有單元測試通過
- [ ] BDD 場景測試通過
- [ ] 效能基準測試
- [ ] 複合指令測試
- [ ] CI/CD 管線測試
- [ ] 文件更新

## 🧪 每日測試檢查點

### 基本功能測試 (每個指令)
```powershell
# 編譯
cmake --build build --config Debug

# 特定指令測試
.\build\tests\unit_tests.exe --gtest_filter="*And*"
.\build\tests\unit_tests.exe --gtest_filter="*Or*" 
.\build\tests\unit_tests.exe --gtest_filter="*Xor*"
.\build\tests\unit_tests.exe --gtest_filter="*Nor*"

# 所有邏輯指令測試
.\build\tests\unit_tests.exe --gtest_filter="*Logical*"
```

### 整合測試 (最終)
```powershell
# 完整測試套件
.\build\tests\unit_tests.exe

# BDD 測試
.\build\tests\bdd_tests.exe

# 效能測試
.\build\tests\unit_tests.exe --gtest_filter="*Performance*"
```

## 📊 進度追蹤表

| 日期 | 指令 | 狀態 | 測試通過 | 時間花費 | 備註 |
|------|------|------|----------|----------|------|
| Day 1 | AND | ⏳ | ❌ | 0/2h | 準備開始 |
| Day 2 | OR | ⏸️ | ❌ | 0/1.5h | 等待 AND 完成 |
| Day 3 | XOR | ⏸️ | ❌ | 0/1.5h | 等待 OR 完成 |
| Day 4 | NOR | ⏸️ | ❌ | 0/1.5h | 等待 XOR 完成 |
| Day 5 | 整合 | ⏸️ | ❌ | 0/1h | 等待 NOR 完成 |

**圖例**: ⏳ 進行中 | ✅ 完成 | ❌ 未開始 | ⏸️ 等待中 | ⚠️ 有問題

## 🔧 快速參考代碼模板

### 指令實作模板 (複製貼上使用)

```cpp
// ============ Instruction.h ============
class [INSTRUCTION]Instruction : public RTypeInstruction {
public:
    [INSTRUCTION]Instruction(int rd, int rs, int rt);
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

// ============ Instruction.cpp ============
[INSTRUCTION]Instruction::[INSTRUCTION]Instruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {}

void [INSTRUCTION]Instruction::execute(Cpu& cpu) {
    uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
    uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rs_val [OPERATOR] rt_val;  // 替換 [OPERATOR]
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string [INSTRUCTION]Instruction::getName() const {
    return "[instruction]";  // 小寫指令名稱
}

// ============ InstructionDecoder.cpp ============
case [FUNCTION_CODE]: 
    return std::make_unique<[INSTRUCTION]Instruction>(rd, rs, rt);

// ============ Assembler.cpp ============
else if (opcode == "[instruction]") {
    int rd = parseRegister(parts[1]);
    int rs = parseRegister(parts[2]); 
    int rt = parseRegister(parts[3]);
    return std::make_unique<[INSTRUCTION]Instruction>(rd, rs, rt);
}
```

### 運算子對照表
| 指令 | 運算子 | 功能碼 | 範例 |
|------|--------|--------|------|
| AND | `&` | 0x24 | `result = rs_val & rt_val;` |
| OR | `\|` | 0x25 | `result = rs_val \| rt_val;` |
| XOR | `^` | 0x26 | `result = rs_val ^ rt_val;` |
| NOR | `~(\|)` | 0x27 | `result = ~(rs_val \| rt_val);` |

## ⚠️ 常見問題快速解決

### 編譯錯誤
```powershell
# 清除並重新建置
rm -rf build
cmake -B build -G "Ninja"
cmake --build build --config Debug
```

### 測試失敗
```powershell
# 查看詳細測試輸出
.\build\tests\unit_tests.exe --gtest_verbose
```

### 找不到指令
- 檢查 `InstructionDecoder.cpp` 中的功能碼
- 檢查 `Assembler.cpp` 中的關鍵字拼寫

### 暫存器值錯誤
- 確認讀取順序：先讀取所有源暫存器，再寫入目標暫存器
- 檢查運算子是否正確

## 📞 需要幫助時

### 立即檢查項目
1. **編譯是否通過？** 先解決編譯錯誤
2. **現有測試是否還通過？** 確保沒有破壞現有功能
3. **新測試是否涵蓋所有情況？** 檢查測試覆蓋率
4. **效能是否符合要求？** 運行效能測試

### 尋求幫助的管道
- **GitHub Issues**: 記錄問題和解決過程
- **代碼審查**: 請同事審查實作
- **TA 諮詢**: 技術問題和需求確認
- **文件查閱**: MIPS 指令手冊和現有實作

## 🎯 本週目標

### 定量目標
- **指令數量**: 從 9 → 13 個指令 (+4)
- **覆蓋率**: 從 19% → 27% (+8%)
- **測試數量**: 從 85 → 110+ 個測試 (+25+)
- **代碼行數**: 預估增加 200-300 行

### 定性目標  
- **代碼品質**: 維持現有的高標準
- **測試覆蓋**: 新功能 100% 測試覆蓋
- **文件完整**: 每個新指令都有完整文件
- **效能標準**: 符合 < 10ns/指令的要求

---

**🚀 立即開始**: 複製這個檢查清單，開始第一個 AND 指令的實作！每完成一個項目就打勾，保持進度可見性。

**成功關鍵**: 
1. **小步快跑** - 每個指令獨立完成和測試
2. **測試先行** - 確保每次修改都有測試保護
3. **持續整合** - 經常編譯和測試，及早發現問題
4. **文件同步** - 實作和文件同時更新
