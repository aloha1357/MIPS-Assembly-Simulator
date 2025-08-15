# 🚀 MIPS Assembly Simulator - 完整開發交接報告

## 📋 專案概要
**專案名稱:** MIPS Assembly Simulator  
**開發方法:** 嚴格 BDD (Behavior-Driven Development) 方法論  
**測試框架:** Google Test Framework  
**開發語言:** C++17  
**交接日期:** 2025年8月15日  

## 📊 當前開發狀態

### ✅ 測試統計
- **總測試數:** 154 個測試
- **通過率:** 100% (154/154)
- **執行時間:** ~46ms
- **DISABLED測試:** 0 個 (全部已解決)

### 🎯 最新完成成就 (本開發週期)

#### ✅ BDD 整合測試完成 (6個新場景)
1. **BNE (Branch Not Equal) 指令整合測試**
   - Decoder Integration: InstructionDecoder 支援 opcode 0x05
   - Assembler Integration: Assembler 支援 "bne" 語法解析

2. **SLTIU (Set Less Than Immediate Unsigned) 指令整合測試**
   - Decoder Integration: InstructionDecoder 支援 opcode 0x0B
   - Assembler Integration: Assembler 支援 "sltiu" 語法解析

3. **SLTU (Set Less Than Unsigned) 指令整合測試**
   - Decoder Integration: InstructionDecoder 支援 function code 0x2B
   - Assembler Integration: Assembler 支援 "sltu" 語法解析

**測試進度:** 148 → 154 個測試 (+6)  
**DISABLED解決:** 6 → 0 個 (-6)

### 🔧 已完成指令清單 (19/47 = 40%)

| 指令 | 類型 | 功能 | Decoder | Assembler | 狀態 |
|------|------|------|---------|-----------|------|
| `add` | R-type | 加法運算 | ✅ | ✅ | 完成 |
| `sub` | R-type | 減法運算 | ✅ | ✅ | 完成 |
| `and` | R-type | 位元AND | ✅ | ⚠️ | 需整合測試 |
| `or` | R-type | 位元OR | ✅ | ⚠️ | 需整合測試 |
| `xor` | R-type | 位元XOR | ✅ | ⚠️ | 需整合測試 |
| `nor` | R-type | 位元NOR | ✅ | ⚠️ | 需整合測試 |
| `slt` | R-type | 有符號比較 | ✅ | ✅ | 完成 |
| `sltu` | R-type | 無符號比較 | ✅ | ✅ | **🆕 BDD完成** |
| `sll` | R-type | 左位移 | ✅ | ⚠️ | 需整合測試 |
| `syscall` | R-type | 系統呼叫 | ✅ | ✅ | 完成 |
| `addi` | I-type | 立即值加法 | ✅ | ✅ | 完成 |
| `slti` | I-type | 立即值有符號比較 | ✅ | ⚠️ | 需整合測試 |
| `sltiu` | I-type | 立即值無符號比較 | ✅ | ✅ | **🆕 BDD完成** |
| `lw` | I-type | 載入字組 | ✅ | ✅ | 完成 |
| `sw` | I-type | 儲存字組 | ✅ | ✅ | 完成 |
| `beq` | I-type | 相等分支 | ✅ | ✅ | 完成 |
| `bne` | I-type | 不等分支 | ✅ | ✅ | **🆕 BDD完成** |
| `j` | J-type | 無條件跳躍 | ✅ | ✅ | 完成 |

## 🎯 下一階段開發計劃

### 🚀 立即優先 - 邏輯指令 BDD 整合測試

**建議開發順序:**
1. **AND 指令整合測試** (預估2-3小時)
2. **OR 指令整合測試** (預估2-3小時)
3. **XOR 指令整合測試** (預估2-3小時)
4. **NOR 指令整合測試** (預估2-3小時)

**預期成果:**
- 新增 8 個 BDD 場景 (4指令 × 2場景)
- 測試數量: 154 → 162
- 完成度: 40% → 43%

### ⚡ 5分鐘環境驗證

```bash
# 1. 確認編譯正常
cd c:\Users\aloha\Documents\GitHub\MIPS-Assembly-Simulator\build
ninja unit_tests

# 2. 確認所有測試通過 - 應該顯示 154 個測試
.\tests\unit_tests.exe --gtest_brief
# 期望結果: [  PASSED  ] 154 tests

# 3. 驗證最新完成的整合測試 - 應該顯示 6 個
.\tests\unit_tests.exe --gtest_filter="*Integration*" 
# 期望結果: 6 個整合測試通過 (BNE, SLTIU, SLTU 各2個)
```

## 🔄 嚴格 BDD 開發流程

### Phase A: 紅燈階段 (寫失敗測試)
```cpp
// 範例: tests/test_and_instruction.cpp
TEST_F(AndInstructionTest, AndInstruction_DecoderIntegration_ShouldDecodeCorrectly) {
    mips::InstructionDecoder decoder;
    
    // AND $t0, $t1, $t2 => function code 0x24
    uint32_t machineCode = (0x00 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 0x24;
    
    auto instruction = decoder.decode(machineCode);
    ASSERT_NE(instruction, nullptr) << "解碼器無法解碼 AND 指令";
    EXPECT_EQ(instruction->getName(), "and");
}
```

### Phase B: 綠燈階段 (實作最小功能)
```cpp
// 在 src/InstructionDecoder.cpp 的 decodeRType 中加入:
case 0x24:  // AND instruction
    return std::make_unique<AndInstruction>(rd, rs, rt);

// 在 src/Assembler.cpp 的 parseInstruction 中加入:
else if (opcode == "and" && tokens.size() >= 4) {
    // R-type 解析邏輯...
    return std::make_unique<AndInstruction>(rd, rs, rt);
}
```

### Phase C: 重構階段 (回歸測試)
```bash
# 確認測試通過
.\tests\unit_tests.exe --gtest_filter="*AndInstruction*"

# 執行完整回歸測試
.\tests\unit_tests.exe --gtest_brief
```

## 📁 關鍵檔案位置

### � 需要修改的核心檔案
- `src/InstructionDecoder.cpp` - 加入新指令解碼支援
- `src/Assembler.cpp` - 加入新指令組譯支援
- `tests/test_*_instruction.cpp` - 建立新的整合測試

### � 參考範例
- `tests/test_bne_instruction.cpp` - BNE 整合測試範例
- `tests/test_sltiu_instruction.cpp` - SLTIU 整合測試範例
- `tests/test_slt_instruction.cpp` - 包含 SLTU 整合測試

### 📊 文檔更新
- `DEVELOPMENT_STATUS_REPORT.md` - 開發狀態報告
- 本文件 - 完成後更新進度

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

## ⚠️ 重要注意事項

### 🎯 開發原則
1. **嚴格遵循 BDD**: 必須按 A→B→C 循環，不要跳過紅燈階段
2. **一次一個場景**: 不要同時開發多個指令的整合測試
3. **即時驗證**: 每個階段完成後立即運行測試
4. **保持簡潔**: 實作最小可行功能，避免過度工程

### 🔧 技術限制
1. **RTypeInstruction getter**: 當前缺少 public getter 方法，測試只能驗證指令名稱
2. **命名空間**: 使用 `mips::` 命名空間，注意 include 正確的標頭檔
3. **機器碼格式**: R-type 指令使用 function code，I-type 使用 opcode

## 🏆 成功標準

### ✅ 每個指令整合測試完成標準
- [ ] Decoder Integration 測試通過
- [ ] Assembler Integration 測試通過  
- [ ] 所有現有測試繼續通過 (回歸測試)
- [ ] 零編譯警告或錯誤
- [ ] 更新開發報告

### 📈 階段完成目標
- **下一階段:** 162 個測試 (當前154 + 8個邏輯指令整合)
- **中期目標:** 170+ 個測試 (加入 I-type 指令整合)
- **長期目標:** 180+ 個測試 (記憶體和跳躍指令完善)

## 🚀 開始下一個開發週期

### 建議開發流程 (AND 指令範例)
```bash
# 1. 建立測試檔案
code tests/test_and_instruction.cpp

# 2. 實作 Decoder Integration 測試 (Phase A)
# 3. 在 InstructionDecoder.cpp 加入支援 (Phase B)  
# 4. 驗證測試通過 (Phase C)
# 5. 實作 Assembler Integration 測試 (Phase A)
# 6. 在 Assembler.cpp 加入支援 (Phase B)
# 7. 驗證測試通過並回歸測試 (Phase C)
```

**記住**: 小步前進，測試驅動，嚴格遵循 BDD！🎯

---
**交接完成** | **準備繼續 BDD 開發** | **下一目標: AND 指令整合測試** 🚀
