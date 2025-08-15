# 🚀 MIPS Assembly Simulator - 完整開發交接報告

## 📋 專案概要
**專案名稱:** MIPS Assembly Simulator  
**開發方法:** 嚴格 BDD (Behavior-Driven Development) 方法論  
**測試框架:** Google Test Framework  
**開發語言:** C++17  
**交接日期:** 2025年8月15日  

## 📊 當前開發狀態

### 🏗️ 程式架構分析

#### **核心架構組件:**
- **📁 Pipeline架構:** 5階段MIPS流水線 (IF, ID, EX, MEM, WB)
- **📁 指令系統:** 物件導向指令類別階層
- **📁 組譯器:** MIPS組合語言到機器碼的轉換
- **📁 解碼器:** 機器碼到指令物件的解碼  
- **📁 GUI系統:** ImGui-based 圖形介面
- **📁 測試框架:** Google Test + BDD方法論

#### **指令類別架構:**
```cpp
Instruction (基底類別)
├── RTypeInstruction (R-type指令基底)
│   ├── AddInstruction ✅ | SubInstruction ✅
│   ├── AndInstruction ✅ | OrInstruction ✅ 
│   ├── XorInstruction ✅ | NorInstruction ✅
│   ├── SltInstruction ✅ | SltuInstruction ✅
│   └── SllInstruction ⚠️ (已宣告，需BDD+Integration)
├── ITypeInstruction (I-type指令基底)
│   ├── AddiInstruction ✅ | SltiInstruction ⚠️
│   ├── SltiuInstruction ✅ | LwInstruction ✅
│   ├── SwInstruction ✅ | OriInstruction ⚠️
│   ├── AndiInstruction ⚠️ | XoriInstruction ⚠️
│   └── BneInstruction ✅
├── BranchInstruction (分支指令基底)
│   └── BeqInstruction ✅
└── JTypeInstruction (跳躍指令基底)
    └── JInstruction ✅
```

### ✅ 測試統計
- **總測試數:** 170 個測試 (+2 from NOR Integration)
- ### ✅ 成功標準

### ✅ NOR Integration測試完成標準 - **[已達成]**
- [x] **創建 test_nor_instruction.cpp** - 包含2個Integration測試 ✅
- [x] **Decoder Integration測試通過** - 驗證function code 0x27解碼 ✅
- [x] **Assembler Integration測試通過** - 驗證"nor"語法解析 ✅  
- [x] **所有現有測試繼續通過** - 168→170個測試全部PASSED ✅
- [x] **零編譯警告或錯誤** - ninja unit_tests編譯成功 ✅
- [x] **更新開發報告** - 在本文件記錄完成狀態 ✅100% (170/170)  
- **執行時間:** ~53ms
- **DISABLED測試:** 0 個 (全部已解決)
- **邏輯指令BDD測試:** 18 個測試 
- **Integration測試:** 19 個測試 (+2 NOR Integration)

### 🎯 最新完成成就 (本開發週期)

#### ✅ 邏輯指令群組BDD完整實作完成 (18個新場景)

**1. AND指令BDD完整實作完成** ✅
- **BDD Test Cases (2個):**
  - 互補位元模式測試: 0xF0F0F0F0 & 0x0F0F0F0F = 0x00000000
  - 交替位元模式測試: 0xAAAAAAAA & 0x55555555 = 0x00000000
- **Integration Tests (2個):**
  - Decoder Integration: InstructionDecoder 支援 function code 0x24  
  - Assembler Integration: Assembler 支援 "and" 語法解析

**2. OR指令BDD完整實作完成** ✅
- **BDD Test Cases (2個):**
  - 互補位元模式測試: 0xF0F0F0F0 | 0x0F0F0F0F = 0xFFFFFFFF
  - 與零值運算測試: 0xDEADBEEF | 0x00000000 = 0xDEADBEEF
- **Integration Tests (2個):**
  - Decoder Integration: InstructionDecoder 支援 function code 0x25
  - Assembler Integration: Assembler 支援 "or" 語法解析

**3. XOR指令BDD完整實作完成** ✅
- **BDD Test Cases (2個):**
  - 交替位元模式測試: 0xAAAAAAAA ^ 0x55555555 = 0xFFFFFFFF
  - 自我XOR測試: A ^ A = 0x00000000
- **Integration Tests (2個):**
  - Decoder Integration: InstructionDecoder 支援 function code 0x26
  - Assembler Integration: Assembler 支援 "xor" 語法解析

**4. NOR指令BDD實作完成** ✅ **[剛完成]**
- **BDD Test Cases (2個):**
  - 全零輸入測試: ~(0x00000000 | 0x00000000) = 0xFFFFFFFF
  - 全1輸入測試: ~(0xFFFFFFFF | 0xFFFFFFFF) = 0x00000000
- **Integration Tests (2個):** ✅ **[剛完成]**
  - Decoder Integration: InstructionDecoder 支援 function code 0x27 ✅
  - Assembler Integration: Assembler 支援 "nor" 語法解析 ✅

**測試進度:** 168 → 170 個測試 (+2)  
**新增指令:** NOR Integration測試完成
**BDD完成指令:** 4/4 邏輯指令 (100%)
**Integration完成指令:** 4/4 邏輯指令 (100%) ✅ **[剛完成]**

### 🔧 MIPS指令完整開發狀態 (17/47 = 36%)

| 指令 | 類型 | 功能 | Decoder | Assembler | BDD測試 | Integration | 狀態 |
|------|------|------|---------|-----------|---------|-------------|------|
| `add` | R-type | 加法運算 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `sub` | R-type | 減法運算 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `and` | R-type | 位元AND | ✅ | ✅ | **✅ BDD完成** | **✅ Integration完成** | **🆕 全面完成** |
| `or` | R-type | 位元OR | ✅ | ✅ | **✅ BDD完成** | **✅ Integration完成** | **🆕 全面完成** |
| `xor` | R-type | 位元XOR | ✅ | ✅ | **✅ BDD完成** | **✅ Integration完成** | **🆕 全面完成** |
| `nor` | R-type | 位元NOR | ✅ | ✅ | **✅ BDD完成** | **✅ Integration完成** | **� 全面完成** |
| `slt` | R-type | 有符號比較 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `sltu` | R-type | 無符號比較 | ✅ | ✅ | **✅ BDD完成** | ✅ | 完成 |
| `sll` | R-type | 左位移 | ✅ | ⚠️ | ⚠️ | ⚠️ | 需整合測試 |
| `syscall` | R-type | 系統呼叫 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `addi` | I-type | 立即值加法 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `slti` | I-type | 立即值有符號比較 | ✅ | ⚠️ | ⚠️ | ⚠️ | 需整合測試 |
| `sltiu` | I-type | 立即值無符號比較 | ✅ | ✅ | **✅ BDD完成** | ✅ | 完成 |
| `lw` | I-type | 載入字組 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `sw` | I-type | 儲存字組 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `beq` | I-type | 相等分支 | ✅ | ✅ | ✅ | ✅ | 完成 |
| `bne` | I-type | 不等分支 | ✅ | ✅ | **✅ BDD完成** | ✅ | 完成 |
| `j` | J-type | 無條件跳躍 | ✅ | ✅ | ✅ | ✅ | 完成 |

**🎯 邏輯指令群組完成度:** 4/4 = 100% (BDD測試) | 4/4 = 100% (Integration測試) ✅ **[完全完成]**

## 🗺️ 完整開發規劃 (17/47 → 47/47)

### 🎯 開發優先順序策略

基於複雜度、依賴關係和實用性分析，制定以下開發優先順序：

#### **🚀 Phase 1: 位移指令群組 (3指令) - 當前優先**
**目標:** 170 → 182 個測試 (+12)  
**難度:** 🟡 中等  
**預估時間:** 2-3小時

1. **`sll` (左位移) - 完善現有實作**
   - ✅ Decoder已完成 (function code 0x00)
   - ⚠️ 需要 Assembler 支援
   - ❌ 需要 BDD測試 (2場景)
   - ❌ 需要 Integration測試 (2測試)

2. **`srl` (右位移邏輯) - 全新實作**
   - ❌ 需要 InstructionDecoder (function code 0x02)
   - ❌ 需要 SrlInstruction類別
   - ❌ 需要 Assembler 支援
   - ❌ 需要 BDD測試 (2場景)
   - ❌ 需要 Integration測試 (2測試)

3. **`sra` (右位移算術) - 全新實作**
   - ❌ 需要 InstructionDecoder (function code 0x03)
   - ❌ 需要 SraInstruction類別
   - ❌ 需要 Assembler 支援
   - ❌ 需要 BDD測試 (2場景)
   - ❌ 需要 Integration測試 (2測試)

#### **🚀 Phase 2: 立即值邏輯指令群組 (3指令)**
**目標:** 182 → 200 個測試 (+18)  
**難度:** 🟡 中等偏高  
**預估時間:** 3-4小時

4. **`ori` (立即值OR) - 部分實作存在**
5. **`andi` (立即值AND) - 部分實作存在**
6. **`xori` (立即值XOR) - 部分實作存在**

#### **🚀 Phase 3: 分支指令群組 (2指令)**
**目標:** 200 → 212 個測試 (+12)  
**難度:** 🟡 中等  
**預估時間:** 2-3小時

7. **`blez` (小於等於零分支)**
8. **`bgtz` (大於零分支)**

#### **🚀 Phase 4: 無符號算術指令群組 (3指令)**
**目標:** 212 → 230 個測試 (+18)  
**難度:** 🟢 簡單  
**預估時間:** 2-3小時

9. **`addu` (無符號加法)**
10. **`subu` (無符號減法)**
11. **`addiu` (無符號立即值加法)**

#### **🚀 Phase 5: 變數位移指令群組 (3指令)**
**目標:** 230 → 248 個測試 (+18)  
**難度:** 🟡 中等  
**預估時間:** 3-4小時

12. **`sllv` (變數左位移)**
13. **`srlv` (變數右位移邏輯)**
14. **`srav` (變數右位移算術)**

#### **🚀 Phase 6: 跳躍指令群組 (3指令)**
**目標:** 248 → 266 個測試 (+18)  
**難度:** 🟡 中等偏高  
**預估時間:** 3-4小時

15. **`jr` (跳到暫存器)**
16. **`jalr` (跳到暫存器並連結)**
17. **`jal` (跳躍並連結)**

#### **🚀 Phase 7: 記憶體指令群組 (8指令)**
**目標:** 266 → 314 個測試 (+48)  
**難度:** 🔴 高  
**預估時間:** 6-8小時

18. **`lb` (載入位元組)**
19. **`lh` (載入半字)**
20. **`lbu` (載入無符號位元組)**
21. **`lhu` (載入無符號半字)**
22. **`sb` (儲存位元組)**
23. **`sh` (儲存半字)**
24. **`llo` (載入低位立即值)**
25. **`lhi` (載入高位立即值)**

#### **🚀 Phase 8: 乘除法指令群組 (4指令)**
**目標:** 314 → 338 個測試 (+24)  
**難度:** 🔴 高  
**預估時間:** 5-7小時

26. **`mult` (有符號乘法)** - 需要HI/LO暫存器
27. **`multu` (無符號乘法)** - 需要HI/LO暫存器
28. **`div` (有符號除法)** - 需要HI/LO暫存器
29. **`divu` (無符號除法)** - 需要HI/LO暫存器

#### **🚀 Phase 9: HI/LO指令群組 (4指令)**
**目標:** 338 → 362 個測試 (+24)  
**難度:** 🟡 中等  
**預估時間:** 3-4小時

30. **`mfhi` (從HI移動)**
31. **`mthi` (移動到HI)**
32. **`mflo` (從LO移動)**
33. **`mtlo` (移動到LO)**

#### **🚀 Phase 10: 補完階段 (4指令)**
**目標:** 362 → 386 個測試 (+24)  
**難度:** 🟡 中等  
**預估時間:** 3-4小時

34. **`slti` (立即值有符號比較)** - 完善現有實作
35. **`trap` (陷阱指令)**

### 📊 開發里程碑追蹤

| Phase | 指令群組 | 指令數 | 測試增量 | 累計測試 | 累計完成率 | 預估時間 |
|-------|----------|--------|----------|----------|------------|----------|
| **✅ 已完成** | 邏輯+基礎 | 17 | - | 170 | 36% | - |
| **🎯 Phase 1** | 位移指令 | 3 | +12 | 182 | 43% | 2-3h |
| **🎯 Phase 2** | 立即值邏輯 | 3 | +18 | 200 | 49% | 3-4h |
| **🎯 Phase 3** | 分支指令 | 2 | +12 | 212 | 53% | 2-3h |
| **🎯 Phase 4** | 無符號算術 | 3 | +18 | 230 | 60% | 2-3h |
| **🎯 Phase 5** | 變數位移 | 3 | +18 | 248 | 66% | 3-4h |
| **🎯 Phase 6** | 跳躍指令 | 3 | +18 | 266 | 72% | 3-4h |
| **🎯 Phase 7** | 記憶體指令 | 8 | +48 | 314 | 85% | 6-8h |
| **🎯 Phase 8** | 乘除法指令 | 4 | +24 | 338 | 91% | 5-7h |
| **🎯 Phase 9** | HI/LO指令 | 4 | +24 | 362 | 96% | 3-4h |
| **🎯 Phase 10** | 補完階段 | 2 | +24 | 386 | 100% | 3-4h |

**🎯 總開發時間預估:** 32-44小時  
**🎯 總測試目標:** 170 → 386 (+216個測試)

## 🎯 立即行動計劃 - Phase 1: 位移指令群組

### 🚀 下一步：SLL指令BDD+Integration完善

**當前狀態分析:**
- ✅ SllInstruction類別已存在 (`src/Instruction.h:286`)
- ✅ InstructionDecoder已支援 (function code 0x00)
- ⚠️ Assembler語法解析需確認
- ❌ 缺少BDD測試場景
- ❌ 缺少Integration測試

**立即任務 (預估1-1.5小時):**

#### **Phase 1.1: SLL指令BDD測試 - 嚴格BDD循環**

**A. 撰寫階段 (紅燈):**
1. 創建 `tests/test_logical_sll_bdd_minimal.cpp`
2. 實作2個BDD場景:
   - 基本左位移測試: 0x00000001 << 4 = 0x00000010
   - 邊界位移測試: 0x80000000 << 1 = 0x00000000 (溢位)
3. 確認測試失敗 (紅燈狀態)

**B. 實作階段 (綠燈):**
1. 檢查並完善 `SllInstruction::execute()` 方法
2. 確認 Assembler 支援 "sll $rd, $rt, shamt" 語法
3. 執行測試確認通過 (綠燈狀態)

**C. 重構階段:**
1. 代碼清理和重構
2. 全回歸測試: 170 → 172 (+2 BDD測試)

#### **Phase 1.2: SLL指令Integration測試**

**A. 撰寫階段 (紅燈):**
1. 創建 `tests/test_sll_instruction.cpp`
2. 實作2個Integration測試:
   - Decoder Integration: 驗證function code 0x00解碼
   - Assembler Integration: 驗證"sll"語法解析
3. 更新 `CMakeLists.txt`

**B. 實作階段 (綠燈):**
1. 確認Integration組件正常運作
2. 執行測試確認通過

**C. 重構階段:**
1. 全回歸測試: 172 → 174 (+2 Integration測試)

**成功標準:**
- [ ] 創建SLL BDD測試檔案
- [ ] 創建SLL Integration測試檔案  
- [ ] 所有現有測試繼續通過 (170→174)
- [ ] 零編譯警告或錯誤
- [ ] 更新開發報告進度

**完成狀態:**
- ✅ **NOR指令BDD測試:** 2個測試案例已完成並通過
- ✅ **NOR指令核心實作:** NorInstruction類別已實作並可執行
- ✅ **Assembler支援:** 已加入"nor"語法解析支援
- ✅ **Integration測試:** 2個Integration測試已實作並通過

**📊 NOR Integration測試實作成果:**
1. ✅ **創建 `test_nor_instruction.cpp`** - 完成
2. ✅ **實作2個Integration測試:**
   - Decoder Integration: 驗證 InstructionDecoder 正確解碼 NOR 指令 (function code 0x27) ✅
   - Assembler Integration: 驗證 Assembler 正確解析 "nor $rd, $rs, $rt" 語法 ✅

**🎉 完成後成果:**
- 測試數量: 168 → 170 (+2個Integration測試)
- 邏輯指令群組: 100% 完成 (BDD + Integration)
- 完成度提升: 邏輯指令群組全面完成

### 🚀 邏輯指令群組里程碑達成 🎉

**✅ 重大成就 - 邏輯指令群組100%完成:**
- **AND指令:** BDD測試 ✅ + Integration測試 ✅ = 完全完成
- **OR指令:** BDD測試 ✅ + Integration測試 ✅ = 完全完成  
- **XOR指令:** BDD測試 ✅ + Integration測試 ✅ = 完全完成
- **NOR指令:** BDD測試 ✅ + Integration測試 ✅ = 完全完成

**📈 測試覆蓋率:**
- 邏輯指令BDD測試: 18個測試 (4指令 × 2場景 + 10個其他邏輯測試)
- 邏輯指令Integration測試: 8個測試 (4指令 × 2類型測試)

### ⚡ 5分鐘環境驗證

```bash
# 1. 確認編譯正常
cd c:\Users\aloha\Documents\GitHub\MIPS-Assembly-Simulator\build
ninja unit_tests

# 2. 確認所有測試通過 - 應該顯示 170 個測試
.\tests\unit_tests.exe --gtest_brief
# 實際結果: [  PASSED  ] 170 tests ✅

# 3. 驗證邏輯指令BDD測試 - 應該顯示 18 個
.\tests\unit_tests.exe --gtest_filter="*Logical*" --gtest_brief
# 實際結果: [  PASSED  ] 18 tests ✅

# 4. 驗證Integration測試 - 應該顯示 19 個
.\tests\unit_tests.exe --gtest_filter="*Integration*" --gtest_brief  
# 實際結果: [  PASSED  ] 19 tests ✅ (包含2個新增的NOR Integration測試)

# 5. 驗證邏輯指令Integration測試 - 應該顯示 8 個
.\tests\unit_tests.exe --gtest_filter="*Instruction*Integration*" --gtest_brief
# 實際結果: [  PASSED  ] 14 tests (包含邏輯指令的8個 + 其他指令的6個)
```

### 🎯 中期開發建議

**邏輯指令群組已100%完成！下一階段建議:**

1. **選項A: 位移指令群組 (SLL, SRL, SRA)** 🎯 **[推薦優先]**
   - 3個R-type指令，已有基礎實作
   - 預估增加12個測試 (6 BDD + 6 Integration)
   - 難度: 中等
   - 可複用現有邏輯指令的測試結構

2. **選項B: 立即值邏輯指令 (ANDI, ORI, XORI)**
   - 3個I-type指令，需要新實作
   - 預估增加18個測試 (6 BDD + 12 Integration)  
   - 難度: 中等偏高

3. **選項C: 記憶體指令完善 (LB, LH, SB, SH)**
   - 4個I-type指令，涉及位元組操作
   - 預估增加24個測試 (8 BDD + 16 Integration)
   - 難度: 高

**建議優先順序:** A → B → C

## 🔄 嚴格 BDD 開發流程指南

### 📋 每個指令的標準BDD循環

**適用範圍:** 每個新指令都必須遵循此循環

#### **🔴 Phase A: 撰寫步驟 (紅燈階段)**

1. **創建BDD測試檔案**
```bash
# 命名規範: test_logical_{指令名}_bdd_minimal.cpp
# 例如: test_logical_sll_bdd_minimal.cpp
```

2. **實作BDD測試場景 (失敗狀態)**
```cpp
/**
 * @file test_logical_sll_bdd_minimal.cpp
 * @brief SLL指令的BDD測試場景
 * 
 * Phase A: 紅燈階段 - 故意失敗的測試
 */

class SllInstructionBDDTest : public ::testing::Test {
    // Setup/TearDown邏輯
    // Given-When-Then helper方法
};

TEST_F(SllInstructionBDDTest, Sll_BasicShift_ShouldShiftLeftCorrectly) {
    // Given: 暫存器包含初始值
    // When: 執行SLL指令
    // Then: 結果應該是左位移後的值
    FAIL() << "Phase A: 故意失敗 - 需要實作SLL指令";
}

TEST_F(SllInstructionBDDTest, Sll_OverflowShift_ShouldHandleOverflow) {
    // Given: 暫存器包含會溢位的值
    // When: 執行SLL指令
    // Then: 應該正確處理溢位
    FAIL() << "Phase A: 故意失敗 - 需要實作溢位處理";
}
```

3. **執行測試確認紅燈**
```bash
ninja unit_tests
.\tests\unit_tests.exe --gtest_filter="*Sll*" --gtest_brief
# 應該顯示: [  FAILED  ] 2 tests
```

#### **🟢 Phase B: 實作階段 (綠燈階段)**

1. **檢查核心實作**
```cpp
// 檢查 src/Instruction.h 中的類別宣告
// 檢查 src/Instruction.cpp 中的 execute() 實作
// 檢查 src/InstructionDecoder.cpp 中的解碼邏輯
// 檢查 src/Assembler.cpp 中的語法解析
```

2. **移除FAIL()並實作邏輯**
```cpp
TEST_F(SllInstructionBDDTest, Sll_BasicShift_ShouldShiftLeftCorrectly) {
    // Given
    given_register_contains("$t0", 0x00000001);
    
    // When  
    when_program_executed_for_cycles("sll $t1, $t0, 4", 1);
    
    // Then
    then_register_should_equal("$t1", 0x00000010);
}
```

3. **執行測試確認綠燈**
```bash
.\tests\unit_tests.exe --gtest_filter="*Sll*" --gtest_brief
# 應該顯示: [  PASSED  ] 2 tests
```

#### **♻️ Phase C: 重構階段**

1. **代碼重構**
   - 清理重複代碼
   - 改善命名
   - 優化演算法

2. **全回歸測試**
```bash
.\tests\unit_tests.exe --gtest_brief
# 確認測試數量增加且全部通過
```

3. **Integration測試循環**
   - 重複 A→B→C 循環建立Integration測試
   - 檔案命名: `test_{指令名}_instruction.cpp`

### 🎯 BDD最佳實踐原則

#### **✅ DO (應該做的)**
- 每次只實作一個指令
- 嚴格遵循紅燈→綠燈→重構循環
- 先寫失敗的測試，再實作功能
- 每個測試只驗證一個行為
- 使用描述性的測試名稱
- Given-When-Then結構清晰

#### **❌ DON'T (不應該做的)**
- 一次實作多個指令
- 跳過紅燈階段直接寫通過的測試
- 在沒有測試的情況下寫生產代碼
- 忽略重構階段
- 寫過於複雜的測試場景

### 📊 進度追蹤模板

每完成一個指令後，更新以下追蹤表：

```markdown
| 指令 | BDD測試 | Integration測試 | 測試數增量 | 累計測試數 | 狀態 |
|------|---------|----------------|-----------|-----------|------|
| sll  | ✅ 2個  | ✅ 2個        | +4        | 174       | 完成 |
| srl  | ⏳ 進行中 | ❌ 待實作    | +0        | 174       | 開發中 |
```

**Phase A: 創建test_nor_instruction.cpp (紅燈階段)**
```cpp
// 範例: tests/test_nor_instruction.cpp
TEST_F(NorInstructionIntegrationTest, NorInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    // Given: InstructionDecoder 已創建
    
    // When: 解碼 NOR 指令機器碼
    // NOR $t2, $t0, $t1 => R-type: op=0, rs=8($t0), rt=9($t1), rd=10($t2), shamt=0, funct=0x27
    uint32_t machineCode = (0x00 << 26) | (8 << 21) | (9 << 16) | (10 << 11) | (0 << 6) | 0x27;
    
    auto instruction = decoder->decode(machineCode);
    
    // Then: 應該成功解碼為 NorInstruction
    ASSERT_NE(instruction, nullptr) << "解碼器應該能夠解碼 NOR 指令";
    EXPECT_EQ(instruction->getName(), "nor") << "指令名稱應該是 'nor'";
}

TEST_F(NorInstructionIntegrationTest, NorInstruction_AssemblerIntegration_ShouldParseCorrectly) {
    // Given: Assembler 已創建
    
    // When: 解析 NOR 指令語法
    std::string assembly = "nor $t2, $t0, $t1";
    auto instructions = assembler->assemble(assembly);
    
    // Then: 應該成功解析為一條指令
    ASSERT_EQ(instructions.size(), 1) << "應該解析出恰好一條指令";
    ASSERT_NE(instructions[0], nullptr) << "解析的指令不應該為空";
    EXPECT_EQ(instructions[0]->getName(), "nor") << "指令名稱應該是 'nor'";
}
```

**Phase B: 更新CMakeLists.txt (綠燈階段)**
```cmake
# 在 tests/CMakeLists.txt 中加入:
test_nor_instruction.cpp               # NOR instruction Integration tests
```

**Phase C: 編譯與驗證 (重構階段)**
```bash
# 確認編譯通過
cd build && ninja unit_tests

# 執行NOR Integration測試
.\tests\unit_tests.exe --gtest_filter="*NorInstruction*Integration*"

# 確認所有測試通過 (應該是207個)
.\tests\unit_tests.exe --gtest_brief
```

### 📝 已實作的NOR指令核心組件

**✅ NorInstruction類別實作 (src/Instruction.cpp):**
```cpp
void NorInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = ~(rsValue | rtValue);  // Bitwise NOR operation (NOT OR)
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

**✅ Assembler支援已加入 (src/Assembler.cpp):**
```cpp
else if (opcode == "nor" && tokens.size() >= 4) {
    // Parse: nor $rd, $rs, $rt  
    // ... 解析邏輯 ...
    return std::make_unique<NorInstruction>(rd, rs, rt);
}
```

**✅ BDD測試已完成 (tests/test_logical_nor_bdd_minimal.cpp):**
- 全零輸入測試: ~(0 | 0) = 0xFFFFFFFF ✅
- 全1輸入測試: ~(0xFFFFFFFF | 0xFFFFFFFF) = 0 ✅

## 📁 關鍵檔案位置

### 🎯 NOR Integration測試需要修改的檔案
- **新建:** `tests/test_nor_instruction.cpp` - NOR指令Integration測試
- **修改:** `tests/CMakeLists.txt` - 加入新測試檔案

### 📊 已完成檔案清單
- **BDD測試檔案:** 
  - ✅ `tests/test_logical_and_bdd_minimal.cpp` (2個BDD場景)
  - ✅ `tests/test_logical_or_bdd_minimal.cpp` (2個BDD場景)  
  - ✅ `tests/test_logical_xor_bdd_minimal.cpp` (2個BDD場景)
  - ✅ `tests/test_logical_nor_bdd_minimal.cpp` (2個BDD場景)
- **Integration測試檔案:**
  - ✅ `tests/test_and_instruction.cpp` (2個Integration場景)
  - ✅ `tests/test_or_instruction.cpp` (2個Integration場景)
  - ✅ `tests/test_xor_instruction.cpp` (2個Integration場景)
  - ✅ `tests/test_nor_instruction.cpp` ✅ **[剛完成]** (2個Integration場景)
- **核心實作檔案:**
  - ✅ `src/Instruction.h` - NorInstruction類別宣告
  - ✅ `src/Instruction.cpp` - NorInstruction::execute()實作
  - ✅ `src/Assembler.cpp` - "nor"語法解析支援
  - ✅ `src/InstructionDecoder.cpp` - function code 0x27支援

### 📚 參考範例檔案
- **最佳參考:** `tests/test_xor_instruction.cpp` - 完全相同的結構
- **CMakeLists範例:** 查看現有Integration測試的加入方式
- **BDD範例:** `tests/test_logical_*_bdd_minimal.cpp` 系列

### 📊 文檔位置
- **本交接報告:** `docs/DEVELOPMENT_HANDOVER_REPORT.md`
- **開發狀態:** 可在此檔案追蹤整體進度

## �️ 開發工具與命令

### 🔍 常用搜尋命令
```bash
# 查找現有指令實作模式
grep -r "AndInstruction" src/
grep -r "function code" src/InstructionDecoder.cpp

# 查找測試範例
grep -r "Integration" tests/
```

### 🧪 測試命令
```bash
# 編譯專案
cd build && ninja unit_tests

# 測試特定指令
.\tests\unit_tests.exe --gtest_filter="*And*"

# 完整測試
.\tests\unit_tests.exe --gtest_brief

# 查看 DISABLED 測試
.\tests\unit_tests.exe --gtest_list_tests | grep DISABLED
```

### � 成功標準

### ✅ NOR Integration測試完成標準
- [ ] **創建 test_nor_instruction.cpp** - 包含2個Integration測試
- [ ] **Decoder Integration測試通過** - 驗證function code 0x27解碼
- [ ] **Assembler Integration測試通過** - 驗證"nor"語法解析  
- [ ] **所有現有測試繼續通過** - 205→207個測試全部PASSED
- [ ] **零編譯警告或錯誤** - ninja unit_tests編譯成功
- [ ] **更新開發報告** - 在本文件記錄完成狀態

### 📈 階段完成目標
- **✅ 達成目標:** 170 個測試 (完成168 + 2個NOR Integration)
- **🎯 下一目標:** 182+ 個測試 (加入位移指令群組BDD+Integration)
- **中期目標:** 200+ 個測試 (加入立即值邏輯指令)
- **長期目標:** 240+ 個測試 (記憶體指令完善)

## 🚀 開始下一個開發週期

### 🎉 邏輯指令群組完成慶祝

**✅ 重大里程碑達成:**
- 4個邏輯指令 (AND, OR, XOR, NOR) 100% 完成
- 18個BDD測試場景 + 8個Integration測試場景
- 26個邏輯指令相關測試全部通過
- 零技術債務，100%測試通過率

### 🚀 下一步：位移指令群組開發

**下一個建議開發目標：位移指令群組 (SLL, SRL, SRA)**

**準備工作檢查清單：**
```bash
# 檢查位移指令是否已有基礎實作
grep -r "SllInstruction\|SrlInstruction\|SraInstruction" src/

# 檢查現有位移指令測試
.\tests\unit_tests.exe --gtest_filter="*Sll*|*Srl*|*Sra*" --gtest_brief

# 檢查Assembler是否支援位移指令語法
grep -r "sll\|srl\|sra" src/Assembler.cpp
```

**開發計劃：**
1. **Phase 1:** SLL指令BDD測試 (2個場景)
2. **Phase 2:** SLL指令Integration測試 (2個測試)
3. **Phase 3:** SRL指令BDD測試 (2個場景)
4. **Phase 4:** SRL指令Integration測試 (2個測試)
5. **Phase 5:** SRA指令BDD測試 (2個場景)  
6. **Phase 6:** SRA指令Integration測試 (2個測試)

**預期成果：** 170 → 182 個測試 (+12)

## 🎯 開發交接總結

### ✅ 當前重大成就
1. **邏輯指令群組100%完成:** AND, OR, XOR, NOR (4/4) ✅
2. **程式架構完整建立:** Pipeline + 指令系統 + GUI ✅  
3. **BDD流程標準化:** 18個BDD場景 + 19個Integration測試 ✅
4. **測試框架成熟:** 170個測試，100%通過率 ✅
5. **完整開發規劃:** 47個指令的10階段開發路線圖 ✅

### 📊 完成度現況
- **已完成指令:** 17/47 = 36%
- **部分完成指令:** 5/47 = 11%  
- **待開發指令:** 25/47 = 53%
- **測試覆蓋率:** 170個測試，預計最終386個測試

### 🎯 立即下一步行動
**Phase 1 目標:** 位移指令群組 (SLL, SRL, SRA)
**優先任務:** SLL指令BDD+Integration完善
**預期成果:** 170 → 182個測試 (+12)
**預估時間:** 2-3小時

### 🗺️ 中長期發展路線
1. **短期 (1-2週):** Phase 1-3 (位移+立即值邏輯+分支) → 60%完成度
2. **中期 (3-4週):** Phase 4-6 (無符號+變數位移+跳躍) → 80%完成度  
3. **長期 (6-8週):** Phase 7-10 (記憶體+乘除+完整) → 100%完成度

### 🏗️ 架構優勢
- **物件導向設計:** 指令類別階層清晰
- **Pipeline架構:** 支援5階段MIPS流水線
- **測試驅動:** BDD方法論確保品質
- **模組化結構:** 各組件獨立可測試
- **GUI支援:** ImGui圖形介面整合

### 📝 開發方法論優勢
- **嚴格BDD循環:** 確保每個功能都有測試覆蓋
- **小步迭代:** 降低風險，容易除錯
- **持續集成:** 每個變更都有完整回歸測試
- **文檔驅動:** 每個階段都有清楚的文檔記錄

**記住核心原則:** 
> **小步前進 → 測試驅動 → 嚴格循環 → 持續重構** 🎯

---
**開發交接完成** | **邏輯指令群組100%達成** | **下一目標: 位移指令群組** 🚀  
**總進度: 17/47指令 (36%)** | **測試覆蓋: 170個測試** | **架構: Pipeline + BDD** ✨
