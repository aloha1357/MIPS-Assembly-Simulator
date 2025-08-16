# 🚀 Phase 9 開發指南 - HI/LO存取指令群組 (MFHI, MTHI, MFLO, MTLO)

## 📋 Phase 9 開發目標

Phase 9 將完成MIPS HI/LO存取指令群組的實現，這些指令負責在HI/LO特殊暫存器與通用暫存器之間傳輸資料：

### 目標指令清單
1. **MFHI** (Move From HI) - R-type 指令
   - **Function Code:** 0x10
   - **語法:** `mfhi $rd`
   - **功能:** $rd = HI (將HI暫存器內容移動到通用暫存器)
   
2. **MTHI** (Move To HI) - R-type 指令
   - **Function Code:** 0x11
   - **語法:** `mthi $rs`
   - **功能:** HI = $rs (將通用暫存器內容移動到HI暫存器)

3. **MFLO** (Move From LO) - R-type 指令
   - **Function Code:** 0x12
   - **語法:** `mflo $rd`
   - **功能:** $rd = LO (將LO暫存器內容移動到通用暫存器)

4. **MTLO** (Move To LO) - R-type 指令
   - **Function Code:** 0x13
   - **語法:** `mtlo $rs`
   - **功能:** LO = $rs (將通用暫存器內容移動到LO暫存器)

## 🎯 技術要點與設計原則

### HI/LO存取指令特性
- **簡單資料傳輸:** 純粹的資料移動操作，無算術邏輯運算
- **特殊暫存器存取:** 唯一能夠存取HI/LO暫存器的指令
- **相互依賴性:** 通常與乘除法指令配合使用
- **效能要求:** 單週期執行，無副作用

### 實現策略
```cpp
// MFHI實現範例 (R-type)
void MFHIInstruction::execute(Cpu& cpu) {
    uint32_t hiValue = cpu.getRegisterFile().readHI();
    cpu.getRegisterFile().write(m_rd, hiValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

// MTHI實現範例 (R-type)
void MTHIInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    cpu.getRegisterFile().writeHI(rsValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

// MFLO實現範例 (R-type)
void MFLOInstruction::execute(Cpu& cpu) {
    uint32_t loValue = cpu.getRegisterFile().readLO();
    cpu.getRegisterFile().write(m_rd, loValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

// MTLO實現範例 (R-type)
void MTLOInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    cpu.getRegisterFile().writeLO(rsValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

## 🔄 BDD實現週期規劃

### 🔴 Phase 9.1: MFHI指令 Red-Light Phase
**目標檔案:** `tests/test_mfhi_instruction_bdd_minimal.cpp`

**必要測試情境 (DISABLED_前綴):**
```cpp
// 情境1: 基本HI讀取測試
DISABLED_TEST_F(MFHIInstructionBDD, BasicHIRead)

// 情境2: 零值HI讀取測試
DISABLED_TEST_F(MFHIInstructionBDD, ZeroHIRead)

// 情境3: 最大值HI讀取測試
DISABLED_TEST_F(MFHIInstructionBDD, MaxValueHIRead)

// 情境4: 不同目標暫存器測試
DISABLED_TEST_F(MFHIInstructionBDD, DifferentTargetRegister)
```

### 🟢 Phase 9.2: MFHI指令 Green-Light Phase
**實現文件:**
1. **InstructionDecoder.cpp** - 新增function code 0x10路由
2. **Assembler.cpp** - 新增"mfhi"語法解析
3. **指令類別** - 創建MFHIInstruction類別

### 🔴 Phase 9.3: MTHI指令 Red-Light Phase
**目標檔案:** `tests/test_mthi_instruction_bdd_minimal.cpp`

**必要測試情境:**
```cpp
// 情境1: 基本HI寫入測試
DISABLED_TEST_F(MTHIInstructionBDD, BasicHIWrite)

// 情境2: 零值HI寫入測試
DISABLED_TEST_F(MTHIInstructionBDD, ZeroHIWrite)

// 情境3: 最大值HI寫入測試
DISABLED_TEST_F(MTHIInstructionBDD, MaxValueHIWrite)

// 情境4: 覆寫現有HI值測試
DISABLED_TEST_F(MTHIInstructionBDD, OverwriteExistingHI)
```

### 🔴 Phase 9.4: MFLO指令 Red-Light Phase
**目標檔案:** `tests/test_mflo_instruction_bdd_minimal.cpp`

### 🔴 Phase 9.5: MTLO指令 Red-Light Phase
**目標檔案:** `tests/test_mtlo_instruction_bdd_minimal.cpp`

## 📊 預期開發成果

### 量化目標
- **新增測試數:** +16個測試 (330 → 346)
- **指令完成:** 4個HI/LO存取指令
- **完成度提升:** 94% → 100%
- **估計開發時間:** 4-6小時

### 質化目標
- **HI/LO存取完整性:** 完整的特殊暫存器存取能力
- **乘除法指令互補:** 與Phase 8指令完美配合
- **指令集完成:** MIPS核心指令集實現達到100%
- **架構完整性:** 完整的MIPS處理器模擬能力

## 🔑 關鍵實現差異

### MFHI vs MFLO
- **MFHI:** 從HI暫存器讀取，通常用於取得乘法高位結果
- **MFLO:** 從LO暫存器讀取，通常用於取得乘法低位結果或除法商
- **共同特性:** 都是從特殊暫存器到通用暫存器的資料傳輸

### MTHI vs MTLO
- **MTHI:** 向HI暫存器寫入，用於設定HI暫存器初始值
- **MTLO:** 向LO暫存器寫入，用於設定LO暫存器初始值
- **共同特性:** 都是從通用暫存器到特殊暫存器的資料傳輸

### 典型使用情境
```mips
# 典型乘法操作序列
mult $t0, $t1    # 乘法結果存入HI:LO
mflo $t2         # 取得低32位結果
mfhi $t3         # 取得高32位結果

# 典型除法操作序列
div $t0, $t1     # 除法結果存入LO，餘數存入HI
mflo $t2         # 取得商
mfhi $t3         # 取得餘數

# HI/LO暫存器初始化
li $t0, 0x12345678
mthi $t0         # 設定HI初始值
mtlo $zero       # 清零LO暫存器
```

## ✅ Phase 9 開發檢核清單

### 🔴 Red-Light階段檢核
- [ ] MFHI BDD測試檔案建立 (4個DISABLED測試)
- [ ] MTHI BDD測試檔案建立 (4個DISABLED測試)  
- [ ] MFLO BDD測試檔案建立 (4個DISABLED測試)
- [ ] MTLO BDD測試檔案建立 (4個DISABLED測試)
- [ ] CMakeLists.txt更新包含新測試檔案
- [ ] 編譯確認 - 應該有16個失敗測試

### 🟢 Green-Light階段檢核
- [ ] InstructionDecoder.cpp新增四個function code路由 (0x10-0x13)
- [ ] Assembler.cpp新增四個語法解析器 (mfhi, mthi, mflo, mtlo)
- [ ] 四個指令類別實現完成
- [ ] BDD測試全部通過 (移除DISABLED前綴)
- [ ] 零編譯警告確認

### 🔄 Integration階段檢核
- [ ] 完整回歸測試 (所有330+16個測試通過)
- [ ] Phase 9完成驗收
- [ ] MIPS指令集100%完成慶祝 🎉

## 🚀 實現優先順序建議

**推薦開發順序:**
1. **MFHI (最高優先)** - 最常用，從乘法結果取高位
2. **MFLO (高優先)** - 常用，從乘法/除法結果取值
3. **MTHI (中等優先)** - 用於HI暫存器初始化
4. **MTLO (最低優先)** - 用於LO暫存器初始化

**理由:**
- MFHI/MFLO是乘除法指令的自然延續，使用頻率最高
- 可重用Phase 8的HI/LO暫存器基礎設施
- Move From指令比Move To指令更常用

## 📝 BDD測試模板

### MFHI指令BDD測試範例
```cpp
/**
 * @file test_mfhi_instruction_bdd_minimal.cpp
 * @brief MFHI指令的BDD測試場景
 */

#include <gtest/gtest.h>
#include "Cpu.h"
#include "Instruction.h"

class MFHIInstructionBDD : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<Cpu>();
        instruction = std::make_unique<MFHIInstruction>(10); // $t2
    }

    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<MFHIInstruction> instruction;
};

TEST_F(MFHIInstructionBDD, BasicHIRead_ShouldMoveHIToRegister) {
    // Given: HI暫存器包含特定值
    cpu->getRegisterFile().writeHI(0x12345678);
    cpu->getRegisterFile().write(10, 0x00000000); // $t2 = 0
    
    // When: 執行MFHI指令
    instruction->execute(*cpu);
    
    // Then: 目標暫存器應包含HI的值
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x12345678);
    EXPECT_EQ(cpu->getRegisterFile().readHI(), 0x12345678); // HI不變
}

TEST_F(MFHIInstructionBDD, ZeroHIRead_ShouldMoveZeroToRegister) {
    // Given: HI暫存器為零
    cpu->getRegisterFile().writeHI(0x00000000);
    cpu->getRegisterFile().write(10, 0xFFFFFFFF); // $t2 = 全1
    
    // When: 執行MFHI指令
    instruction->execute(*cpu);
    
    // Then: 目標暫存器應為零
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x00000000);
}

// 其他測試情境...
```

## 🎯 Phase 9成功標準

### 技術指標
- **330 → 346個測試:** 100%通過率
- **零編譯錯誤:** 完整編譯成功
- **BDD循環完整:** 完成16次完整red-light → green-light循環
- **回歸穩定性:** 原有功能保持完整

### 驗收標準  
- **MFHI功能:** 正確的HI暫存器讀取
- **MTHI功能:** 正確的HI暫存器寫入
- **MFLO功能:** 正確的LO暫存器讀取
- **MTLO功能:** 正確的LO暫存器寫入
- **整合品質:** Assembler + Decoder完整支援

## 🏆 MIPS指令集完成里程碑

Phase 9完成後，將達到以下重大里程碑：
- **指令完成度:** 44/47 → 47/47 (100%)
- **測試覆蓋:** 346個測試，完整BDD覆蓋
- **架構完整性:** 完整的MIPS處理器模擬能力
- **開發品質:** 零技術債務，生產級代碼品質

## 📚 參考資源

### 最佳參考範例檔案
- **指令參考:** `src/Instruction.cpp` 中的 MULT/DIV 指令
- **BDD參考:** `tests/test_divu_instruction_bdd_minimal.cpp`
- **RegisterFile參考:** `src/RegisterFile.h/cpp` HI/LO實現
- **解碼參考:** `src/InstructionDecoder.cpp` 中的 R-type function codes

### MIPS架構文檔
- HI/LO暫存器使用規範
- 乘除法指令與HI/LO存取指令的配合使用
- R-type指令編碼格式

## 🎉 預期完成慶祝

Phase 9完成後，MIPS Assembly Simulator將達到：
- **100%核心指令集支援** 🏆
- **完整BDD測試覆蓋** ✅
- **生產級代碼品質** 💎
- **成熟的架構設計** 🏗️

這將是一個重大的開發里程碑，標誌著MIPS處理器模擬器的完整實現！

---

**🎯 立即下一步行動**
**Phase 9.1啟動:** MFHI指令BDD Red-Light Phase
**首要任務:** 建立 `tests/test_mfhi_instruction_bdd_minimal.cpp`
**參考文檔:** `tests/test_divu_instruction_bdd_minimal.cpp` (最新BDD模板)
**預期成果:** 4個DISABLED_測試，編譯成功但測試失敗
**預估時間:** 30分鐘
**成功指標:** 編譯無錯誤，4個紅燈測試
