# 🚀 Phase 5 開發指南 - 變數位移指令群組實現

## 📋 Phase 5 總覽

### 🎯 開發目標
完成MIPS變數位移指令群組的完整實現，包含三個核心指令：SLLV、SRLV、SRAV

### 📊 量化指標
- **起始狀態:** 244個測試，28/47指令完成 (60%)
- **目標狀態:** 262個測試，31/47指令完成 (66%)  
- **新增測試:** +18個測試 (每個指令6個測試)
- **預估時間:** 5-7小時
- **難度等級:** 🟡 中等

---

## 🎯 目標指令清單

### 1. SLLV (Shift Left Logical Variable)
- **類型:** R-type指令
- **Function Code:** 0x04
- **語法:** `sllv $rd, $rt, $rs`
- **功能:** `$rd = $rt << ($rs & 0x1F)`
- **特性:** 變數左位移，位移量由$rs暫存器提供

### 2. SRLV (Shift Right Logical Variable)  
- **類型:** R-type指令
- **Function Code:** 0x06
- **語法:** `srlv $rd, $rt, $rs`
- **功能:** `$rd = $rt >> ($rs & 0x1F)`
- **特性:** 變數邏輯右位移，零擴展

### 3. SRAV (Shift Right Arithmetic Variable)
- **類型:** R-type指令
- **Function Code:** 0x07
- **語法:** `srav $rd, $rt, $rs`
- **功能:** `$rd = $rt >>> ($rs & 0x1F)`
- **特性:** 變數算術右位移，符號擴展

---

## 🔧 技術實現要點

### 關鍵差異：變數位移 vs 立即值位移

| 特性 | 立即值位移 (SLL/SRL/SRA) | 變數位移 (SLLV/SRLV/SRAV) |
|------|-------------------------|---------------------------|
| 位移量來源 | 5位元shamt欄位 | $rs暫存器 |
| 位移量範圍 | 0-31 (固定) | 0-31 (需要& 0x1F) |
| Function Code | 0x00/0x02/0x03 | 0x04/0x06/0x07 |
| 暫存器用法 | $rd, $rt, shamt | $rd, $rt, $rs |

### 實現關鍵點

#### 1. 位移量處理
```cpp
// 關鍵：只使用$rs的低5位
uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
uint32_t shiftAmount = rsValue & 0x1F; // 確保位移量在0-31範圍
```

#### 2. 暫存器角色
```cpp
// 注意：$rs提供位移量，$rt是被位移的值
uint32_t rtValue = cpu.getRegisterFile().read(m_rt);  // 被位移值
uint32_t rsValue = cpu.getRegisterFile().read(m_rs);  // 位移量
```

#### 3. 符號處理 (SRAV專用)
```cpp
// SRAV需要符號擴展
int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));
int32_t result = rtValue >> shiftAmount; // 算術右位移
```

---

## 📋 BDD開發循環

### Phase 5.1: SLLV指令開發

#### 🔴 Red-Light Phase (30分鐘)
**檔案:** `tests/test_sllv_instruction_bdd_minimal.cpp`

```cpp
class SLLVInstructionBDDTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        // 初始化設定
    }
    
    std::unique_ptr<mips::Cpu> cpu;
};

DISABLED_TEST_F(SLLVInstructionBDDTest, BasicVariableLeftShift) {
    // Given: $rt = 0x00000001, $rs = 4
    // When: sllv $rd, $rt, $rs
    // Then: $rd = 0x00000010
}

DISABLED_TEST_F(SLLVInstructionBDDTest, ShiftAmountTruncation) {
    // Given: $rt = 0x00000001, $rs = 36 (0x24)
    // When: sllv $rd, $rt, $rs  
    // Then: $rd = 0x00000010 (36 & 0x1F = 4)
}

DISABLED_TEST_F(SLLVInstructionBDDTest, ZeroShiftIdentity) {
    // Given: $rt = 0x12345678, $rs = 0
    // When: sllv $rd, $rt, $rs
    // Then: $rd = 0x12345678
}

DISABLED_TEST_F(SLLVInstructionBDDTest, MaximumShift) {
    // Given: $rt = 0x00000001, $rs = 31
    // When: sllv $rd, $rt, $rs
    // Then: $rd = 0x80000000
}
```

#### 🟢 Green-Light Phase (45分鐘)

**1. 實現SLLVInstruction類別**
```cpp
// src/Instruction.h
class SLLVInstruction : public RTypeInstruction {
public:
    SLLVInstruction(uint32_t rd, uint32_t rt, uint32_t rs);
    void execute(Cpu& cpu) override;
    std::string getName() const override { return "sllv"; }
};

// src/Instruction.cpp  
void SLLVInstruction::execute(Cpu& cpu) {
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t shiftAmount = rsValue & 0x1F;
    uint32_t result = rtValue << shiftAmount;
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

**2. 更新InstructionDecoder**
```cpp
// src/InstructionDecoder.cpp
case 0x04: // SLLV instruction
    return std::make_unique<SLLVInstruction>(rd, rt, rs);
```

**3. 更新Assembler**
```cpp
// src/Assembler.cpp
else if (opcode == "sllv" && tokens.size() >= 4) {
    uint32_t rd = parseRegister(tokens[1]);
    uint32_t rt = parseRegister(tokens[2]);
    uint32_t rs = parseRegister(tokens[3]);
    return std::make_unique<SLLVInstruction>(rd, rt, rs);
}
```

**4. 移除DISABLED_前綴，運行測試**

#### 🔄 Integration Phase (30分鐘)
建立Integration測試，確保解碼器和組譯器正常工作

### Phase 5.2: SRLV指令開發
重複相同的BDD循環，實現SRLV指令

### Phase 5.3: SRAV指令開發  
重複相同的BDD循環，實現SRAV指令（注意符號擴展）

---

## ✅ 開發檢核清單

### Red-Light階段檢核
- [ ] 建立test_sllv_instruction_bdd_minimal.cpp (4個DISABLED測試)
- [ ] 建立test_srlv_instruction_bdd_minimal.cpp (4個DISABLED測試)
- [ ] 建立test_srav_instruction_bdd_minimal.cpp (4個DISABLED測試)
- [ ] 更新tests/CMakeLists.txt
- [ ] 編譯確認：ninja unit_tests (應該有12個失敗測試)

### Green-Light階段檢核
- [ ] 實現三個指令類別 (SLLVInstruction, SRLVInstruction, SRAVInstruction)
- [ ] 更新InstructionDecoder.cpp (三個function code)
- [ ] 更新Assembler.cpp (三個語法解析器)
- [ ] 移除所有DISABLED_前綴
- [ ] 確認12個BDD測試全部通過

### Integration階段檢核
- [ ] 建立Integration測試檔案 (3檔案 × 2測試 = 6個測試)
- [ ] 完整回歸測試：244 → 262個測試
- [ ] 零編譯警告確認
- [ ] Phase 5完成驗收

---

## 🎯 成功標準

### 技術指標
- **測試通過率:** 262/262 = 100%
- **編譯狀態:** 零警告零錯誤
- **功能完整性:** 三個變數位移指令完全實現
- **回歸穩定性:** 原有244個測試保持通過

### 驗收標準
- **SLLV功能:** 正確的變數左位移，位移量從$rs獲取
- **SRLV功能:** 正確的變數邏輯右位移，零擴展
- **SRAV功能:** 正確的變數算術右位移，符號擴展
- **位移量處理:** 正確的低5位截取 (& 0x1F)

---

## 🚀 開發優先順序

### 建議實現順序
1. **SLLV (優先)** - 最簡單，邏輯直觀
2. **SRLV (中等)** - 邏輯右位移，零擴展
3. **SRAV (最後)** - 算術右位移，需要符號處理

### 時間分配
- **SLLV:** 2小時 (Red 30min + Green 45min + Integration 30min + Debug 15min)
- **SRLV:** 1.5小時 (可複用SLLV架構)
- **SRAV:** 2小時 (需要額外符號處理邏輯)
- **整合測試:** 1小時
- **緩衝時間:** 0.5小時

---

## 🎯 風險管控

### 已知風險點
1. **暫存器角色混淆:** $rs(位移量) vs $rt(被位移值)
2. **位移量範圍:** 超過31的位移量處理
3. **符號擴展:** SRAV的正確符號位處理

### 風險對策
1. **充分測試:** 包含邊界值和異常情況測試
2. **參考實現:** 重用現有SLL/SRL/SRA的邏輯結構
3. **逐步驗證:** 每個指令完成後立即測試

---

## 🎉 Phase 5完成後

### 預期成果
- **指令完成度:** 28/47 → 31/47 (60% → 66%)
- **測試覆蓋:** 244 → 262個測試
- **架構成熟度:** 變數位移指令模式建立

### Phase 6預告
- **下一目標:** 跳躍指令群組 (JR, JALR, JAL)
- **預期增量:** +18個測試 (262 → 280)
- **完成度目標:** 66% → 72%

---

## 🔧 開發工具與命令

### 常用測試命令
```bash
# 編譯
cd c:\Users\aloha\Documents\GitHub\MIPS-Assembly-Simulator\build
ninja unit_tests

# 測試特定指令
.\tests\unit_tests.exe --gtest_filter="*SLLV*"

# 完整回歸測試
.\tests\unit_tests.exe --gtest_brief

# 檢查測試總數
.\tests\unit_tests.exe 2>&1 | Select-String "PASSED"
```

### 參考檔案
- **BDD測試模板:** `tests/test_addiu_instruction_bdd_minimal.cpp`
- **R-type指令參考:** `src/Instruction.cpp` 中的 SLLInstruction
- **解碼器參考:** `src/InstructionDecoder.cpp` 中的 R-type 處理
- **組譯器參考:** `src/Assembler.cpp` 中的 R-type 語法解析

---

**🎯 Phase 5 立即行動：開始SLLV指令BDD Red-Light Phase**
**目標檔案:** `tests/test_sllv_instruction_bdd_minimal.cpp`
**成功指標:** 4個DISABLED測試建立，編譯成功但測試失敗
