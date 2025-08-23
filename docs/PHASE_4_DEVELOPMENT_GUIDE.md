# 🚀 Phase 4 開發指南 - 無符號算術指令群組

## 📋 當前狀態總結 (2025年8月15日更新)

### ✅ Phase 3 完成成果
- **BGTZ指令實現完成:** 232個測試全部通過
- **分支指令群組100%完成:** BLEZ + BGTZ 雙指令全面實現
- **測試增長:** 從219個增加到232個測試 (+13個測試)
- **完成度提升:** 從52%提升到53% (25/47指令完成)

### 🎯 Phase 4 目標：無符號算術指令群組

#### 目標指令清單
1. **ADDIU** (Add Immediate Unsigned) - I-type 指令
   - **Opcode:** 0x09
   - **語法:** `addiu $rt, $rs, immediate`
   - **功能:** rt = rs + sign_extend(immediate) (無溢位檢查)
   
2. **SUBU** (Subtract Unsigned) - R-type 指令
   - **Function Code:** 0x23
   - **語法:** `subu $rd, $rs, $rt`
   - **功能:** rd = rs - rt (無溢位檢查)

3. **ADDU** (Add Unsigned) - R-type 指令
   - **Function Code:** 0x21
   - **語法:** `addu $rd, $rs, $rt`
   - **功能:** rd = rs + rt (無溢位檢查)

## 🎯 BDD開發計劃

### Phase 4.1: ADDIU指令 (Red-Light → Green-Light → Integration)

**開發順序:**
1. **Red-Light Phase:** 創建 `tests/test_addiu_instruction_bdd_minimal.cpp` (4個DISABLED測試)
2. **Green-Light Phase:** 實現ADDIU指令類別和核心組件
3. **Integration Phase:** 創建Integration測試

**關鍵技術要點:**
- **無溢位檢查:** 不產生算術溢位異常
- **有符號擴展:** 儘管名稱為"unsigned"，立即值仍使用有符號擴展
- **參考實現:** 可參考ADDI指令的實現模式

### Phase 4.2: SUBU指令 (R-type實現)

**開發重點:**
- R-type指令格式
- 無下溢檢查邏輯
- 參考SUB指令實現

### Phase 4.3: ADDU指令 (R-type實現)

**開發重點:**
- R-type指令格式 
- 無溢位檢查邏輯
- 參考ADD指令實現

## 📊 預期成果

### 量化目標
- **新增測試數:** +18-21個測試 (232 → 250-253)
- **指令完成:** 3個無符號算術指令
- **完成度提升:** 53% → 60%
- **估計開發時間:** 6-8小時

### 完成標準
- **功能驗證:** 所有3個指令正確實現無溢位算術
- **測試覆蓋:** 完整BDD + Integration測試
- **回歸穩定:** 所有現有232個測試繼續通過
- **零編譯錯誤:** 完整編譯成功

## 🚀 立即行動計劃

### Step 1: 環境驗證 (5分鐘)
```bash
cd c:\Users\aloha\Documents\GitHub\MIPS-Assembly-Simulator\build
.\tests\unit_tests.exe --gtest_brief
# 確認顯示: [  PASSED  ] 232 tests
```

### Step 2: 開始ADDIU開發 (45分鐘)
1. 創建 `tests/test_addiu_instruction_bdd_minimal.cpp`
2. 實現4個DISABLED_測試場景
3. 更新CMakeLists.txt
4. 編譯確認紅燈狀態

### Step 3: ADDIU實現 (60分鐘)
1. 在 `src/Instruction.h` 添加ADDIUInstruction類別
2. 在 `src/Instruction.cpp` 實現execute()方法
3. 在 `src/InstructionDecoder.cpp` 添加case 0x09
4. 在 `src/Assembler.cpp` 添加"addiu"語法解析
5. 移除DISABLED_前綴，確認綠燈狀態

### Step 4: Integration測試 (30分鐘)
1. 創建 `tests/test_addiu_instruction_integration.cpp`
2. 實現解碼器和組譯器整合測試
3. 完整回歸測試確認

## 📁 參考文件

### 實現參考
- **ADDI實現:** `src/Instruction.cpp` 中的ADDIInstruction類別
- **I-type解碼:** `src/InstructionDecoder.cpp` 中的I-type處理
- **語法解析:** `src/Assembler.cpp` 中的立即值指令解析

### 測試參考
- **BDD模式:** `tests/test_addi_instruction_bdd_minimal.cpp` (如果存在)
- **Integration模式:** 任何現有的Integration測試文件

## 🎯 成功指標

### Phase 4完成檢核清單
- [ ] ADDIU指令100%實現 (BDD + Integration)
- [ ] SUBU指令100%實現 (BDD + Integration) 
- [ ] ADDU指令100%實現 (BDD + Integration)
- [ ] 總測試數達到250個以上
- [ ] 完成度達到60%
- [ ] 零編譯警告和錯誤
- [ ] 完整回歸測試通過

### Phase 5準備
完成Phase 4後，下一階段將進入：
- **Phase 5:** 變數位移指令群組 (SLLV, SRLV, SRAV)
- **目標測試數:** 250 → 268個測試
- **完成度目標:** 60% → 66%

---

**🎯 下一位開發者立即任務:** 開始ADDIU指令BDD Red-Light Phase開發
**預期時間投入:** 2-3小時完成ADDIU指令
**關鍵原則:** 嚴格遵循BDD red-light → green-light → refactor週期
