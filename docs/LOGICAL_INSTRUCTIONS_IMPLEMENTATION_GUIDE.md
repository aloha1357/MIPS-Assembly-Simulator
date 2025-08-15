# 邏輯指令原子化實作指南

**創建日期**: 2025年8月15日  
**目標**: 提供逐步實作邏輯指令的詳細指南  
**適用範圍**: AND, OR, XOR, NOR 四個邏輯指令

## 📋 實作檢查清單

### 準備階段 ✅ 
- [x] 分析現有指令架構
- [x] 創建測試計劃
- [x] 設定開發環境
- [ ] 確認TA批准使用的庫

### 實作階段
- [ ] **Phase 1**: AND 指令實作 (預估2小時)
- [ ] **Phase 2**: OR 指令實作 (預估1.5小時)
- [ ] **Phase 3**: XOR 指令實作 (預估1.5小時)
- [ ] **Phase 4**: NOR 指令實作 (預估1.5小時)
- [ ] **Phase 5**: 整合測試 (預估1小時)

## 🚀 Phase 1: AND 指令實作

### Step 1.1: 創建指令類別聲明

在 `src/Instruction.h` 中添加：

```cpp
/**
 * @brief R-type AND instruction (bitwise AND)
 */
class AndInstruction : public RTypeInstruction {
public:
    AndInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};
```

### Step 1.2: 實作指令邏輯

在 `src/Instruction.cpp` 中添加：

```cpp
// AND Instruction Implementation
AndInstruction::AndInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void AndInstruction::execute(Cpu& cpu) {
    // 讀取源暫存器值
    uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
    uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
    
    // 執行位元 AND 運算
    uint32_t result = rs_val & rt_val;
    
    // 寫入目標暫存器
    cpu.getRegisterFile().write(m_rd, result);
    
    // 更新程式計數器
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AndInstruction::getName() const {
    return "and";
}
```

### Step 1.3: 添加到指令解碼器

在 `src/InstructionDecoder.cpp` 的 `decodeRType()` 方法中添加：

```cpp
case 0x24: // AND function code
    return std::make_unique<AndInstruction>(rd, rs, rt);
```

### Step 1.4: 添加到彙編器

在 `src/Assembler.cpp` 的 `parseInstruction()` 方法中添加：

```cpp
else if (opcode == "and") {
    // 解析暫存器
    int rd = parseRegister(parts[1]);
    int rs = parseRegister(parts[2]);
    int rt = parseRegister(parts[3]);
    return std::make_unique<AndInstruction>(rd, rs, rt);
}
```

### Step 1.5: 編譯並測試

```powershell
# 編譯項目
cmake --build build --config Debug

# 執行基本測試
.\build\tests\unit_tests.exe --gtest_filter="*And*"
```

### Step 1.6: 驗證 Definition of Done

**功能驗證**:
- [ ] 基本 AND 運算正確
- [ ] 源暫存器保持不變
- [ ] PC 正確遞增
- [ ] 支援暫存器別名

**測試驗證**:
- [ ] 所有單元測試通過
- [ ] BDD 場景通過
- [ ] 邊界值測試通過
- [ ] 效能測試符合要求

**代碼質量**:
- [ ] 遵循現有代碼風格
- [ ] 適當的註釋和文件
- [ ] 無編譯警告
- [ ] 無記憶體洩漏

## 🚀 Phase 2: OR 指令實作

### Step 2.1: 基於 AND 指令模式實作

由於已建立了良好的模式，OR 指令實作應該更快：

```cpp
// 在 Instruction.h 中
class OrInstruction : public RTypeInstruction {
public:
    OrInstruction(int rd, int rs, int rt);
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

// 在 Instruction.cpp 中
OrInstruction::OrInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {}

void OrInstruction::execute(Cpu& cpu) {
    uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
    uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rs_val | rt_val;  // 唯一的差異：使用 | 而不是 &
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string OrInstruction::getName() const {
    return "or";
}
```

### Step 2.2: 添加解碼支援

```cpp
// InstructionDecoder.cpp
case 0x25: // OR function code
    return std::make_unique<OrInstruction>(rd, rs, rt);

// Assembler.cpp
else if (opcode == "or") {
    int rd = parseRegister(parts[1]);
    int rs = parseRegister(parts[2]);
    int rt = parseRegister(parts[3]);
    return std::make_unique<OrInstruction>(rd, rs, rt);
}
```

## 🚀 Phase 3: XOR 指令實作

### 關鍵實作要點

```cpp
void XorInstruction::execute(Cpu& cpu) {
    uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
    uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rs_val ^ rt_val;  // XOR 運算
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

**特別測試要求**:
- 自我 XOR（A ⊕ A = 0）
- 與零 XOR（A ⊕ 0 = A）
- 可逆性測試（加密/解密）

## 🚀 Phase 4: NOR 指令實作

### 關鍵實作要點

```cpp
void NorInstruction::execute(Cpu& cpu) {
    uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
    uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
    uint32_t result = ~(rs_val | rt_val);  // NOR 運算：NOT(OR)
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

**特別測試要求**:
- 作為 NOT 運算（A NOR 0 = ~A）
- 與零 NOR 的特殊性質
- 德摩根定律驗證

## 🔧 常見實作陷阱與解決方案

### 陷阱 1: 忘記更新程式計數器
**問題**: 指令執行後程式卡在原地
**解決**: 確保每個 `execute()` 方法都有 `cpu.setProgramCounter(cpu.getProgramCounter() + 1);`

### 陷阱 2: 暫存器別名處理不當
**問題**: `and $t0, $t0, $t0` 產生錯誤結果
**解決**: 先讀取所有源暫存器，最後才寫入目標暫存器

### 陷阱 3: 功能碼錯誤
**問題**: 指令無法正確解碼
**解決**: 確認 MIPS 指令手冊中的正確功能碼
- AND: 0x24
- OR: 0x25  
- XOR: 0x26
- NOR: 0x27

### 陷阱 4: 測試資料錯誤
**問題**: 測試通過但實際運算錯誤
**解決**: 使用已驗證的測試向量，手工計算預期結果

## 📊 效能最佳化建議

### 1. 內聯小函數
對於簡單的位元運算，確保編譯器能夠內聯優化。

### 2. 避免不必要的記憶體訪問
```cpp
// 好的實作
uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
uint32_t result = rs_val & rt_val;
cpu.getRegisterFile().write(m_rd, result);

// 避免的實作
cpu.getRegisterFile().write(m_rd, 
    cpu.getRegisterFile().read(m_rs) & cpu.getRegisterFile().read(m_rt));
```

### 3. 使用適當的資料類型
確保使用 `uint32_t` 而不是 `int` 來避免符號擴展問題。

## 🧪 測試驗證策略

### 單元測試層級
- **正面測試**: 基本功能正確性
- **邊界測試**: 0x00000000, 0xFFFFFFFF
- **別名測試**: 源與目標暫存器相同
- **隨機測試**: 大量隨機輸入驗證

### 整合測試層級
- **指令序列**: 多個邏輯指令組合
- **與現有指令交互**: 與 ADD, ADDI, LW, SW 等組合
- **實際程式**: 完整的 MIPS 程式執行

### BDD 測試層級
- **場景覆蓋**: 所有定義的 BDD 場景
- **使用者故事**: 實際使用情境驗證
- **錯誤情境**: 異常輸入處理

## 📈 進度追蹤

### 每日檢查點
- **Day 1 結束**: AND 指令完成，所有測試通過
- **Day 2 結束**: OR 指令完成，累計測試通過
- **Day 3 結束**: XOR 指令完成，複合測試通過
- **Day 4 結束**: NOR 指令完成，所有邏輯指令實作完成
- **Day 5 結束**: 整合測試、效能優化、文件完成

### 里程碑指標
- **指令覆蓋率**: 從 19% → 27% (13/47 指令)
- **測試數量**: 從 85 → 110+ 個測試
- **BDD 場景**: 新增 20+ 個邏輯指令場景
- **效能基準**: 每條指令 < 10ns

## 🎯 成功標準

### 必須達成 (Must Have)
- [ ] 所有 4 個邏輯指令正確實作
- [ ] 100% 測試通過率
- [ ] 符合現有代碼架構
- [ ] 通過 CI/CD 管線

### 應該達成 (Should Have)
- [ ] BDD 場景 100% 覆蓋
- [ ] 效能基準達標
- [ ] 代碼覆蓋率 ≥ 95%
- [ ] 無記憶體洩漏

### 可以達成 (Could Have)
- [ ] 額外的邊界測試
- [ ] 效能優化
- [ ] 更詳細的文件
- [ ] 示例程式

## 📞 支援資源

### 技術參考
- [MIPS 指令集手冊](../requirement/mips.pdf)
- [現有指令實作](../src/Instruction.cpp)
- [測試框架範例](../tests/test_cpu.cpp)

### 開發工具
- **IDE**: Visual Studio Code
- **編譯器**: GCC 13.2.0
- **建置系統**: CMake + Ninja
- **測試框架**: GoogleTest
- **版本控制**: Git

### 故障排除
1. **編譯錯誤**: 檢查標頭檔包含和名稱空間
2. **鏈接錯誤**: 確保新類別已添加到 CMakeLists.txt
3. **測試失敗**: 使用調試器逐步執行
4. **效能問題**: 使用 profiler 分析瓶頸

---

**總結**: 這份實作指南提供了完整的邏輯指令實作路徑，從代碼結構到測試驗證，確保每個步驟都有明確的標準和檢查點。按照這個指南執行，應該能夠在預計時間內完成高質量的實作。
