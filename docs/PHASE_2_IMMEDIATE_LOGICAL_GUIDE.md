# 🚀 Phase 2 開發指南：立即值邏輯指令群組實現

## 📋 開發目標

**Phase 2 目標：** 立即值邏輯指令群組完成  
**指令清單：** ANDI, ORI, XORI (3個I-type指令)  
**當前狀態：** 182個測試  
**目標狀態：** 200個測試 (+18個測試)  
**完成里程碑：** 立即值邏輯指令群組100%完成

## 🎯 Phase 2 詳細任務列表

### Phase 2.1: ANDI指令實現 (AND Immediate)

#### A. 核心實現要求
- **功能：** 位元AND與立即值
- **格式：** `andi $rt, $rs, immediate`
- **邏輯：** `rt = rs & (zero_extend(immediate))`
- **Opcode：** 0x0C

#### B. 實現清單
1. **檢查現有AndiInstruction實現狀況**
2. **完善AndiInstruction::execute()方法** 
3. **InstructionDecoder支援** (Opcode 0x0C)
4. **Assembler語法支援** ("andi $rt, $rs, imm")
5. **BDD測試** (2個場景)
6. **Integration測試** (2個測試)

### Phase 2.2: ORI指令實現 (OR Immediate)

#### A. 核心實現要求
- **功能：** 位元OR與立即值
- **格式：** `ori $rt, $rs, immediate`
- **邏輯：** `rt = rs | (zero_extend(immediate))`
- **Opcode：** 0x0D

#### B. 實現清單
1. **檢查現有OriInstruction實現狀況**
2. **完善OriInstruction::execute()方法**
3. **InstructionDecoder支援** (Opcode 0x0D)
4. **Assembler語法支援** ("ori $rt, $rs, imm")
5. **BDD測試** (2個場景)
6. **Integration測試** (2個測試)

### Phase 2.3: XORI指令實現 (XOR Immediate)

#### A. 核心實現要求
- **功能：** 位元XOR與立即值
- **格式：** `xori $rt, $rs, immediate`
- **邏輯：** `rt = rs ^ (zero_extend(immediate))`
- **Opcode：** 0x0E

#### B. 實現清單
1. **檢查現有XoriInstruction實現狀況**
2. **完善XoriInstruction::execute()方法**
3. **InstructionDecoder支援** (Opcode 0x0E)
4. **Assembler語法支援** ("xori $rt, $rs, imm")
5. **BDD測試** (2個場景)
6. **Integration測試** (2個測試)

## 📊 Phase 2 BDD測試場景設計

### ANDI指令測試場景
**場景1：** 基本AND立即值操作
- Given: $t0 = 0xFF00FF00
- When: andi $t1, $t0, 0x00FF
- Then: $t1 = 0x00000000

**場景2：** 高位清零測試
- Given: $t0 = 0xFFFFFFFF
- When: andi $t1, $t0, 0xFF00
- Then: $t1 = 0x0000FF00

### ORI指令測試場景
**場景1：** 基本OR立即值操作
- Given: $t0 = 0x0000F000
- When: ori $t1, $t0, 0x0F00
- Then: $t1 = 0x0000FF00

**場景2：** 設置特定位元
- Given: $t0 = 0x00000000
- When: ori $t1, $t0, 0xFFFF
- Then: $t1 = 0x0000FFFF

### XORI指令測試場景
**場景1：** 基本XOR立即值操作
- Given: $t0 = 0xFF00FF00
- When: xori $t1, $t0, 0x00FF
- Then: $t1 = 0xFF00FFFF

**場景2：** 位元翻轉測試
- Given: $t0 = 0x0000FFFF
- When: xori $t1, $t0, 0xFFFF
- Then: $t1 = 0x00000000

## 🔄 Phase 2 開發流程

### 步驟1：現況分析
1. 檢查現有立即值邏輯指令實現狀況
2. 識別已完成和待實現的組件
3. 分析現有測試覆蓋度

### 步驟2：實現優先順序
1. **優先實現ANDI** (最常用)
2. **接著實現ORI** (設置位元常用)
3. **最後實現XORI** (位元翻轉)

### 步驟3：嚴格BDD循環
對每個指令執行：
- **紅燈階段：** 創建失敗的BDD和Integration測試
- **綠燈階段：** 實現功能使測試通過
- **重構階段：** 優化代碼並確保所有測試通過

## ✅ Phase 2 成功標準

### 技術要求
- [ ] 3個立即值邏輯指令類別完整實現
- [ ] InstructionDecoder支援3個Opcodes
- [ ] Assembler支援3種語法解析
- [ ] 6個BDD測試場景實現 (每指令2個)
- [ ] 6個Integration測試實現 (每指令2個)
- [ ] 所有現有測試繼續通過 (182→200)
- [ ] 零編譯警告或錯誤

### 品質要求
- [ ] 每個指令都有完整的零擴展邏輯
- [ ] 正確處理16位立即值
- [ ] BDD測試覆蓋邊界條件
- [ ] Integration測試驗證組件整合

## 🎉 Phase 2 完成後預期成果

**立即值邏輯指令群組完成狀態：**
- ✅ ANDI: BDD測試 + Integration測試 = 完全完成
- ✅ ORI: BDD測試 + Integration測試 = 完全完成  
- ✅ XORI: BDD測試 + Integration測試 = 完全完成

**測試增長：** 182 → 200 (+18個測試)
**完成度提升：** 20 → 23 指令 (49%)
**群組完成：** 立即值邏輯指令群組100%達成

## 🗺️ Phase 3 準備

**Phase 3 目標：** 分支指令群組 (BLEZ, BGTZ)
**預期測試：** 200 → 212 (+12個測試)
**完成度：** 49% → 53%

---
**開發指導：** 建立在位移指令群組成功經驗基礎上，複用BDD測試架構 🎯
