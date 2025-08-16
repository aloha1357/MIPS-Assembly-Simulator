# 📋 Phase 6 開發指南 - 跳躍指令群組 (JR, JALR, JAL)

## 🎯 Phase 6 概述

**階段目標:** 實現MIPS跳躍指令群組，完成無條件跳躍和子程序調用功能
**預期成果:** 256 → 274個測試，66% → 72%完成度
**估計時間:** 6-8小時
**複雜度等級:** 🟡 中等

## 📋 目標指令清單

### 1. JR (Jump Register) - R-type 指令
- **Function Code:** 0x08
- **格式:** `jr $rs`
- **功能:** PC = $rs (跳到暫存器指定地址)
- **用途:** 函數返回、間接跳躍

### 2. JALR (Jump And Link Register) - R-type 指令
- **Function Code:** 0x09
- **格式:** `jalr $rd, $rs` 或 `jalr $rs` (默認$rd=$ra)
- **功能:** $rd = PC+4; PC = $rs (保存返回地址並跳躍)
- **用途:** 間接函數調用

### 3. JAL (Jump And Link) - J-type 指令
- **Opcode:** 0x03
- **格式:** `jal target`
- **功能:** $ra = PC+4; PC = target (保存返回地址並跳躍)
- **用途:** 直接函數調用

## 🔄 Phase 6 開發順序 (嚴格BDD循環)

### 6.1 JR指令實現 (2-3小時)

#### 🔴 Red-Light Phase (45分鐘)
1. **創建BDD測試檔案**
```bash
tests/test_jr_instruction_bdd_minimal.cpp
```

2. **實現4個DISABLED測試場景**
```cpp
DISABLED_TEST_F(JRInstructionBDD, BasicRegisterJump)
DISABLED_TEST_F(JRInstructionBDD, DifferentRegisterJump) 
DISABLED_TEST_F(JRInstructionBDD, ZeroAddressJump)
DISABLED_TEST_F(JRInstructionBDD, HighAddressJump)
```

3. **更新CMakeLists.txt並確認編譯失敗**

#### 🟢 Green-Light Phase (60分鐘)
1. **創建JRInstruction類別** (src/Instruction.h & src/Instruction.cpp)
2. **添加解碼器支援** (src/InstructionDecoder.cpp - function code 0x08)
3. **添加組譯器支援** (src/Assembler.cpp - "jr"語法)
4. **移除DISABLED前綴，確認測試通過**

#### 🔄 Integration Phase (30分鐘)
1. **創建Integration測試** (tests/test_jr_instruction_integration.cpp)
2. **完整回歸測試確認**

### 6.2 JAL指令實現 (2-3小時)

#### 🔴 Red-Light Phase (45分鐘)
1. **創建BDD測試檔案** (tests/test_jal_instruction_bdd_minimal.cpp)
2. **實現4個DISABLED測試場景**
3. **確認編譯失敗**

#### 🟢 Green-Light Phase (60分鐘) 
1. **創建JALInstruction類別** 
2. **添加解碼器支援** (opcode 0x03)
3. **添加組譯器支援** ("jal"語法)
4. **確認測試通過**

#### 🔄 Integration Phase (30分鐘)
1. **創建Integration測試**
2. **回歸測試確認**

### 6.3 JALR指令實現 (2-3小時)

#### 🔴 Red-Light Phase (45分鐘)
1. **創建BDD測試檔案** (tests/test_jalr_instruction_bdd_minimal.cpp)
2. **實現4個DISABLED測試場景**
3. **確認編譯失敗**

#### 🟢 Green-Light Phase (75分鐘)
1. **創建JALRInstruction類別**
2. **添加解碼器支援** (function code 0x09)
3. **添加組譯器支援** ("jalr"語法，處理可選參數)
4. **確認測試通過**

#### 🔄 Integration Phase (30分鐘)
1. **創建Integration測試**
2. **最終回歸測試確認**

## 🛠️ 技術實現指南

### JR指令實現範例
```cpp
class JRInstruction : public RTypeInstruction {
public:
    JRInstruction(uint32_t rs) : RTypeInstruction(0, 0, rs, 0, 0x08) {}
    
    void execute(Cpu& cpu) override {
        uint32_t targetAddress = cpu.getRegisterFile().read(m_rs);
        cpu.setProgramCounter(targetAddress / 4); // 地址除以4
    }
    
    std::string getName() const override { return "jr"; }
};
```

### JAL指令實現範例
```cpp
class JALInstruction : public JTypeInstruction {
public:
    JALInstruction(uint32_t target) : JTypeInstruction(0x03, target) {}
    
    void execute(Cpu& cpu) override {
        uint32_t returnAddress = (cpu.getProgramCounter() + 1) * 4;
        cpu.getRegisterFile().write(31, returnAddress); // $ra = $31
        
        uint32_t targetAddress = m_target * 4;
        cpu.setProgramCounter(targetAddress / 4);
    }
    
    std::string getName() const override { return "jal"; }
};
```

### JALR指令實現範例
```cpp
class JALRInstruction : public RTypeInstruction {
public:
    JALRInstruction(uint32_t rd, uint32_t rs) 
        : RTypeInstruction(rd, 0, rs, 0, 0x09) {}
    
    void execute(Cpu& cpu) override {
        uint32_t targetAddress = cpu.getRegisterFile().read(m_rs);
        uint32_t returnAddress = (cpu.getProgramCounter() + 1) * 4;
        
        cpu.getRegisterFile().write(m_rd, returnAddress);
        cpu.setProgramCounter(targetAddress / 4);
    }
    
    std::string getName() const override { return "jalr"; }
};
```

## 🧪 BDD測試模板

### JR指令BDD測試範例
```cpp
TEST_F(JRInstructionBDD, BasicRegisterJump) {
    // Given: 暫存器包含目標地址
    given_register_contains("$t0", 0x00001000);
    given_register_contains("$t1", 0x12345678);
    
    // When: 執行JR指令
    when_program_executed_for_cycles("jr $t0", 1);
    
    // Then: PC應該跳躍到目標地址
    then_program_counter_should_equal(0x00001000 / 4);
    // And: 其他暫存器不受影響
    then_register_should_equal("$t1", 0x12345678);
}
```

### JAL指令BDD測試範例
```cpp
TEST_F(JALInstructionBDD, BasicJumpAndLink) {
    // Given: PC在特定位置
    given_program_counter_at(0x00000100);
    
    // When: 執行JAL指令
    when_program_executed_for_cycles("jal 0x2000", 1);
    
    // Then: PC應該跳躍到目標地址
    then_program_counter_should_equal(0x2000);
    // And: $ra應該包含返回地址
    then_register_should_equal("$ra", 0x00000104);
}
```

### JALR指令BDD測試範例
```cpp
TEST_F(JALRInstructionBDD, BasicJumpAndLinkRegister) {
    // Given: 暫存器包含目標地址，PC在特定位置
    given_register_contains("$t0", 0x00003000);
    given_program_counter_at(0x00000200);
    
    // When: 執行JALR指令
    when_program_executed_for_cycles("jalr $ra, $t0", 1);
    
    // Then: PC應該跳躍到目標地址
    then_program_counter_should_equal(0x00003000 / 4);
    // And: $ra應該包含返回地址
    then_register_should_equal("$ra", 0x00000204);
}
```

## ⚠️ 常見陷阱與注意事項

### 地址計算陷阱
1. **地址對齊:** MIPS要求跳躍地址4位元對齊
2. **地址轉換:** PC值通常需要乘以或除以4
3. **返回地址:** 是PC+4還是(PC+1)*4的選擇

### 暫存器處理陷阱
1. **JALR可選參數:** 語法`jalr $rs`等同於`jalr $ra, $rs`
2. **$ra暫存器:** 編號為31的特殊暫存器
3. **源和目標暫存器:** 避免同一暫存器既作源又作目標

### 組譯器語法陷阱
1. **立即值格式:** JAL使用26位立即值
2. **暫存器順序:** JALR的參數順序是$rd, $rs
3. **可選參數處理:** JALR的$rd參數可以省略

## ✅ Phase 6 成功檢核清單

### Red-Light階段檢核
- [ ] 3個BDD測試檔案創建完成 (12個DISABLED測試)
- [ ] CMakeLists.txt更新包含新測試檔案
- [ ] 編譯成功但12個測試失敗
- [ ] 確認測試名稱和結構正確

### Green-Light階段檢核
- [ ] 3個指令類別實現完成
- [ ] InstructionDecoder.cpp新增3個路由
- [ ] Assembler.cpp新增3個語法解析器
- [ ] 移除DISABLED前綴後所有12個BDD測試通過
- [ ] 零編譯警告確認

### Integration階段檢核
- [ ] 3個Integration測試檔案創建 (預計6個測試)
- [ ] 解碼器Integration測試通過
- [ ] 組譯器Integration測試通過
- [ ] 完整回歸測試: 256 → 274個測試100%通過
- [ ] Phase 6完成驗收

### 最終驗收標準
- [ ] **JR功能:** 正確的暫存器跳躍
- [ ] **JAL功能:** 正確的立即值跳躍並保存返回地址
- [ ] **JALR功能:** 正確的暫存器跳躍並保存返回地址
- [ ] **地址計算:** 所有跳躍地址計算正確
- [ ] **返回地址:** JAL/JALR正確保存返回地址
- [ ] **整合品質:** Assembler + Decoder完整支援

## 🚀 Phase 7 預告

Phase 6完成後，下一階段開發重點：
- **Phase 7:** 記憶體指令群組 (LB, LH, SB, SH, LBU, LHU)
- **目標:** 274 → 310個測試，72% → 85%完成度
- **複雜度:** 🔴 高難度 (涉及記憶體位元組操作)
- **估計時間:** 8-10小時

## 📚 參考資源

- **BDD模板參考:** `tests/test_sllv_instruction_bdd_minimal.cpp`
- **R-type實現參考:** `src/Instruction.cpp` 中的變數位移指令
- **J-type實現參考:** `src/Instruction.cpp` 中的J指令
- **組譯器語法參考:** `src/Assembler.cpp` 中的現有指令解析
- **測試工具使用:** `.\tests\unit_tests.exe --gtest_filter="*JR*"`

---

**🎯 關鍵成功因素:**
1. **嚴格遵循BDD循環** - Red-Light → Green-Light → Integration
2. **小步前進** - 一次只實現一個指令
3. **完整回歸測試** - 每次變更後確認256個測試持續通過
4. **地址計算精確** - 特別注意MIPS地址對齊和轉換
5. **返回地址正確** - JAL/JALR的返回地址保存邏輯

**預期完成後狀態:**
- **指令完成度:** 31 → 34 指令 (66% → 72%)
- **測試數量:** 256 → 274 個測試
- **跳躍指令群組:** 3/3 = 100%完成
- **為Phase 7記憶體指令群組奠定基礎**
