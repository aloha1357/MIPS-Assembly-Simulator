## 🚀 Phase 4 開發指南 - 無符號算術指令群組 (ADDIU, SUBU, ADDU)

### 4.1 Phase 4 開發目標
Phase 4 將完成MIPS無符號算術指令群組的實現，包含三個關鍵的算術指令：

#### 目標指令清單
1. **ADDIU** (Add Immediate Unsigned) - I-type 指令
   - **Opcode:** 0x09
   - **語法:** `addiu $rt, $rs, immediate`
   - **功能:** 無符號立即值加法，不產生溢位異常
   
2. **SUBU** (Subtract Unsigned) - R-type 指令
   - **Opcode:** 0x00, **Function:** 0x23
   - **語法:** `subu $rd, $rs, $rt`
   - **功能:** 無符號減法，不產生溢位異常
   
3. **ADDU** (Add Unsigned) - R-type 指令
   - **Opcode:** 0x00, **Function:** 0x21
   - **語法:** `addu $rd, $rs, $rt`
   - **功能:** 無符號加法，不產生溢位異常

### 4.2 BDD實現週期規劃

#### 🔴 Phase 4.1: ADDIU指令 Red-Light Phase
**目標檔案:** `tests/test_addiu_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 基本無符號加法
DISABLED_TEST(ADDIUInstructionBDD, BasicUnsignedAddition)

// 情境2: 溢位情況處理 (不產生異常)
DISABLED_TEST(ADDIUInstructionBDD, OverflowHandling)

// 情境3: 負立即值加法
DISABLED_TEST(ADDIUInstructionBDD, NegativeImmediateAddition)

// 情境4: 最大值邊界測試
DISABLED_TEST(ADDIUInstructionBDD, MaxValueBoundary)
```

#### 🟢 Phase 4.2: ADDIU指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增case 0x09路由
2. **Assembler.cpp** - 新增"addiu"語法解析
3. **指令類別** - 確認ADDIUInstruction類別存在且功能完整

#### 🔄 Phase 4.3: ADDIU指令 Integration Testing
**目標檔案:** `tests/test_addiu_instruction_integration.cpp`

#### 🔴 Phase 4.4: SUBU指令 Red-Light Phase
**目標檔案:** `tests/test_subu_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 基本無符號減法
DISABLED_TEST(SUBUInstructionBDD, BasicUnsignedSubtraction)

// 情境2: 下溢情況處理 (不產生異常)
DISABLED_TEST(SUBUInstructionBDD, UnderflowHandling)

// 情境3: 相等數值減法 (結果為零)
DISABLED_TEST(SUBUInstructionBDD, EqualValuesSubtraction)

// 情境4: 大數減小數
DISABLED_TEST(SUBUInstructionBDD, LargeMinusSmallValues)
```

#### 🟢 Phase 4.5: SUBU指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增function 0x23路由
2. **Assembler.cpp** - 新增"subu"語法解析
3. **指令類別** - 確認SUBUInstruction類別存在且功能完整

#### 🔄 Phase 4.6: SUBU指令 Integration Testing
**目標檔案:** `tests/test_subu_instruction_integration.cpp`

#### 🔴 Phase 4.7: ADDU指令 Red-Light Phase
**目標檔案:** `tests/test_addu_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 基本無符號加法
DISABLED_TEST(ADDUInstructionBDD, BasicUnsignedAddition)

// 情境2: 溢位情況處理 (不產生異常)
DISABLED_TEST(ADDUInstructionBDD, OverflowHandling)

// 情境3: 零值加法
DISABLED_TEST(ADDUInstructionBDD, ZeroValueAddition)

// 情境4: 最大值邊界測試
DISABLED_TEST(ADDUInstructionBDD, MaxValueBoundary)
```

#### 🟢 Phase 4.8: ADDU指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增function 0x21路由
2. **Assembler.cpp** - 新增"addu"語法解析
3. **指令類別** - 確認ADDUInstruction類別存在且功能完整

#### 🔄 Phase 4.9: ADDU指令 Integration Testing
**目標檔案:** `tests/test_addu_instruction_integration.cpp`

### 4.3 預期開發成果

#### 量化目標
- **新增測試數:** +18-21個測試 (232 → 250-253)
- **指令完成:** 3個無符號算術指令
- **完成度提升:** 54% → 60%
- **估計開發時間:** 6-7小時

#### 質化目標
- **溢位處理驗證:** 確認無符號指令不產生溢位異常
- **數據型別正確性:** 驗證無符號/有符號運算區別
- **邊界條件覆蓋:** UINT_MAX, 0值, 溢位情況測試
- **BDD方法論延續:** 保持嚴格red-light → green-light → refactor週期

### 4.4 技術實現細節

#### 無符號算術指令關鍵特性
1. **溢位行為:** 不產生溢位異常，結果以模運算處理
2. **數據解釋:** 寄存器值以無符號整數解釋
3. **ADDIU特殊性:** 儘管名稱為"無符號"，實際上符號擴展立即值
4. **與有符號指令區別:** ADD/SUB會檢查溢位，ADDU/SUBU不會

#### 預期實現模式
```cpp
// InstructionDecoder.cpp
case 0x09: return decodeIType(word, "ADDIU");
// 在function switch中：
case 0x21: return decodeRType(word, "ADDU");
case 0x23: return decodeRType(word, "SUBU");

// Assembler.cpp  
if (instruction == "addiu") return new ADDIUInstruction(rt, rs, immediate);
if (instruction == "addu") return new ADDUInstruction(rd, rs, rt);
if (instruction == "subu") return new SUBUInstruction(rd, rs, rt);
```

### 4.5 與現有指令的差異分析

#### ADDIU vs ADDI
- **ADDI:** 檢查有符號溢位，溢位時產生異常
- **ADDIU:** 不檢查溢位，結果直接寫入目標暫存器

#### SUBU vs SUB  
- **SUB:** 檢查有符號溢位，溢位時產生異常
- **SUBU:** 不檢查溢位，允許"借位"操作

#### ADDU vs ADD
- **ADD:** 檢查有符號溢位，溢位時產生異常
- **ADDU:** 不檢查溢位，純二進位加法

### 4.6 Phase 4 開發檢核清單

#### 🔴 Red-Light階段檢核
- [ ] ADDIU BDD測試檔案建立 (4個DISABLED測試)
- [ ] SUBU BDD測試檔案建立 (4個DISABLED測試)  
- [ ] ADDU BDD測試檔案建立 (4個DISABLED測試)
- [ ] CMakeLists.txt更新包含新測試檔案
- [ ] 編譯確認 - 應該有12個跳過測試

#### 🟢 Green-Light階段檢核
- [ ] InstructionDecoder.cpp新增三個指令路由
- [ ] Assembler.cpp新增三個語法解析器
- [ ] BDD測試全部通過 (移除DISABLED前綴)
- [ ] 零編譯警告確認

#### 🔄 Integration階段檢核
- [ ] ADDIU整合測試檔案建立 (6-7個測試)
- [ ] SUBU整合測試檔案建立 (6-7個測試)
- [ ] ADDU整合測試檔案建立 (6-7個測試)
- [ ] 完整回歸測試 (所有250+個測試通過)
- [ ] Phase 4完成驗收

### 4.7 風險管控與除錯策略

#### 已知風險點
1. **溢位處理差異:** 確保無符號指令不產生異常
2. **數據型別混淆:** 無符號/有符號運算邏輯區別
3. **立即值符號擴展:** ADDIU的立即值處理特殊性

#### 除錯策略
1. **對比測試:** 與對應的有符號指令進行行為對比
2. **邊界值重點測試:** UINT_MAX, 0, 溢位邊界特殊處理
3. **逐步增量實現:** 先實現基本功能，再處理邊界
4. **參考MIPS手冊:** 確認無符號運算的確切行為

### 4.8 Phase 4成功標準

#### 技術指標
- **232 → 250個測試:** 100%通過率
- **零編譯錯誤:** 完整編譯成功
- **BDD循環完整:** 完成9次完整red-light → green-light循環
- **回歸穩定性:** 原有功能保持完整

#### 驗收標準  
- **ADDIU功能:** 正確無符號立即值加法，無溢位異常
- **SUBU功能:** 正確無符號減法，處理下溢情況
- **ADDU功能:** 正確無符號加法，無溢位異常
- **整合品質:** Assembler + Decoder完整支援
- **程式碼品質:** 符合專案架構標準

### 4.9 Phase 5預告

Phase 4完成後，下一階段將進入：
- **Phase 5:** 記憶體存取指令群組 (LB, LH, SB, SH等)
- **目標測試數:** 250 → 275個測試
- **完成度目標:** 60% → 68%

---

### 🎯 立即下一步行動
**Phase 4.1啟動:** ADDIU指令BDD Red-Light Phase
**首要任務:** 建立 `tests/test_addiu_instruction_bdd_minimal.cpp`
**預期成果:** 4個DISABLED_測試，編譯成功但測試跳過
**預估時間:** 30分鐘
**成功指標:** 編譯無錯誤，4個跳過測試

### 🗺️ 中長期發展路線
1. **✅ 已完成 (Phase 1):** 位移指令群組100% → 182個測試 → 43%總完成度
2. **✅ 已完成 (Phase 2):** 立即值邏輯指令群組100% → 213個測試 → 49%總完成度  
3. **✅ 已完成 (Phase 3):** 分支指令群組100% → 232個測試 → 54%總完成度
4. **🚀 進行中 (Phase 4):** 無符號算術指令群組 → 250個測試 → 60%總完成度
5. **📋 規劃中 (Phase 5-6):** 記憶體存取+其他指令 → 300個測試 → 75%總完成度
6. **🎯 長期目標 (Phase 7-10):** 乘除+浮點+完整系統 → 386個測試 → 100%總完成度

### 🏗️ 架構優勢維持
- **嚴格BDD方法論:** 每個指令都有完整測試覆蓋
- **物件導向設計:** 指令類別階層清晰穩定
- **Pipeline架構:** 5階段MIPS流水線支援完整
- **模組化結構:** 各組件獨立且可測試
- **持續整合:** 每個變更都有完整回歸測試

### 📝 關鍵經驗總結
1. **檔案結構完整性:** Instruction.h等核心檔案的完整性對編譯至關重要
2. **二進制編碼精確性:** 使用位運算而非硬編碼十六進制值的重要性
3. **BDD循環有效性:** 紅燈→綠燈→重構的嚴格執行保證品質
4. **Integration測試模式:** 解碼器+組譯器雙重驗證的必要性
5. **測試驅動開發:** 小步迭代降低風險，容易除錯和維護

### 🚀 技術債務狀態
- **零編譯警告:** 232個測試全部編譯成功
- **零失敗測試:** 100%通過率維持
- **檔案結構健全:** 所有核心檔案結構完整
- **代碼品質良好:** 遵循物件導向和BDD最佳實踐

**下一位開發者接手指南:**
1. 先執行 `ninja unit_tests && .\tests\unit_tests.exe` 確認當前狀態
2. 參考已完成的BGTZ指令實現作為模板
3. 遵循嚴格BDD循環：先寫失敗測試，再實現功能
4. 每個變更後都執行完整回歸測試
5. 使用此文檔追蹤進度

**記住核心原則:** 
> **小步前進 → 測試驅動 → 嚴格循環 → 持續重構** 🎯

---
**✅ Phase 3 開發交接完成** | **✅ 分支指令群組100%達成** | **🚀 下一目標: Phase 4無符號算術指令群組** 🚀  
**總進度: 25/47指令 (54%)** | **測試覆蓋: 232個測試** | **架構: Pipeline + BDD** ✨

**Phase 4 準備就緒 - 無符號算術指令群組 (ADDIU, SUBU, ADDU) 實現指南已完整建立** 🎯
