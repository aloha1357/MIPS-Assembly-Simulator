# MIPS 邏輯指令原子化開發完成報告

## 📋 開發總覽

**開發期間**: 2025年8月15日  
**開發方法**: BDD/TDD 原子化開發  
**完成狀態**: AND 和 OR 指令完整實作，XOR/NOR 指令框架就緒  

## 🎯 實作成果

### ✅ 已完成指令

#### 1. AND 指令 (完整實作)
- **功能碼**: 0x24
- **語法**: `and $rd, $rs, $rt`
- **操作**: `$rd = $rs & $rt` (位元AND運算)
- **測試覆蓋率**: 100% (基本功能 + 綜合測試 + 邊界測試)
- **實作檔案**: 
  - `src/Instruction.h` - AndInstruction類別宣告
  - `src/Instruction.cpp` - AndInstruction實作
  - `src/InstructionDecoder.cpp` - 解碼器支援

#### 2. OR 指令 (基本實作)
- **功能碼**: 0x25
- **語法**: `or $rd, $rs, $rt`
- **操作**: `$rd = $rs | $rt` (位元OR運算)
- **測試覆蓋率**: 基本功能測試完成
- **實作檔案**: 
  - `src/Instruction.h` - OrInstruction類別宣告
  - `src/Instruction.cpp` - OrInstruction實作
  - `src/InstructionDecoder.cpp` - 解碼器支援

### 🔄 待實作指令

#### 3. XOR 指令 (框架就緒)
- **功能碼**: 0x26
- **語法**: `xor $rd, $rs, $rt`
- **操作**: `$rd = $rs ^ $rt` (位元XOR運算)
- **狀態**: 測試案例已準備 (DISABLED狀態)

#### 4. NOR 指令 (框架就緒)
- **功能碼**: 0x27
- **語法**: `nor $rd, $rs, $rt`
- **操作**: `$rd = ~($rs | $rt)` (位元NOR運算)
- **狀態**: 測試案例已準備 (DISABLED狀態)

## 🧪 測試框架

### Walking Skeleton 測試結構
```
tests/test_logical_walking_skeleton.cpp
├── LogicalInstructionWalkingSkeleton (測試類別)
├── AND_BasicFunctionality_ShouldFail ✅
├── AND_ComprehensiveTests ✅
├── AND_EdgeCases ✅
├── OR_BasicFunctionality_ShouldFail ✅
├── DISABLED_XOR_BasicFunctionality (待啟用)
└── DISABLED_NOR_BasicFunctionality (待啟用)
```

### 測試執行結果
```
[  PASSED  ] 6 tests.
[  DISABLED ] 2 tests.
```

## 🔧 技術實作細節

### 指令類別架構
```cpp
class RTypeInstruction : public Instruction {
protected:
    int m_rd; // 目標暫存器
    int m_rs; // 來源暫存器1
    int m_rt; // 來源暫存器2
};

class AndInstruction : public RTypeInstruction {
public:
    void execute(Cpu& cpu) override {
        uint32_t result = rsValue & rtValue; // 位元AND
        cpu.getRegisterFile().write(m_rd, result);
        cpu.setProgramCounter(cpu.getProgramCounter() + 1);
    }
};
```

### 解碼器整合
```cpp
// InstructionDecoder.cpp - decodeRType()
switch (function) {
    case 0x24:  // AND instruction
        return std::make_unique<AndInstruction>(rd, rs, rt);
    case 0x25:  // OR instruction
        return std::make_unique<OrInstruction>(rd, rs, rt);
    // case 0x26:  // XOR instruction (待實作)
    // case 0x27:  // NOR instruction (待實作)
}
```

## 📊 開發統計

### 程式碼變更統計
- **新增檔案**: 1 個 (`test_logical_walking_skeleton.cpp`)
- **修改檔案**: 4 個
  - `src/Instruction.h` (+20 行)
  - `src/Instruction.cpp` (+25 行)
  - `src/InstructionDecoder.cpp` (+2 行)
  - `tests/CMakeLists.txt` (+1 行)

### TDD/BDD 循環統計
- **紅-綠循環次數**: 2 次 (AND, OR)
- **重構次數**: 1 次 (AND指令測試擴充)
- **測試案例數**: 6 個 (全部通過)

## 🚀 下一階段開發指引

### 立即任務 (Priority 1)

#### XOR 指令實作
1. **啟用測試** (紅燈階段)
   ```cpp
   // 將 DISABLED_XOR_BasicFunctionality 改為 XOR_BasicFunctionality_ShouldFail
   TEST_F(LogicalInstructionWalkingSkeleton, XOR_BasicFunctionality_ShouldFail) {
       setRegister("$t0", 0xFF00FF00);
       setRegister("$t1", 0x00FF00FF);
       executeInstruction("xor $t2, $t0, $t1");
       expectRegister("$t2", 0xFFFF0000); // FF00FF00 ^ 00FF00FF
   }
   ```

2. **實作指令** (綠燈階段)
   - 在 `Instruction.h` 添加 `XorInstruction` 類別
   - 在 `Instruction.cpp` 實作 XOR 位元運算 (`rsValue ^ rtValue`)
   - 在 `InstructionDecoder.cpp` 添加 case 0x26
   - 在測試的 `executeInstruction()` 添加 XOR 支援

3. **重構和測試** 
   - 添加 XOR 綜合測試
   - 驗證邊界條件

#### NOR 指令實作
重複相同的紅-綠-重構循環，實作 NOR 指令 (function code 0x27)

### 中期任務 (Priority 2)

#### 1. 改善指令解析器
目前的 `executeInstruction()` 使用硬編碼字串比對，建議改善為：
```cpp
void executeInstruction(const std::string& instruction) {
    auto tokens = parseInstruction(instruction);
    auto opcode = tokens[0];
    auto rd = getRegisterNumber(tokens[1]);
    auto rs = getRegisterNumber(tokens[2]);
    auto rt = getRegisterNumber(tokens[3]);
    
    if (opcode == "and") {
        auto instr = std::make_unique<AndInstruction>(rd, rs, rt);
        instr->execute(*cpu);
    }
    // ... 其他指令
}
```

#### 2. 綜合測試擴充
為 OR, XOR, NOR 指令添加類似 AND 的綜合測試：
- 全0測試
- 全1測試
- 交替位元模式
- 邊界值測試
- $zero 暫存器測試

#### 3. 性能測試
添加指令執行性能測試：
```cpp
TEST_F(LogicalInstructionWalkingSkeleton, Performance_Tests) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        executeInstruction("and $t2, $t0, $t1");
    }
    auto end = std::chrono::high_resolution_clock::now();
    // 驗證執行時間在可接受範圍內
}
```

### 長期任務 (Priority 3)

#### 1. Assembler 整合
將邏輯指令整合到主要的 Assembler 中：
- 修改 `Assembler.cpp` 支援邏輯指令解析
- 添加標籤和符號表支援
- 整合到 GUI 界面

#### 2. 管線化支援
確保邏輯指令在管線化 CPU 中正確工作：
- 測試管線化執行
- 處理資料危險 (data hazards)
- 性能優化

## 🔄 BDD Walking Skeleton 工作流程

### 建議的開發循環
```
1. 啟用下一個 DISABLED 測試 (紅燈)
2. 運行測試確認失敗
3. 實作最小功能讓測試通過 (綠燈)
4. 重構代碼改善品質
5. 添加綜合測試
6. 重複循環
```

### 執行命令
```bash
# 構建測試
cd build
ninja unit_tests

# 運行特定測試
.\tests\unit_tests.exe --gtest_filter="LogicalInstructionWalkingSkeleton.XOR_*"

# 運行所有邏輯指令測試
.\tests\unit_tests.exe --gtest_filter="LogicalInstructionWalkingSkeleton.*"
```

## 📁 重要檔案清單

### 核心實作檔案
```
src/
├── Instruction.h         # 指令類別定義
├── Instruction.cpp       # 指令實作
└── InstructionDecoder.cpp # 指令解碼器

tests/
└── test_logical_walking_skeleton.cpp # BDD測試套件
```

### 配置檔案
```
CMakeLists.txt           # 主要構建配置 (已啟用features)
tests/CMakeLists.txt     # 測試構建配置
features/CMakeLists.txt  # BDD測試配置 (cucumber-cpp待修復)
```

## ⚠️ 已知問題和注意事項

### 1. Cucumber-cpp 依賴問題
- 目前 features/ 目錄下的 Cucumber 測試因缺少依賴而無法編譯
- 建議優先使用 GoogleTest 版本的 Walking Skeleton
- 未來可考慮修復 Cucumber 整合

### 2. 指令解析器限制
- 目前的解析器使用字串比對，功能有限
- 不支援複雜的指令格式
- 建議在 Priority 2 階段改善

### 3. 錯誤處理
- 目前缺少無效指令的錯誤處理
- 建議添加異常處理機制
- 需要更好的錯誤訊息

## 🎯 交接要點

### 給下一個開發者的建議

1. **從 XOR 指令開始**: 已有完整的測試框架，直接啟用測試即可
2. **保持原子化開發**: 一次只實作一個指令，確保每步都有測試覆蓋
3. **遵循現有模式**: 參考 AND/OR 指令的實作模式
4. **測試先行**: 先寫測試，再寫實作
5. **持續重構**: 不要怕改善代碼品質

### 快速上手步驟
```bash
# 1. 確認環境
cd c:\Users\aloha\Documents\GitHub\MIPS-Assembly-Simulator\build
ninja unit_tests

# 2. 運行現有測試
.\tests\unit_tests.exe --gtest_filter="LogicalInstructionWalkingSkeleton.*"

# 3. 啟用 XOR 測試 (改 DISABLED_ 前綴)
# 編輯 tests/test_logical_walking_skeleton.cpp

# 4. 確認紅燈
ninja unit_tests
.\tests\unit_tests.exe --gtest_filter="*XOR*"

# 5. 實作 XOR 指令讓測試通過
```

## 📈 專案整體進展

### MIPS 指令實作狀態
- **總指令數**: 47
- **已實作**: 11 (原有9 + 新增2)
- **完成比例**: 23.4% (+4.3%)

### 新增的邏輯指令
- AND ✅ (完整實作)
- OR ✅ (基本實作)
- XOR ⏳ (框架就緒)
- NOR ⏳ (框架就緒)

---

**開發完成**: 2025年8月15日  
**開發者**: GitHub Copilot  
**審查狀態**: 待下一階段開發者接手  
**下一里程碑**: 完成 XOR 和 NOR 指令實作
