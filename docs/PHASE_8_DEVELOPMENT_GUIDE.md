# 📋 Phase 8 開發指南 - 乘除法指令群組

## 🎯 Phase 8 開發目標

**狀態:** Phase 7記憶體指令群組已100%完成 (306個測試全部通過)
**下一個目標:** Phase 8 - 乘除法指令群組實現 (MULT, MULTU, DIV, DIVU)

Phase 8 將完成MIPS乘除法指令群組的實現，這些指令需要特殊的HI/LO暫存器：

### 目標指令清單

1. **MULT** (Multiply) - R-type 指令
   - **Function Code:** 0x18
   - **語法:** `mult $rs, $rt`
   - **功能:** HI:LO = $rs × $rt (有符號64位結果)
   
2. **MULTU** (Multiply Unsigned) - R-type 指令
   - **Function Code:** 0x19
   - **語法:** `multu $rs, $rt`
   - **功能:** HI:LO = $rs × $rt (無符號64位結果)

3. **DIV** (Divide) - R-type 指令
   - **Function Code:** 0x1A
   - **語法:** `div $rs, $rt`
   - **功能:** LO = $rs ÷ $rt, HI = $rs mod $rt (有符號)

4. **DIVU** (Divide Unsigned) - R-type 指令
   - **Function Code:** 0x1B
   - **語法:** `divu $rs, $rt`
   - **功能:** LO = $rs ÷ $rt, HI = $rs mod $rt (無符號)

## 🏗️ 技術要點與設計原則

### 乘除法指令特性
- **HI/LO暫存器:** 需要擴展RegisterFile支援32+32位特殊暫存器
- **64位結果:** 乘法結果需要64位存儲空間
- **除零處理:** 除法指令需要處理除零異常
- **符號處理:** 有符號/無符號運算的正確實現

### 核心實現架構

#### RegisterFile HI/LO擴展
```cpp
// 在 src/RegisterFile.h 中加入:
class RegisterFile {
private:
    uint32_t m_registers[32];
    uint32_t m_hi;  // HI register for multiply/divide
    uint32_t m_lo;  // LO register for multiply/divide
    
public:
    uint32_t readHI() const { return m_hi; }
    uint32_t readLO() const { return m_lo; }
    void writeHI(uint32_t value) { m_hi = value; }
    void writeLO(uint32_t value) { m_lo = value; }
};
```

#### MULT實現範例
```cpp
void MULTInstruction::execute(Cpu& cpu) {
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));
    int64_t result = static_cast<int64_t>(rsValue) * static_cast<int64_t>(rtValue);
    
    uint32_t hi = static_cast<uint32_t>((result >> 32) & 0xFFFFFFFF);
    uint32_t lo = static_cast<uint32_t>(result & 0xFFFFFFFF);
    
    cpu.getRegisterFile().writeHI(hi);
    cpu.getRegisterFile().writeLO(lo);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

#### DIV實現範例
```cpp
void DIVInstruction::execute(Cpu& cpu) {
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));
    
    if (rtValue == 0) {
        // 除零處理 - 結果未定義但不產生異常
        cpu.getRegisterFile().writeLO(0);
        cpu.getRegisterFile().writeHI(0);
    } else {
        int32_t quotient = rsValue / rtValue;
        int32_t remainder = rsValue % rtValue;
        
        cpu.getRegisterFile().writeLO(static_cast<uint32_t>(quotient));
        cpu.getRegisterFile().writeHI(static_cast<uint32_t>(remainder));
    }
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

## 🔄 BDD實現週期規劃

### 🔴 Phase 8.1: RegisterFile HI/LO擴展 (優先基礎設施)

**必要修改檔案:**
1. **src/RegisterFile.h** - 加入HI/LO暫存器宣告
2. **src/RegisterFile.cpp** - 實現HI/LO讀寫方法
3. **現有測試更新** - 確保原有功能不受影響

### 🔴 Phase 8.2: MULT指令 Red-Light Phase
**目標檔案:** `tests/test_mult_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 基本有符號乘法測試
DISABLED_TEST_F(MULTInstructionBDD, BasicSignedMultiplication)

// 情境2: 64位結果測試 (HI:LO驗證)
DISABLED_TEST_F(MULTInstructionBDD, SixtyFourBitResult)

// 情境3: 負數乘法測試
DISABLED_TEST_F(MULTInstructionBDD, NegativeMultiplication)

// 情境4: 零乘法測試
DISABLED_TEST_F(MULTInstructionBDD, ZeroMultiplication)
```

### 🟢 Phase 8.3: MULT指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增function code 0x18路由
2. **Assembler.cpp** - 新增"mult"語法解析
3. **指令類別** - 創建MULTInstruction類別

### 🔴 Phase 8.4: MULTU指令 Red-Light Phase
**目標檔案:** `tests/test_multu_instruction_bdd_minimal.cpp`

**必要測試情境:**
```cpp
// 情境1: 基本無符號乘法測試
DISABLED_TEST_F(MULTUInstructionBDD, BasicUnsignedMultiplication)

// 情境2: 大數無符號乘法測試
DISABLED_TEST_F(MULTUInstructionBDD, LargeUnsignedMultiplication)

// 情境3: 最大值乘法測試
DISABLED_TEST_F(MULTUInstructionBDD, MaxValueMultiplication)

// 情境4: 零乘法測試
DISABLED_TEST_F(MULTUInstructionBDD, ZeroMultiplication)
```

### 🔴 Phase 8.5: DIV指令 Red-Light Phase
**目標檔案:** `tests/test_div_instruction_bdd_minimal.cpp`

**必要測試情境:**
```cpp
// 情境1: 基本有符號除法測試
DISABLED_TEST_F(DIVInstructionBDD, BasicSignedDivision)

// 情境2: 除零處理測試
DISABLED_TEST_F(DIVInstructionBDD, DivideByZeroHandling)

// 情境3: 負數除法測試
DISABLED_TEST_F(DIVInstructionBDD, NegativeDivision)

// 情境4: 餘數驗證測試
DISABLED_TEST_F(DIVInstructionBDD, RemainderVerification)
```

### 🔴 Phase 8.6: DIVU指令 Red-Light Phase
**目標檔案:** `tests/test_divu_instruction_bdd_minimal.cpp`

**必要測試情境:**
```cpp
// 情境1: 基本無符號除法測試
DISABLED_TEST_F(DIVUInstructionBDD, BasicUnsignedDivision)

// 情境2: 除零處理測試
DISABLED_TEST_F(DIVUInstructionBDD, DivideByZeroHandling)

// 情境3: 大數無符號除法測試
DISABLED_TEST_F(DIVUInstructionBDD, LargeUnsignedDivision)

// 情境4: 無符號餘數測試
DISABLED_TEST_F(DIVUInstructionBDD, UnsignedRemainderVerification)
```

## 📊 預期開發成果

### 量化目標
- **新增測試數:** +16個測試 (306 → 322)
- **指令完成:** 4個乘除法指令
- **完成度提升:** 85% → 94%
- **估計開發時間:** 8-10小時

### 質化目標
- **HI/LO暫存器實現:** 完整的特殊暫存器支援
- **64位運算處理:** 正確的64位乘法結果
- **除零異常處理:** 安全的除零情況處理
- **符號/無符號差異:** 正確的有符號/無符號運算

## 🔧 關鍵實現差異

### MULT vs MULTU
- **MULT:** 有符號32×32=64位乘法
- **MULTU:** 無符號32×32=64位乘法
- **結果存儲:** 都使用HI:LO存儲64位結果
- **符號處理:** MULT需要處理負數運算

### DIV vs DIVU
- **DIV:** 有符號除法和取餘
- **DIVU:** 無符號除法和取餘
- **除零處理:** 兩者都需要安全的除零處理
- **符號處理:** DIV需要正確處理負數除法

## ✅ Phase 8 開發檢核清單

### 🔴 Red-Light階段檢核
- [ ] RegisterFile擴展支援HI/LO暫存器
- [ ] MULT BDD測試檔案建立 (4個DISABLED測試)
- [ ] MULTU BDD測試檔案建立 (4個DISABLED測試)  
- [ ] DIV BDD測試檔案建立 (4個DISABLED測試)
- [ ] DIVU BDD測試檔案建立 (4個DISABLED測試)
- [ ] CMakeLists.txt更新包含新測試檔案
- [ ] 編譯確認 - 應該有16個失敗測試

### 🟢 Green-Light階段檢核
- [ ] InstructionDecoder.cpp新增四個function code路由
- [ ] Assembler.cpp新增四個語法解析器
- [ ] 四個指令類別實現完成
- [ ] BDD測試全部通過 (移除DISABLED前綴)
- [ ] 零編譯警告確認

### 🔄 Integration階段檢核
- [ ] 完整回歸測試 (所有306+16個測試通過)
- [ ] Phase 8完成驗收

## 🎯 實現優先順序建議

**推薦開發順序:**
1. **RegisterFile HI/LO擴展 (最高優先)** - 基礎設施必須先完成
2. **MULT (高優先)** - 有符號乘法，相對簡單
3. **MULTU (中等優先)** - 無符號乘法，類似MULT
4. **DIV (中等優先)** - 有符號除法，需要除零處理
5. **DIVU (最低優先)** - 無符號除法，類似DIV

**理由:**
- RegisterFile是所有乘除法指令的基礎
- 乘法指令相對除法指令實現較簡單
- 無符號版本可以複用有符號版本的測試架構

## ⚠️ 技術風險與對策

### 已知風險點
1. **HI/LO暫存器整合:** 可能影響現有RegisterFile功能
2. **64位運算精度:** C++的int64_t處理需要小心
3. **除零異常處理:** 不同平台的除零行為可能不同
4. **符號擴展邏輯:** 有符號/無符號轉換容易出錯

### 風險對策
1. **漸進式開發:** 先完成HI/LO基礎設施，再實現指令
2. **全面測試:** 特別測試邊界值和異常情況
3. **參考實現:** 使用標準MIPS文檔作為參考
4. **單元測試重點:** 對每個組件進行獨立測試

## 🏆 Phase 8成功標準

### 技術指標
- **306 → 322個測試:** 100%通過率
- **零編譯錯誤:** 完整編譯成功
- **BDD循環完整:** 完成16次完整red-light → green-light循環
- **回歸穩定性:** 原有功能保持完整

### 驗收標準  
- **MULT功能:** 正確的有符號64位乘法
- **MULTU功能:** 正確的無符號64位乘法
- **DIV功能:** 正確的有符號除法和取餘
- **DIVU功能:** 正確的無符號除法和取餘
- **HI/LO支援:** 完整的特殊暫存器操作

## 🚀 Phase 9預告

Phase 8完成後，下一階段將進入：
- **Phase 9:** HI/LO存取指令群組 (MFHI, MTHI, MFLO, MTLO)
- **目標測試數:** 322 → 338個測試
- **完成度目標:** 94% → 100%

---

## 🎯 立即下一步行動

**Phase 8.1啟動:** RegisterFile HI/LO擴展
**首要任務:** 修改 `src/RegisterFile.h` 和 `src/RegisterFile.cpp`
**參考文檔:** 此開發指南
**預期成果:** HI/LO暫存器支援，現有測試繼續通過
**預估時間:** 1-2小時
**成功指標:** 編譯無錯誤，306個測試持續通過

**下一步:** MULT指令BDD Red-Light Phase
**任務:** 建立 `tests/test_mult_instruction_bdd_minimal.cpp`
**參考:** `tests/test_lhu_instruction_bdd_minimal.cpp` (最新BDD模板)
**預期成果:** 4個DISABLED_測試，編譯成功但測試失敗
