# Development Checklist & Quick Reference

## 🎯 Immediate Actions (Next 48 Hours)

### ✅ Completed Setup
- [x] Development environment verified (CMake, Ninja, GoogleTest)
- [x] TDD/BDD framework implemented (Gherkin + cucumber-cpp)
- [x] Test templates created for rapid development
- [x] Documentation guidelines established
- [x] CI/CD pipeline working (Debug + Release builds)

### 🔥 URGENT: TA Clarification Required
- [ ] **Send library compliance email** (use template from PROJECT_DEVELOPMENT_SUMMARY.md)
- [ ] **Get approval for**: Dear ImGui + SDL2 + OpenGL vs Qt6 vs console-only
- [ ] **Clarify**: Minimum instruction set requirements (17% vs 60% vs 100%)
- [ ] **Confirm**: GoogleTest acceptable as "test framework exception"

### 📋 Development Backlog (Ready to Start) ⭐ 更新為原子化開發

#### Priority 1: Logical Instructions (Week 1) - 原子化實作就緒
- [ ] **Scenario 1**: `and` instruction (TDD: 2 hours estimated)
  - [ ] 基本 AND 運算測試 (0xF0F0F0F0 & 0x0F0F0F0F = 0x00000000)
  - [ ] 交替位元模式測試 (0xAAAAAAAA & 0x55555555 = 0x00000000)
  - [ ] 全1測試 (0xFFFFFFFF & 0xFFFFFFFF = 0xFFFFFFFF)
  - [ ] 實用遮罩測試 (0x12345678 & 0xFFFFF000 = 0x12345000)
  - [ ] 零值測試和暫存器別名測試
  - [ ] BDD場景: 6個原子測試場景
  
- [ ] **Scenario 2**: `or` instruction (TDD: 1.5 hours estimated)
  - [ ] 基本 OR 運算測試 (0xF0F0F0F0 | 0x0F0F0F0F = 0xFFFFFFFF)
  - [ ] 位元合併測試 (0x12340000 | 0x00005678 = 0x12345678)
  - [ ] 與零OR恆等運算測試
  - [ ] BDD場景: 5個原子測試場景
  
- [ ] **Scenario 3**: `xor` instruction (TDD: 1.5 hours estimated)
  - [ ] 基本 XOR 運算測試 (0xAAAAAAAA ^ 0x55555555 = 0xFFFFFFFF)
  - [ ] 自我XOR歸零測試 (A ^ A = 0)
  - [ ] 可逆性測試 (加密/解密應用)
  - [ ] BDD場景: 6個原子測試場景，包含加密應用
  
- [ ] **Scenario 4**: `nor` instruction (TDD: 1.5 hours estimated)
  - [ ] 基本 NOR 運算測試 (~(0xF0F0F0F0 | 0x0F0F0F0F) = 0x00000000)
  - [ ] 作為NOT運算測試 (A NOR 0 = ~A)
  - [ ] 德摩根定律驗證
  - [ ] BDD場景: 5個原子測試場景

#### Priority 2: Comparison Instructions (Week 2)
- [ ] `slt` instruction (TDD: 2 hours estimated)
- [ ] `sltu` instruction (TDD: 1.5 hours estimated)

#### Priority 2: Control Flow (Week 2)
- [ ] `bne` instruction (branch not equal)
- [ ] `blez` instruction (branch ≤ zero)
- [ ] `bgtz` instruction (branch > zero)
- [ ] `jal` instruction (jump and link)
- [ ] `jr` instruction (jump register)

#### Priority 3: Memory Operations (Week 2)
- [ ] `lb`, `lbu` instructions (load byte)
- [ ] `lh`, `lhu` instructions (load halfword)
- [ ] `sb` instruction (store byte)  
- [ ] `sh` instruction (store halfword)

## 🛠️ TDD Development Template (Copy-Paste Ready) ⭐ 原子化版本

### 原子化開發流程 (每個指令)

#### Step 1: 建立 BDD 原子場景
```gherkin
# 添加到 features/logical_instructions_atomic.feature
Scenario Outline: [INSTRUCTION] instruction performs bitwise [OPERATION] operation
  Given register <source1> contains <value1>
  And register <source2> contains <value2>
  When I execute the instruction "[INSTRUCTION] <dest>, <source1>, <source2>"
  Then register <dest> should contain <expected>

  Examples:
    | source1 | source2 | dest | value1     | value2     | expected   |
    | $t0     | $t1     | $t2  | [VALUE1]   | [VALUE2]   | [EXPECTED] |
```

#### Step 2: 編寫原子測試套件
```cpp
// 添加到 tests/test_logical_instructions_atomic.cpp
TEST_F(LogicalInstructionsTest, [Instruction]Instruction_BasicOperation) {
    // Arrange
    cpu->getRegisterFile().write(8, [VALUE1]);  // $t0
    cpu->getRegisterFile().write(9, [VALUE2]);  // $t1
    
    // Act
    mips::[Instruction]Instruction instr(10, 8, 9);
    instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().read(10), [EXPECTED]);
    EXPECT_EQ(cpu->getRegisterFile().read(8), [VALUE1]);   // 源暫存器不變
    EXPECT_EQ(cpu->getRegisterFile().read(9), [VALUE2]);   // 源暫存器不變
}
```

#### Step 3: 原子化實作模板
```cpp
// 添加到 src/Instruction.h
class [Instruction]Instruction : public RTypeInstruction {
public:
    [Instruction]Instruction(int rd, int rs, int rt);
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

// 添加到 src/Instruction.cpp
void [Instruction]Instruction::execute(Cpu& cpu) {
    uint32_t rs_val = cpu.getRegisterFile().read(m_rs);
    uint32_t rt_val = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rs_val [OPERATOR] rt_val;  // 替換 [OPERATOR]
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

#### Step 4: 原子化整合到系統
```cpp
// src/InstructionDecoder.cpp - 添加到 decodeRType():
case [FUNCTION_CODE]: return std::make_unique<[Instruction]Instruction>(rd, rs, rt);

// src/Assembler.cpp - 添加到 parseInstruction():
else if (opcode == "[instruction]") {
    return std::make_unique<[Instruction]Instruction>(rd, rs, rt);
}
```

### 原子化驗證檢查點
- [ ] 單一指令獨立測試通過
- [ ] 與現有指令無衝突  
- [ ] BDD場景100%覆蓋
- [ ] 效能要求達標 (< 10ns/指令)
- [ ] 代碼覆蓋率100%

## 📊 Current Instruction Set Status

### ✅ Implemented (8/47 = 17%)
| Type | Instruction | Opcode | Function | Status |
|------|-------------|--------|----------|---------|
| R | `add` | 0x00 | 0x20 | ✅ |
| R | `sub` | 0x00 | 0x22 | ✅ |
| R | `sll` | 0x00 | 0x00 | ✅ |
| R | `syscall` | 0x00 | 0x0C | ✅ |
| I | `addi` | 0x08 | - | ✅ |
| I | `lw` | 0x23 | - | ✅ |
| I | `sw` | 0x2B | - | ✅ |
| I | `beq` | 0x04 | - | ✅ |
| J | `j` | 0x02 | - | ✅ |

### 🎯 Next to Implement (Priority Order)
| Priority | Instruction | Opcode | Function | Effort | 
|----------|-------------|--------|----------|--------|
| 1 | `and` | 0x00 | 0x24 | 2h |
| 2 | `or` | 0x00 | 0x25 | 1.5h |
| 3 | `xor` | 0x00 | 0x26 | 1.5h |
| 4 | `nor` | 0x00 | 0x27 | 1.5h |
| 5 | `slt` | 0x00 | 0x2A | 2h |
| 6 | `sltu` | 0x00 | 0x2B | 1.5h |

## 🧪 Testing Commands (Quick Reference)

### Build & Test (Debug)
```bash
cmake --build build --config Debug
cd build
.\tests\unit_tests.exe --gtest_brief=1
```

### Build & Test (Release)  
```bash
cmake --build build --config Release
cd build
.\tests\unit_tests.exe --gtest_brief=1
```

### Run Specific Tests
```bash
.\tests\unit_tests.exe --gtest_filter="*Logical*"
.\tests\unit_tests.exe --gtest_filter="*AndInstruction*"
```

### Performance Testing
```bash
.\tests\unit_tests.exe --gtest_filter="*Performance*"
```

## 📈 Weekly Sprint Goals ⭐ 原子化更新

### Week 1 Target (Aug 16-22) - 邏輯指令原子化實作
- **Goal**: 實作 4 個邏輯指令，從 19% → 27% 覆蓋率 (9→13 指令)
- **Focus**: AND, OR, XOR, NOR 四個核心邏輯指令
- **Method**: 原子化TDD，每個指令獨立完成
- **Deliverable**: 
  - [ ] 4 個新指令完全實作
  - [ ] 25+ 個新原子測試
  - [ ] 20+ 個新BDD場景
  - [ ] 完整效能基準測試
- **Success Criteria**: 
  - [ ] 所有新測試100%通過
  - [ ] 現有測試零回歸
  - [ ] 代碼覆蓋率 ≥ 95%
  - [ ] 效能要求達標

### Week 1 每日里程碑
- **Day 1 (Aug 16)**: AND 指令完成 + 6個原子測試通過
- **Day 2 (Aug 17)**: OR 指令完成 + 5個原子測試通過
- **Day 3 (Aug 18)**: XOR 指令完成 + 6個原子測試通過 (包含加密測試)
- **Day 4 (Aug 19)**: NOR 指令完成 + 5個原子測試通過
- **Day 5 (Aug 20)**: 整合測試 + 複合指令測試 + 效能測試

### Week 2 Target (Aug 23-29) - 比較指令與控制流程擴展
- **Goal**: 實作比較指令，達到 35% 覆蓋率 (16/47 指令)
- **Focus**: SLT, SLTU 比較指令 + 額外控制流程指令
- **Method**: 基於Week 1建立的原子化模式
- **Deliverable**: 比較指令完整實作與測試

## 🚨 Risk Mitigation

### High Risk: Library Rejection
**Probability**: 60%  
**Impact**: 2-3 days rework  
**Mitigation**: Qt6 backup plan + headless fallback ready

### Medium Risk: Time Underestimation
**Probability**: 40%  
**Impact**: Incomplete implementation  
**Mitigation**: Focus on highest-priority instructions first

### Low Risk: Performance Issues
**Probability**: 20%  
**Impact**: Slow execution  
**Mitigation**: Profile early, optimize critical paths

## 📞 Next Actions Summary

1. **TODAY**: Send TA clarification email (use template)
2. **Day 1-2**: Implement logical instructions (and, or, xor, nor)
3. **Day 3-4**: Implement comparison instructions (slt, sltu)  
4. **Day 5**: Code review, refactoring, documentation update
5. **Week 2**: Control flow and memory operations

**Project Confidence**: HIGH ✅  
**Readiness Level**: Ready to execute immediately upon TA approval
