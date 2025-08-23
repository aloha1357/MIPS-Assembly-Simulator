# 📋 Phase 7 開發指南 - 記憶體指令群組 (LB, LH, SB, SH, LBU, LHU)

## 🎯 Phase 7 開發概要

**當前狀態:** Phase 6跳躍指令群組100%完成 (282個測試)  
**目標:** 實現6個記憶體位元組/半字指令  
**預期成果:** 282 → 318個測試 (+36個測試)  
**完成度提升:** 72% → 85% (34 → 40指令)  
**難度級別:** 🔴 高 (記憶體操作複雜性)  
**預估時間:** 8-10小時

## 🏗️ 記憶體指令群組概述

### 📊 目標指令清單

#### **載入指令 (Load Instructions)**
1. **LB** (Load Byte) - I-type 指令
   - **Opcode:** 0x20
   - **語法:** `lb $rt, offset($rs)`
   - **功能:** rt = sign_extend(memory[rs + offset][7:0])
   
2. **LH** (Load Halfword) - I-type 指令
   - **Opcode:** 0x21  
   - **語法:** `lh $rt, offset($rs)`
   - **功能:** rt = sign_extend(memory[rs + offset][15:0])

3. **LBU** (Load Byte Unsigned) - I-type 指令
   - **Opcode:** 0x24
   - **語法:** `lbu $rt, offset($rs)`
   - **功能:** rt = zero_extend(memory[rs + offset][7:0])

4. **LHU** (Load Halfword Unsigned) - I-type 指令
   - **Opcode:** 0x25
   - **語法:** `lhu $rt, offset($rs)`
   - **功能:** rt = zero_extend(memory[rs + offset][15:0])

#### **儲存指令 (Store Instructions)**
5. **SB** (Store Byte) - I-type 指令
   - **Opcode:** 0x28
   - **語法:** `sb $rt, offset($rs)`
   - **功能:** memory[rs + offset][7:0] = rt[7:0]

6. **SH** (Store Halfword) - I-type 指令
   - **Opcode:** 0x29
   - **語法:** `sh $rt, offset($rs)`
   - **功能:** memory[rs + offset][15:0] = rt[15:0]

## 🔍 技術要點與設計原則

### 🎯 記憶體指令核心特性

#### **位元組/半字操作**
- **位元組 (8位元):** 最小記憶體單位，需要正確的符號/零擴展
- **半字 (16位元):** 2個位元組，需要對齊檢查
- **符號擴展:** LB/LH需要保持符號位 (MSB → 31:8 或 31:16)
- **零擴展:** LBU/LHU將高位清零

#### **地址計算與對齊**
- **地址計算:** effective_address = base(rs) + sign_extend(offset)
- **對齊要求:** 
  - LB/LBU/SB: 無對齊要求 (可訪問任意位元組地址)
  - LH/LHU/SH: 半字對齊 (地址必須是2的倍數)
- **邊界檢查:** 需要檢查記憶體邊界和對齊異常

#### **記憶體系統整合**
- **Little-Endian:** MIPS使用小端位元組序
- **記憶體映射:** 需要正確映射虛擬地址到物理記憶體
- **快取一致性:** 與現有LW/SW指令保持一致的記憶體模型

### 🧩 實現策略模式

#### **LB指令實現範例 (有符號位元組載入)**
```cpp
void LBInstruction::execute(Cpu& cpu) {
    // 1. 計算有效地址
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    int32_t signedOffset = static_cast<int32_t>(static_cast<int16_t>(m_offset));
    uint32_t effectiveAddress = baseAddress + signedOffset;
    
    // 2. 載入位元組 (8位元)
    uint8_t byteValue = cpu.getMemory().readByte(effectiveAddress);
    
    // 3. 符號擴展到32位元
    int32_t signExtended = static_cast<int32_t>(static_cast<int8_t>(byteValue));
    
    // 4. 寫入目標暫存器
    cpu.getRegisterFile().write(m_rt, static_cast<uint32_t>(signExtended));
    
    // 5. 更新PC
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

#### **LBU指令實現範例 (無符號位元組載入)**
```cpp
void LBUInstruction::execute(Cpu& cpu) {
    // 1-2. 地址計算和載入 (同LB)
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    int32_t signedOffset = static_cast<int32_t>(static_cast<int16_t>(m_offset));
    uint32_t effectiveAddress = baseAddress + signedOffset;
    uint8_t byteValue = cpu.getMemory().readByte(effectiveAddress);
    
    // 3. 零擴展到32位元 (關鍵差異)
    uint32_t zeroExtended = static_cast<uint32_t>(byteValue);
    
    // 4-5. 寫入暫存器和更新PC (同LB)
    cpu.getRegisterFile().write(m_rt, zeroExtended);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

#### **SB指令實現範例 (位元組儲存)**
```cpp
void SBInstruction::execute(Cpu& cpu) {
    // 1. 計算有效地址
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    int32_t signedOffset = static_cast<int32_t>(static_cast<int16_t>(m_offset));
    uint32_t effectiveAddress = baseAddress + signedOffset;
    
    // 2. 讀取來源暫存器的低8位元
    uint32_t sourceValue = cpu.getRegisterFile().read(m_rt);
    uint8_t byteToStore = static_cast<uint8_t>(sourceValue & 0xFF);
    
    // 3. 儲存位元組到記憶體
    cpu.getMemory().writeByte(effectiveAddress, byteToStore);
    
    // 4. 更新PC
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

## 📋 Phase 7 BDD實現週期規劃

### 🎯 開發優先順序策略
**推薦實現順序:** LB → SB → LBU → LH → SH → LHU  
**理由:** 從簡單位元組操作開始，逐步增加複雜度

### 🔴 Phase 7.1: LB指令 Red-Light Phase
**目標檔案:** `tests/test_lb_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 基本位元組載入測試 (正值)
DISABLED_TEST_F(LBInstructionBDD, BasicByteLoad_PositiveValue)

// 情境2: 符號擴展測試 (負值位元組)
DISABLED_TEST_F(LBInstructionBDD, SignExtension_NegativeByteValue)

// 情境3: 零偏移載入測試
DISABLED_TEST_F(LBInstructionBDD, ZeroOffset_DirectBaseAddress)

// 情境4: 負偏移載入測試
DISABLED_TEST_F(LBInstructionBDD, NegativeOffset_CalculateEffectiveAddress)
```

### 🟢 Phase 7.2: LB指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增case 0x20路由
2. **Assembler.cpp** - 新增"lb"語法解析
3. **指令類別** - 創建LBInstruction類別
4. **記憶體介面** - 確認Memory類別支援readByte()方法

### 🔄 Phase 7.3: LB指令 Integration Testing
**目標檔案:** `tests/test_lb_instruction_integration.cpp`

**完整整合測試 (約6個測試):**
- 解碼器機器碼解析測試
- 組譯器語法解析測試  
- 記憶體系統整合測試
- 地址計算驗證測試
- 符號擴展正確性測試
- 錯誤處理邊界測試

### 🔴 Phase 7.4-7.12: 其他指令循環
每個指令都遵循相同的BDD循環：
- **SB → LBU → LH → SH → LHU**
- 每個指令: Red-Light → Green-Light → Integration
- 預計每個指令增加6個測試 (4 BDD + 2 Integration)

## 🎯 預期開發成果

### 📊 量化目標
- **新增測試數:** +36個測試 (6指令 × 6測試/指令)
- **測試總數:** 282 → 318個測試
- **指令完成:** 6個I-type記憶體指令
- **完成度提升:** 72% → 85% (34 → 40指令)
- **估計開發時間:** 8-10小時

### 📈 質化目標
- **記憶體操作驗證:** 完整測試位元組/半字級記憶體存取
- **符號擴展邏輯:** 驗證LB/LH的正確符號擴展
- **零擴展邏輯:** 驗證LBU/LHU的正確零擴展
- **地址計算準確性:** 驗證base + offset計算
- **對齊處理:** 驗證半字指令的對齊要求
- **記憶體一致性:** 與現有LW/SW指令的一致性

## 🔧 關鍵實現差異

### 📊 指令特性比較

| 指令 | 大小 | 符號處理 | 對齊要求 | 主要用途 |
|------|------|----------|----------|----------|
| **LB** | 8位元 | 符號擴展 | 無 | 有符號字元載入 |
| **LBU** | 8位元 | 零擴展 | 無 | 無符號字元載入 |
| **LH** | 16位元 | 符號擴展 | 2位元對齊 | 有符號短整數載入 |
| **LHU** | 16位元 | 零擴展 | 2位元對齊 | 無符號短整數載入 |
| **SB** | 8位元 | 截斷低8位元 | 無 | 位元組儲存 |
| **SH** | 16位元 | 截斷低16位元 | 2位元對齊 | 半字儲存 |

### 🎯 與現有指令的差異

#### **LB/LBU vs LW**
- **LW:** 32位元載入，4位元對齊，無符號問題
- **LB:** 8位元載入，符號擴展，無對齊要求
- **LBU:** 8位元載入，零擴展，無對齊要求

#### **SB/SH vs SW**
- **SW:** 32位元儲存，4位元對齊，直接儲存
- **SB:** 8位元儲存，截斷到低8位元，無對齊要求
- **SH:** 16位元儲存，截斷到低16位元，2位元對齊

## 🧪 Phase 7 開發檢核清單

### 🔴 Red-Light階段檢核
- [ ] LB BDD測試檔案建立 (4個DISABLED測試)
- [ ] SB BDD測試檔案建立 (4個DISABLED測試)
- [ ] LBU BDD測試檔案建立 (4個DISABLED測試)
- [ ] LH BDD測試檔案建立 (4個DISABLED測試)
- [ ] SH BDD測試檔案建立 (4個DISABLED測試)
- [ ] LHU BDD測試檔案建立 (4個DISABLED測試)
- [ ] CMakeLists.txt更新包含新測試檔案
- [ ] 編譯確認 - 應該有24個失敗測試

### 🟢 Green-Light階段檢核
- [ ] InstructionDecoder.cpp新增6個opcode路由
- [ ] Assembler.cpp新增6個語法解析器
- [ ] 6個指令類別實現完成
- [ ] Memory類別支援readByte/writeByte/readHalfword/writeHalfword
- [ ] BDD測試全部通過 (移除DISABLED前綴)
- [ ] 零編譯警告確認

### 🔄 Integration階段檢核
- [ ] 6個Integration測試檔案建立 (每個2個測試)
- [ ] 記憶體系統整合驗證
- [ ] 地址計算一致性測試
- [ ] 完整回歸測試 (所有282+36個測試通過)
- [ ] Phase 7完成驗收

## ⚠️ 風險管控與除錯策略

### 🚨 已知風險點
1. **記憶體系統複雜性:** 位元組級操作可能影響現有記憶體架構
2. **符號擴展錯誤:** 有符號/無符號轉換容易出錯
3. **對齊檢查:** 半字指令的對齊驗證實現
4. **Little-Endian處理:** 位元組順序可能造成混淆
5. **記憶體邊界:** 越界存取的錯誤處理

### 🛡️ 風險對策
1. **段階實現:** 先實現基本功能，再加入對齊和錯誤檢查
2. **單元測試重點:** 特別測試符號擴展和零擴展邏輯
3. **參考現有實現:** 使用LW/SW作為記憶體操作模板
4. **邊界測試:** 加強邊界條件和錯誤情況測試
5. **程式碼審查:** 重點檢查位元操作和轉換邏輯

### 🔍 除錯策略
1. **分層除錯:** 先測試地址計算，再測試記憶體存取，最後測試擴展邏輯
2. **十六進制輸出:** 使用十六進制輸出檢查位元操作正確性
3. **記憶體檢視:** 加入記憶體內容檢視功能輔助除錯
4. **逐位元驗證:** 使用位元遮罩驗證擴展和截斷邏輯

## 🎯 Phase 7 成功標準

### 📊 技術指標
- **282 → 318個測試:** 100%通過率
- **零編譯錯誤:** 完整編譯成功
- **BDD循環完整:** 完成18次完整red-light → green-light循環
- **回歸穩定性:** 原有功能保持完整

### ✅ 驗收標準  
- **LB功能:** 正確的有符號位元組載入和符號擴展
- **LBU功能:** 正確的無符號位元組載入和零擴展
- **LH/LHU功能:** 正確的半字載入和擴展處理
- **SB/SH功能:** 正確的位元組/半字儲存和截斷
- **記憶體一致性:** 與LW/SW完全相容的記憶體模型
- **整合品質:** Assembler + Decoder + Memory完整支援

## 🚀 Phase 8 預告

Phase 7完成後，下一階段將進入：
- **Phase 8:** 乘除法指令群組 (MULT, MULTU, DIV, DIVU)
- **目標測試數:** 318 → 342個測試
- **完成度目標:** 85% → 93%
- **關鍵特性:** HI/LO暫存器系統實現

---

## 🎯 Phase 7 立即行動計劃

### 🚀 Phase 7.1 LB指令啟動
**立即任務:** 建立 `tests/test_lb_instruction_bdd_minimal.cpp`  
**參考模板:** `tests/test_jalr_instruction_bdd_minimal.cpp`  
**預期成果:** 4個DISABLED_測試，編譯成功但測試失敗  
**預估時間:** 45分鐘  
**成功指標:** 編譯無錯誤，4個紅燈測試

### 📋 第一步檢核清單
1. **創建LB BDD測試檔案** - 包含4個基本測試情境
2. **更新CMakeLists.txt** - 加入新測試檔案
3. **編譯驗證** - 確認新測試失敗但編譯成功
4. **實現LBInstruction類別** - 在Instruction.h/.cpp
5. **加入解碼器支援** - 在InstructionDecoder.cpp
6. **加入組譯器支援** - 在Assembler.cpp
7. **執行測試確認綠燈** - 移除DISABLED前綴
8. **創建Integration測試** - 完成完整BDD循環

**下一階段完成度目標:** 72% → 85% (6個記憶體指令實現)
**總體目標:** 為乘除法指令群組(Phase 8)奠定穩固基礎

---

**🎉 祝賀 Phase 6 完成!** 跳躍指令群組(JR + JALR + JAL)全面實現，282個測試100%通過，為記憶體指令群組開發奠定了堅實基礎! 🚀
