# 🚀 Phase 1.5 開發指南：SRA指令完整實現

## 📋 開發目標

**目標指令：** SRA (Shift Right Arithmetic) - 算術右位移  
**當前狀態：** 178個測試  
**目標狀態：** 182個測試 (+4個測試)  
**完成里程碑：** 位移指令群組100%完成 (SLL ✅ + SRL ✅ + SRA ✅)

## 🎯 Phase 1.5 詳細任務

### Phase 1.5.1: SRA指令核心實現 (紅燈階段)

#### A. 創建SraInstruction類別
- **檔案位置：** `src/Instruction.h` 和 `src/Instruction.cpp`
- **功能要求：** 算術右位移 (保持符號位)
- **格式：** `sra $rd, $rt, shamt`
- **邏輯：** `rd = rt >>> shamt` (符號位擴展)

#### B. 解碼器支援
- **檔案：** `src/InstructionDecoder.cpp`
- **Function Code：** 0x03
- **實現：** 加入 case 0x03 支援

#### C. 組譯器支援
- **檔案：** `src/Assembler.cpp`
- **語法：** "sra $rd, $rt, shamt"
- **實現：** 解析 SRA 語法並生成機器碼

### Phase 1.5.2: SRA指令BDD測試實現

#### 測試檔案：`tests/test_logical_sra_bdd_minimal.cpp`
**場景1：** 正數算術右位移
- **輸入：** 0x7FFFFFFF >>> 8
- **預期：** 0x007FFFFF (零擴展)

**場景2：** 負數算術右位移
- **輸入：** 0x80000000 >>> 4  
- **預期：** 0xF8000000 (符號位擴展)

### Phase 1.5.3: SRA指令Integration測試實現

#### 測試檔案：`tests/test_sra_instruction.cpp`
**測試1：** Decoder Integration
- 驗證 InstructionDecoder 正確解碼 function code 0x03

**測試2：** Assembler Integration
- 驗證 Assembler 正確解析 "sra" 語法

## 🔄 嚴格BDD開發流程

### Phase A: 撰寫階段 (紅燈)
1. 創建失敗的BDD測試
2. 創建失敗的Integration測試
3. 確認測試失敗狀態

### Phase B: 實作階段 (綠燈)
1. 實現SraInstruction類別
2. 加入解碼器支援
3. 加入組譯器支援
4. 確認測試通過

### Phase C: 重構階段
1. 代碼清理和優化
2. 全回歸測試驗證
3. 檔案結構檢查

## ✅ 成功標準

- [ ] 創建SraInstruction類別並實現算術右位移邏輯
- [ ] InstructionDecoder支援function code 0x03
- [ ] Assembler支援"sra $rd, $rt, shamt"語法
- [ ] 創建SRA BDD測試檔案 (2個場景)
- [ ] 創建SRA Integration測試檔案 (2個測試)
- [ ] 所有現有測試繼續通過 (178→182)
- [ ] 零編譯警告或錯誤
- [ ] 位移指令群組100%完成

## 🎉 預期完成成果

**位移指令群組完成狀態：**
- ✅ SLL (左位移): BDD測試 + Integration測試 = 完全完成
- ✅ SRL (右位移邏輯): BDD測試 + Integration測試 = 完全完成  
- ⏳ SRA (右位移算術): 即將完成

**測試增長：** 178 → 182 (+4個測試)
**完成度提升：** 19 → 20 指令 (42%)
**群組完成：** 位移指令群組100%達成

## 🗺️ 下一階段準備

**Phase 2 目標：** 立即值邏輯指令群組 (ANDI, ORI, XORI)
**預期測試：** 182 → 200 (+18個測試)
**完成度：** 42% → 49%

---
**開發指導：** 遵循嚴格BDD循環，小步前進，測試驅動 🎯
