# MIPS Assembly Simulator - 原子化開發Scenarios與測試標準

**建立日期**: 2025年8月15日  
**目的**: 基於目前實作狀況，提供原子化的開發scenarios和測試標準  
**適用範圍**: 從當前9個指令擴展到完整MIPS指令集

## 📊 當前實作狀況分析

### ✅ 已實作指令 (9/47 = 19%)
| 指令 | 類型 | 功能碼 | TDD狀態 | BDD狀態 |
|------|------|--------|---------|---------|
| `add` | R-type | 0x20 | ✅ 完成 | ✅ 完成 |
| `sub` | R-type | 0x22 | ✅ 完成 | ✅ 完成 |
| `sll` | R-type | 0x00 | ✅ 完成 | ✅ 完成 |
| `syscall` | R-type | 0x0C | ✅ 完成 | ✅ 完成 |
| `addi` | I-type | 0x08 | ✅ 完成 | ✅ 完成 |
| `lw` | I-type | 0x23 | ✅ 完成 | ✅ 完成 |
| `sw` | I-type | 0x2B | ✅ 完成 | ✅ 完成 |
| `beq` | I-type | 0x04 | ✅ 完成 | ✅ 完成 |
| `j` | J-type | 0x02 | ✅ 完成 | ✅ 完成 |

### 🎯 優先開發目標 (下一階段)
基於邏輯指令的重要性和複雜度，將按以下優先順序開發：

## 🚀 原子化開發Scenarios

---

## Scenario 1: AND邏輯指令實作
**優先級**: P1 (最高)  
**預估時間**: 2小時  
**複雜度**: 中等

### 📋 驗收標準 (Acceptance Criteria)

#### 功能需求
1. **基本功能**: `and $rd, $rs, $rt` 執行位元AND運算
2. **結果正確性**: result = rs_value & rt_value
3. **寄存器更新**: 只更新目標寄存器$rd
4. **PC更新**: 自動遞增PC到下一指令

#### 測試覆蓋範圍
- [x] 基本AND運算 (0xF0F0F0F0 & 0x0F0F0F0F = 0x00000000)
- [x] 全1運算 (0xFFFFFFFF & 0xFFFFFFFF = 0xFFFFFFFF)
- [x] 零值運算 (任何值 & 0x00000000 = 0x00000000)
- [x] 邊界值測試
- [x] 寄存器別名測試 (and $t0, $t0, $t0)

### 🧪 BDD場景定義

```cucumber
Feature: AND邏輯指令
  As a MIPS programmer
  I want to perform bitwise AND operations
  So that I can mask and filter bits efficiently

  Background:
    Given I have a MIPS CPU simulator
    And the CPU is in a clean state with all registers set to zero

  Scenario Outline: AND instruction with various bit patterns
    Given register <source1> contains <value1>
    And register <source2> contains <value2>
    When I execute the instruction "and <dest>, <source1>, <source2>"
    Then register <dest> should contain <expected>
    And register <source1> should still contain <value1>
    And register <source2> should still contain <value2>
    And the instruction should complete in exactly 1 cycle

    Examples:
      | source1 | source2 | dest | value1     | value2     | expected   |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000 |
      | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0x00000000 |
      | $t3     | $t4     | $t5  | 0xFFFFFFFF | 0xFFFFFFFF | 0xFFFFFFFF |
      | $a0     | $a1     | $v0  | 0x12345678 | 0xFFFFF000 | 0x12345000 |
```

### 💻 TDD測試模板

```cpp
// tests/test_logical_instructions.cpp
#include <gtest/gtest.h>
#include "../src/Instruction.h"
#include "../src/Cpu.h"

class LogicalInstructionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
    }
    
    std::unique_ptr<mips::Cpu> cpu;
};

// 原子測試1: 基本AND運算
TEST_F(LogicalInstructionsTest, AndInstructionBasicOperation) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xF0F0F0F0);  // $t0
    cpu->getRegisterFile().write(9, 0x0F0F0F0F);  // $t1
    
    // Act
    mips::AndInstruction instr(10, 8, 9);  // and $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x00000000);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xF0F0F0F0);   // 源寄存器不變
    EXPECT_EQ(cpu->getRegisterFile().read(9), 0x0F0F0F0F);   // 源寄存器不變
}

// 原子測試2: 全1測試
TEST_F(LogicalInstructionsTest, AndInstructionAllOnes) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xFFFFFFFF);  // $t0
    cpu->getRegisterFile().write(9, 0xFFFFFFFF);  // $t1
    
    // Act
    mips::AndInstruction instr(10, 8, 9);  // and $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0xFFFFFFFF);
}

// 原子測試3: 寄存器別名測試
TEST_F(LogicalInstructionsTest, AndInstructionRegisterAliasing) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xFFFF0000);  // $t0
    
    // Act
    mips::AndInstruction instr(8, 8, 8);  // and $t0, $t0, $t0
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xFFFF0000);  // 應該不變
}

// 原子測試4: 零值測試
TEST_F(LogicalInstructionsTest, AndInstructionWithZero) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xDEADBEEF);  // $t0
    cpu->getRegisterFile().write(9, 0x00000000);  // $t1 = 0
    
    // Act
    mips::AndInstruction instr(10, 8, 9);  // and $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x00000000);
}
```

### 🔧 實作模板

```cpp
// src/Instruction.h - 添加類別宣告
class AndInstruction : public RTypeInstruction {
public:
    AndInstruction(int rd, int rs, int rt);
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

// src/Instruction.cpp - 實作
AndInstruction::AndInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {}

void AndInstruction::execute(Cpu& cpu) {
    uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
    uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rs_val & rt_val;
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AndInstruction::getName() const {
    return "and";
}

// src/InstructionDecoder.cpp - 添加到解碼器
case 0x24: return std::make_unique<AndInstruction>(rd, rs, rt);

// src/Assembler.cpp - 添加到彙編器
else if (opcode == "and") {
    return std::make_unique<AndInstruction>(rd, rs, rt);
}
```

### ✅ Definition of Done
- [ ] 所有單元測試通過 (4個原子測試)
- [ ] BDD場景通過 (5個範例)
- [ ] 指令解碼正確
- [ ] 彙編解析正確
- [ ] 代碼覆蓋率達到100%
- [ ] 性能測試: 單條指令 < 10ns
- [ ] 文件更新完成

---

## Scenario 2: OR邏輯指令實作
**優先級**: P1  
**預估時間**: 1.5小時  
**複雜度**: 低 (基於AND的經驗)

### 📋 驗收標準

#### 功能需求
1. **基本功能**: `or $rd, $rs, $rt` 執行位元OR運算
2. **結果正確性**: result = rs_value | rt_value
3. **寄存器更新**: 只更新目標寄存器$rd
4. **PC更新**: 自動遞增PC到下一指令

### 🧪 BDD場景定義

```cucumber
Scenario Outline: OR instruction combines bits correctly
  Given register <source1> contains <value1>
  And register <source2> contains <value2>
  When I execute the instruction "or <dest>, <source1>, <source2>"
  Then register <dest> should contain <expected>

  Examples:
    | source1 | source2 | dest | value1     | value2     | expected   |
    | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0xFFFFFFFF |
    | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0xFFFFFFFF |
    | $t3     | $t4     | $t5  | 0x00000000 | 0x00000000 | 0x00000000 |
    | $a0     | $a1     | $v0  | 0x12340000 | 0x00005678 | 0x12345678 |
```

### 💻 TDD測試模板

```cpp
TEST_F(LogicalInstructionsTest, OrInstructionBasicOperation) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xF0F0F0F0);  // $t0
    cpu->getRegisterFile().write(9, 0x0F0F0F0F);  // $t1
    
    // Act
    mips::OrInstruction instr(10, 8, 9);  // or $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0xFFFFFFFF);
}

TEST_F(LogicalInstructionsTest, OrInstructionWithZero) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xDEADBEEF);  // $t0
    cpu->getRegisterFile().write(9, 0x00000000);  // $t1 = 0
    
    // Act
    mips::OrInstruction instr(10, 8, 9);  // or $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0xDEADBEEF);  // OR with 0 = identity
}
```

### ✅ Definition of Done
- [ ] 3個原子測試通過
- [ ] BDD場景通過 (4個範例)
- [ ] 與AND指令架構一致
- [ ] 代碼覆蓋率100%

---

## Scenario 3: XOR邏輯指令實作
**優先級**: P1  
**預估時間**: 1.5小時  
**複雜度**: 低

### 📋 驗收標準

#### 功能需求
1. **基本功能**: `xor $rd, $rs, $rt` 執行位元XOR運算
2. **結果正確性**: result = rs_value ^ rt_value
3. **特殊性質**: A XOR A = 0, A XOR 0 = A

### 🧪 BDD場景定義

```cucumber
Scenario Outline: XOR instruction toggles bits correctly
  Given register <source1> contains <value1>
  And register <source2> contains <value2>
  When I execute the instruction "xor <dest>, <source1>, <source2>"
  Then register <dest> should contain <expected>

  Examples:
    | source1 | source2 | dest | value1     | value2     | expected   |
    | $t0     | $t1     | $t2  | 0xAAAAAAAA | 0x55555555 | 0xFFFFFFFF |
    | $s0     | $s1     | $s2  | 0x12345678 | 0x12345678 | 0x00000000 |
    | $t3     | $t4     | $t5  | 0xDEADBEEF | 0x00000000 | 0xDEADBEEF |
    | $a0     | $a1     | $v0  | 0xFFFFFFFF | 0xAAAAAAAA | 0x55555555 |
```

### 💻 TDD測試模板

```cpp
TEST_F(LogicalInstructionsTest, XorInstructionBasicOperation) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xAAAAAAAA);  // $t0
    cpu->getRegisterFile().write(9, 0x55555555);  // $t1
    
    // Act
    mips::XorInstruction instr(10, 8, 9);  // xor $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0xFFFFFFFF);
}

TEST_F(LogicalInstructionsTest, XorInstructionSelfXor) {
    // Arrange
    cpu->getRegisterFile().write(8, 0x12345678);  // $t0
    
    // Act
    mips::XorInstruction instr(10, 8, 8);  // xor $t2, $t0, $t0
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x00000000);  // A XOR A = 0
}
```

---

## Scenario 4: NOR邏輯指令實作
**優先級**: P1  
**預估時間**: 1.5小時  
**複雜度**: 低

### 📋 驗收標準

#### 功能需求
1. **基本功能**: `nor $rd, $rs, $rt` 執行位元NOR運算
2. **結果正確性**: result = ~(rs_value | rt_value)
3. **特殊用途**: 可用於NOT運算 (NOR with zero)

### 🧪 BDD場景定義

```cucumber
Scenario Outline: NOR instruction performs negated OR
  Given register <source1> contains <value1>
  And register <source2> contains <value2>
  When I execute the instruction "nor <dest>, <source1>, <source2>"
  Then register <dest> should contain <expected>

  Examples:
    | source1 | source2 | dest | value1     | value2     | expected   |
    | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000 |
    | $s0     | $s1     | $s2  | 0x00000000 | 0x00000000 | 0xFFFFFFFF |
    | $t3     | $t4     | $t5  | 0xAAAAAAAA | 0x00000000 | 0x55555555 |
```

### 💻 TDD測試模板

```cpp
TEST_F(LogicalInstructionsTest, NorInstructionBasicOperation) {
    // Arrange
    cpu->getRegisterFile().write(8, 0xF0F0F0F0);  // $t0
    cpu->getRegisterFile().write(9, 0x0F0F0F0F);  // $t1
    
    // Act
    mips::NorInstruction instr(10, 8, 9);  // nor $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x00000000);  // ~(F0F0F0F0 | 0F0F0F0F) = ~FFFFFFFF = 0
}

TEST_F(LogicalInstructionsTest, NorInstructionAsNot) {
    // Arrange - NOR with zero acts as NOT
    cpu->getRegisterFile().write(8, 0xAAAAAAAA);  // $t0
    cpu->getRegisterFile().write(9, 0x00000000);  // $t1 = 0
    
    // Act
    mips::NorInstruction instr(10, 8, 9);  // nor $t2, $t0, $zero
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0x55555555);  // ~AAAAAAAA = 55555555
}
```

---

## Scenario 5: SLT比較指令實作
**優先級**: P2  
**預估時間**: 2小時  
**複雜度**: 中等 (需要處理符號)

### 📋 驗收標準

#### 功能需求
1. **基本功能**: `slt $rd, $rs, $rt` 設定小於比較
2. **結果正確性**: result = (rs < rt) ? 1 : 0 (有符號比較)
3. **符號處理**: 正確處理32位有符號整數

### 🧪 BDD場景定義

```cucumber
Scenario Outline: SLT instruction sets register based on signed comparison
  Given register <source1> contains <value1>
  And register <source2> contains <value2>
  When I execute the instruction "slt <dest>, <source1>, <source2>"
  Then register <dest> should contain <expected>

  Examples:
    | source1 | source2 | dest | value1 | value2 | expected |
    | $t0     | $t1     | $t2  | 5      | 10     | 1        |
    | $s0     | $s1     | $s2  | 10     | 5      | 0        |
    | $t3     | $t4     | $t5  | -5     | 5      | 1        |
    | $a0     | $a1     | $v0  | 5      | -5     | 0        |
    | $t6     | $t7     | $t8  | -10    | -5     | 1        |
```

### 💻 TDD測試模板

```cpp
TEST_F(LogicalInstructionsTest, SltInstructionPositiveNumbers) {
    // Arrange
    cpu->getRegisterFile().write(8, 5);   // $t0
    cpu->getRegisterFile().write(9, 10);  // $t1
    
    // Act
    mips::SltInstruction instr(10, 8, 9);  // slt $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 1);  // 5 < 10 is true
}

TEST_F(LogicalInstructionsTest, SltInstructionNegativeNumbers) {
    // Arrange
    cpu->getRegisterFile().write(8, static_cast<uint32_t>(-5));   // $t0 = -5
    cpu->getRegisterFile().write(9, 5);                          // $t1 = 5
    
    // Act
    mips::SltInstruction instr(10, 8, 9);  // slt $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 1);  // -5 < 5 is true
}

TEST_F(LogicalInstructionsTest, SltInstructionEqualValues) {
    // Arrange
    cpu->getRegisterFile().write(8, 42);  // $t0
    cpu->getRegisterFile().write(9, 42);  // $t1
    
    // Act
    mips::SltInstruction instr(10, 8, 9);  // slt $t2, $t0, $t1
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), 0);  // 42 < 42 is false
}
```

### 🔧 實作要點

```cpp
void SltInstruction::execute(Cpu& cpu) {
    int32_t rs_val = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    int32_t rt_val = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));
    uint32_t result = (rs_val < rt_val) ? 1 : 0;
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

---

## Scenario 6: SLTU無符號比較指令實作
**優先級**: P2  
**預估時間**: 1.5小時  
**複雜度**: 低 (基於SLT)

### 📋 驗收標準

#### 功能需求
1. **基本功能**: `sltu $rd, $rs, $rt` 設定無符號小於比較
2. **結果正確性**: result = (rs < rt) ? 1 : 0 (無符號比較)
3. **無符號處理**: 所有值都視為正數

### 🧪 BDD場景定義

```cucumber
Scenario Outline: SLTU instruction sets register based on unsigned comparison
  Given register <source1> contains <value1>
  And register <source2> contains <value2>
  When I execute the instruction "sltu <dest>, <source1>, <source2>"
  Then register <dest> should contain <expected>

  Examples:
    | source1 | source2 | dest | value1     | value2     | expected |
    | $t0     | $t1     | $t2  | 5          | 10         | 1        |
    | $s0     | $s1     | $s2  | 10         | 5          | 0        |
    | $t3     | $t4     | $t5  | 0xFFFFFFFF | 5          | 0        |
    | $a0     | $a1     | $v0  | 5          | 0xFFFFFFFF | 1        |
```

---

## 📈 開發節奏與里程碑

### Week 1: 邏輯指令組 (P1)
- **Day 1**: AND指令 (Scenario 1)
- **Day 2**: OR指令 (Scenario 2)  
- **Day 3**: XOR指令 (Scenario 3)
- **Day 4**: NOR指令 (Scenario 4)
- **Day 5**: 整合測試與重構

### Week 2: 比較指令組 (P2)
- **Day 1-2**: SLT指令 (Scenario 5)
- **Day 3**: SLTU指令 (Scenario 6)
- **Day 4-5**: 複合指令測試與性能優化

## 🔄 持續整合流程

### 每個Scenario的標準流程
1. **紅燈階段** (30分鐘)
   - 寫BDD場景
   - 寫失敗的單元測試
   - 確認測試失敗

2. **綠燈階段** (60分鐘)
   - 最小實作讓測試通過
   - 添加到解碼器和彙編器
   - 確認所有測試通過

3. **重構階段** (30分鐘)
   - 代碼清理和優化
   - 文件更新
   - 性能驗證

### 質量檢查點
- [ ] 單元測試覆蓋率 ≥ 95%
- [ ] BDD場景覆蓋率 100%
- [ ] 靜態代碼分析無警告
- [ ] 記憶體洩漏檢查通過
- [ ] 性能基準測試通過

## 🎯 成功指標

### 定量指標
- **指令覆蓋率**: 從19% → 32% (15個指令)
- **測試數量**: 從85個 → 120個測試
- **執行性能**: 每條指令 < 10ns
- **編譯時間**: < 30秒 (Debug模式)

### 定性指標
- **代碼一致性**: 所有新指令遵循現有架構模式
- **可維護性**: 新增功能不破壞現有測試
- **可擴展性**: 為後續指令實作建立良好基礎

## 📚 參考資料

### 技術參考
- [MIPS指令集手冊](../requirement/mips.pdf)
- [現有指令實作](../src/Instruction.cpp)
- [測試框架範例](../tests/test_logical_instructions.cpp)

### 開發工具
- **建置系統**: CMake + Ninja
- **測試框架**: GoogleTest + BDD
- **持續整合**: GitHub Actions
- **代碼分析**: 內建靜態分析工具

---

**總結**: 這份原子化開發scenarios提供了從當前狀態到下一階段目標的詳細路徑。每個scenario都是獨立的、可測試的，並且有明確的驗收標準。按照這個計劃執行，可以確保高質量、一致性的開發進度。
