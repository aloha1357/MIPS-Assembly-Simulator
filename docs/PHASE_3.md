## 🚀 Phase 3 開發指南 - 分支指令群組 (BLEZ, BGTZ)

### 3.1 Phase 3 開發目標
Phase 3 將完成MIPS分支指令群組的實現，包含兩個關鍵的分支指令：

#### 目標指令清單
1. **BLEZ** (Branch Less than or Equal Zero) - I-type 指令
   - **Opcode:** 0x06
   - **語法:** `blez $rs, offset`
   - **功能:** 如果 $rs ≤ 0 則分支
   
2. **BGTZ** (Branch Greater Than Zero) - I-type 指令
   - **Opcode:** 0x07  
   - **語法:** `bgtz $rs, offset`
   - **功能:** 如果 $rs > 0 則分支

### 3.2 BDD實現週期規劃

#### 🔴 Phase 3.1: BLEZ指令 Red-Light Phase
**目標檔案:** `tests/test_blez_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 正數測試 (不應分支)
DISABLED_TEST(BLEZInstructionBDD, PositiveValueNoBranch)

// 情境2: 零值測試 (應該分支) 
DISABLED_TEST(BLEZInstructionBDD, ZeroValueBranch)

// 情境3: 負數測試 (應該分支)
DISABLED_TEST(BLEZInstructionBDD, NegativeValueBranch)

// 情境4: 邊界值測試 (INT_MIN應該分支)
DISABLED_TEST(BLEZInstructionBDD, MinIntValueBranch)
```

#### 🟢 Phase 3.2: BLEZ指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增case 0x06路由
2. **Assembler.cpp** - 新增"blez"語法解析
3. **指令類別** - 確認BLEZInstruction類別存在且功能完整

#### 🔄 Phase 3.3: BLEZ指令 Integration Testing
**目標檔案:** `tests/test_blez_instruction_integration.cpp`

**完整整合測試 (約6-7個測試):**
- 組合器語法解析測試
- 機器碼解碼測試  
- PC分支跳轉計算測試
- 邊界條件測試
- 錯誤處理測試

#### 🔴 Phase 3.4: BGTZ指令 Red-Light Phase
**目標檔案:** `tests/test_bgtz_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 正數測試 (應該分支)
DISABLED_TEST(BGTZInstructionBDD, PositiveValueBranch)

// 情境2: 零值測試 (不應分支)
DISABLED_TEST(BGTZInstructionBDD, ZeroValueNoBranch)

// 情境3: 負數測試 (不應分支)  
DISABLED_TEST(BGTZInstructionBDD, NegativeValueNoBranch)

// 情境4: 邊界值測試 (INT_MAX應該分支)
DISABLED_TEST(BGTZInstructionBDD, MaxIntValueBranch)
```

#### 🟢 Phase 3.5: BGTZ指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增case 0x07路由
2. **Assembler.cpp** - 新增"bgtz"語法解析
3. **指令類別** - 確認BGTZInstruction類別存在且功能完整

#### 🔄 Phase 3.6: BGTZ指令 Integration Testing
**目標檔案:** `tests/test_bgtz_instruction_integration.cpp`

### 3.3 預期開發成果

#### 量化目標
- **新增測試數:** +12-14個測試 (213 → 225-227)
- **指令完成:** 2個I-type分支指令
- **完成度提升:** 49% → 53%
- **估計開發時間:** 4-5小時

#### 質化目標
- **分支邏輯驗證:** 完整測試分支條件判斷
- **PC計算準確性:** 驗證相對偏移計算
- **邊界條件覆蓋:** INT_MIN, INT_MAX, 0值測試
- **BDD方法論延續:** 保持嚴格red-light → green-light → refactor週期

### 3.4 技術實現細節

#### 分支指令關鍵特性
1. **I-type格式:** rs, rt=0, 16位元立即值偏移
2. **PC相對偏移:** offset左移2位元後與PC+4相加
3. **條件判斷:** 需要正確處理有符號整數比較
4. **流水線影響:** 分支指令對流水線的影響

#### 預期實現模式
```cpp
// InstructionDecoder.cpp
case 0x06: return decodeIType(word, "BLEZ");
case 0x07: return decodeIType(word, "BGTZ");

// Assembler.cpp  
if (instruction == "blez") return new BLEZInstruction(rs, parseOffset(tokens[2]));
if (instruction == "bgtz") return new BGTZInstruction(rs, parseOffset(tokens[2]));
```

### 3.5 Phase 3 開發檢核清單

#### 🔴 Red-Light階段檢核
- [ ] BLEZ BDD測試檔案建立 (4個DISABLED測試)
- [ ] BGTZ BDD測試檔案建立 (4個DISABLED測試)  
- [ ] CMakeLists.txt更新包含新測試檔案
- [ ] 編譯確認 - 應該有8個失敗測試

#### 🟢 Green-Light階段檢核
- [ ] InstructionDecoder.cpp新增兩個opcode路由
- [ ] Assembler.cpp新增兩個語法解析器
- [ ] BDD測試全部通過 (移除DISABLED前綴)
- [ ] 零編譯警告確認

#### 🔄 Integration階段檢核
- [ ] BLEZ整合測試檔案建立 (6-7個測試)
- [ ] BGTZ整合測試檔案建立 (6-7個測試)
- [ ] 完整回歸測試 (所有213+12個測試通過)
- [ ] Phase 3完成驗收

### 3.6 風險管控與除錯策略

#### 已知風險點
1. **分支邏輯複雜性:** 條件判斷可能有邊界錯誤
2. **PC計算準確性:** 相對偏移計算容易出錯
3. **有符號比較:** C++的有符號/無符號轉換陷阱

#### 除錯策略
1. **單一條件測試:** 每個分支條件獨立驗證
2. **邊界值重點測試:** 0, INT_MIN, INT_MAX特殊處理
3. **逐步增量實現:** 先實現基本功能，再處理邊界
4. **對比參考實現:** 必要時參考MIPS官方文件

### 3.7 Phase 3成功標準

#### 技術指標
- **213 → 225個測試:** 100%通過率
- **零編譯錯誤:** 完整編譯成功
- **BDD循環完整:** 完成6次完整red-light → green-light循環
- **回歸穩定性:** 原有功能保持完整

#### 驗收標準  
- **BLEZ功能:** 正確判斷 ≤ 0 條件分支
- **BGTZ功能:** 正確判斷 > 0 條件分支
- **整合品質:** Assembler + Decoder完整支援
- **程式碼品質:** 符合專案架構標準

### 3.8 Phase 4預告

Phase 3完成後，下一階段將進入：
- **Phase 4:** 無符號算術指令群組 (ADDIU等)
- **目標測試數:** 225 → 240個測試
- **完成度目標:** 53% → 58%

---

### 🎯 立即下一步行動
**Phase 3.1啟動:** BLEZ指令BDD Red-Light Phase
**首要任務:** 建立 `tests/test_blez_instruction_bdd_minimal.cpp`
**預期成果:** 4個DISABLED_測試，編譯成功但測試失敗
**預估時間:** 30分鐘
**成功指標:** 編譯無錯誤，4個紅燈測試

### 🗺️ 中長期發展路線
1. **✅ 已完成 (Phase 1):** 位移指令群組100% → 182個測試 → 43%總完成度
2. **✅ 已完成 (Phase 2):** 立即值邏輯指令群組100% → 213個測試 → 49%總完成度  
3. **🚀 進行中 (Phase 3):** 分支指令群組 → 225個測試 → 53%總完成度
4. **📋 規劃中 (Phase 4-5):** 無符號算術+其他指令 → 260個測試 → 65%總完成度
5. **🎯 長期目標 (Phase 6-10):** 記憶體+乘除+完整系統 → 386個測試 → 100%總完成度

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
- **零編譯警告:** 178個測試全部編譯成功
- **零失敗測試:** 100%通過率維持
- **檔案結構健全:** 所有核心檔案結構完整
- **代碼品質良好:** 遵循物件導向和BDD最佳實踐

**下一位開發者接手指南:**
1. 先執行 `ninja unit_tests && .\tests\unit_tests.exe` 確認當前狀態
2. 參考 `tests/test_srl_instruction.cpp` 作為SRA實現模板
3. 遵循嚴格BDD循環：先寫失敗測試，再實現功能
4. 每個變更後都執行完整回歸測試
5. 使用 `docs/DEVELOPMENT_HANDOVER_REPORT.md` 追蹤進度

**記住核心原則:** 
> **小步前進 → 測試驅動 → 嚴格循環 → 持續重構** 🎯

---
**✅ Phase 2 開發交接完成** | **✅ 立即值邏輯指令群組100%達成** | **🚀 下一目標: Phase 3分支指令群組** 🚀  
**總進度: 23/47指令 (49%)** | **測試覆蓋: 213個測試** | **架構: Pipeline + BDD** ✨

**Phase 3 準備就緒 - 分支指令群組 (BLEZ, BGTZ) 實現指南已完整建立** 🎯
