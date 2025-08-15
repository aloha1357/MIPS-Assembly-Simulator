# MIPS Simulator Development Implementation Guide

## Immediate Action Plan (Next 7 Days)

### Day 1-2: Foundation Setup
1. **TA Clarification** - Send library compliance inquiry
2. **Development Environment** - Verify all tools working
3. **Test Framework Enhancement** - Expand BDD scenarios

### Day 3-7: Core Logical Instructions Implementation
**Target**: Implement `and`, `or`, `xor`, `nor` instructions using strict TDD

## TDD Implementation Template

### Phase 1: Logical AND Instruction (Template for others)

#### Step 1: Write BDD Scenario (Red)
```gherkin
# features/logical_instructions.feature
Feature: Logical AND Instruction
  As a MIPS programmer
  I want to perform bitwise AND operations
  So that I can mask and filter data bits

  Scenario: AND with complementary patterns
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F  
    When I execute "and $t2, $t0, $t1"
    Then register $t2 should contain 0x00000000
    And the instruction should complete in one cycle

  Scenario: AND with identical values
    Given register $s0 contains 0xAAAAAAAA
    And register $s1 contains 0xAAAAAAAA
    When I execute "and $s2, $s0, $s1"  
    Then register $s2 should contain 0xAAAAAAAA

  Scenario: AND with zero operand
    Given register $t0 contains 0xFFFFFFFF
    And register $t1 contains 0x00000000
    When I execute "and $t2, $t0, $t1"
    Then register $t2 should contain 0x00000000
```

#### Step 2: Create Failing Unit Test (Red)
```cpp
// tests/test_logical_instructions.cpp
#include <gtest/gtest.h>
#include "Instruction.h"
#include "Cpu.h"

class LogicalInstructionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
    }
    
    std::unique_ptr<mips::Cpu> cpu;
};

TEST_F(LogicalInstructionsTest, AndInstructionComplementaryPatterns) {
    // Arrange
    cpu->getRegisterFile().setRegister(8, 0xF0F0F0F0);  // $t0
    cpu->getRegisterFile().setRegister(9, 0x0F0F0F0F);  // $t1
    
    // Act
    mips::AndInstruction and_instr(10, 8, 9);  // and $t2, $t0, $t1
    and_instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().getRegister(10), 0x00000000);
    // Verify source registers unchanged
    EXPECT_EQ(cpu->getRegisterFile().getRegister(8), 0xF0F0F0F0);
    EXPECT_EQ(cpu->getRegisterFile().getRegister(9), 0x0F0F0F0F);
}

TEST_F(LogicalInstructionsTest, AndInstructionIdenticalValues) {
    // Arrange
    cpu->getRegisterFile().setRegister(16, 0xAAAAAAAA); // $s0
    cpu->getRegisterFile().setRegister(17, 0xAAAAAAAA); // $s1
    
    // Act
    mips::AndInstruction and_instr(18, 16, 17); // and $s2, $s0, $s1
    and_instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().getRegister(18), 0xAAAAAAAA);
}

TEST_F(LogicalInstructionsTest, AndInstructionWithZeroOperand) {
    // Arrange
    cpu->getRegisterFile().setRegister(8, 0xFFFFFFFF);  // $t0
    cpu->getRegisterFile().setRegister(9, 0x00000000);  // $t1
    
    // Act
    mips::AndInstruction and_instr(10, 8, 9);  // and $t2, $t0, $t1
    and_instr.execute(*cpu);
    
    // Assert
    EXPECT_EQ(cpu->getRegisterFile().getRegister(10), 0x00000000);
}
```

#### Step 3: Minimal Implementation (Green)
```cpp
// src/Instruction.h - Add to header
/**
 * @brief Logical AND instruction
 */
class AndInstruction : public RTypeInstruction {
public:
    AndInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};
```

```cpp
// src/Instruction.cpp - Add implementation
AndInstruction::AndInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void AndInstruction::execute(Cpu& cpu) {
    uint32_t rs_value = cpu.getRegisterFile().getRegister(m_rs);
    uint32_t rt_value = cpu.getRegisterFile().getRegister(m_rt);
    uint32_t result = rs_value & rt_value;
    cpu.getRegisterFile().setRegister(m_rd, result);
}

std::string AndInstruction::getName() const {
    return "and";
}
```

#### Step 4: Add to Decoder (Green)
```cpp
// src/InstructionDecoder.cpp - Update decodeRType method
std::unique_ptr<Instruction> InstructionDecoder::decodeRType(uint32_t word) {
    uint32_t rs = extractRs(word);
    uint32_t rt = extractRt(word);
    uint32_t rd = extractRd(word);
    uint32_t shamt = extractShamt(word);
    uint32_t function = extractFunction(word);
    
    switch (function) {
        case 0x00:  // SLL instruction
            return std::make_unique<SllInstruction>(rd, rt, shamt);
        case 0x20:  // ADD instruction
            return std::make_unique<AddInstruction>(rd, rs, rt);
        case 0x22:  // SUB instruction
            return std::make_unique<SubInstruction>(rd, rs, rt);
        case 0x24:  // AND instruction - NEW
            return std::make_unique<AndInstruction>(rd, rs, rt);
        case 0x0C:  // SYSCALL instruction
            return std::make_unique<SyscallInstruction>();
        default:
            return nullptr; // Unknown R-type instruction
    }
}
```

#### Step 5: Add to Assembler (Green)
```cpp
// src/Assembler.cpp - Update parseInstruction method
std::unique_ptr<Instruction> Assembler::parseInstruction(const std::string& line) {
    // ... existing code ...
    
    if (opcode == "add") {
        // ... existing ADD parsing ...
    } else if (opcode == "and") {  // NEW
        if (tokens.size() != 4) {
            throw std::runtime_error("AND instruction requires 3 operands");
        }
        
        int rd = parseRegisterName(tokens[1]);
        int rs = parseRegisterName(tokens[2]); 
        int rt = parseRegisterName(tokens[3]);
        
        return std::make_unique<AndInstruction>(rd, rs, rt);
    }
    // ... continue with other instructions ...
}
```

#### Step 6: Run Tests (Green → Red Check)
```bash
# Build and run tests
cmake --build build --config Debug
cd build
.\tests\unit_tests.exe --gtest_filter="*LogicalInstructions*"
```

#### Step 7: Refactor (Improve Quality)
```cpp
// Potential refactoring: Extract common logical operation pattern
class LogicalInstructionBase : public RTypeInstruction {
protected:
    LogicalInstructionBase(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}
    
    virtual uint32_t performOperation(uint32_t a, uint32_t b) const = 0;
    
public:
    void execute(Cpu& cpu) override final {
        uint32_t rs_value = cpu.getRegisterFile().getRegister(m_rs);
        uint32_t rt_value = cpu.getRegisterFile().getRegister(m_rt);
        uint32_t result = performOperation(rs_value, rt_value);
        cpu.getRegisterFile().setRegister(m_rd, result);
    }
};

class AndInstruction : public LogicalInstructionBase {
public:
    AndInstruction(int rd, int rs, int rt) : LogicalInstructionBase(rd, rs, rt) {}
    
protected:
    uint32_t performOperation(uint32_t a, uint32_t b) const override {
        return a & b;
    }
    
public:
    std::string getName() const override { return "and"; }
};
```

### Rapid Development Template for Remaining Instructions

#### OR Instruction (15-minute implementation)
```cpp
// Copy AND pattern, change operation to |
class OrInstruction : public LogicalInstructionBase {
    uint32_t performOperation(uint32_t a, uint32_t b) const override { return a | b; }
    std::string getName() const override { return "or"; }
};

// Add to decoder: case 0x25: return std::make_unique<OrInstruction>(rd, rs, rt);
// Add to assembler: else if (opcode == "or") { /* same pattern as AND */ }
// Add tests: TEST_F(LogicalInstructionsTest, OrInstruction...)
```

#### XOR Instruction (15-minute implementation)
```cpp
class XorInstruction : public LogicalInstructionBase {
    uint32_t performOperation(uint32_t a, uint32_t b) const override { return a ^ b; }
    std::string getName() const override { return "xor"; }
};
```

#### NOR Instruction (15-minute implementation)
```cpp
class NorInstruction : public LogicalInstructionBase {
    uint32_t performOperation(uint32_t a, uint32_t b) const override { return ~(a | b); }
    std::string getName() const override { return "nor"; }
};
```

## Development Sprint Plan

### Sprint 1: Core Logical Operations (Days 3-5)
**Goal**: Complete `and`, `or`, `xor`, `nor` with full test coverage

**Daily Tasks**:
- Day 3: AND instruction (full TDD cycle, ~2 hours)
- Day 4: OR, XOR instructions (pattern reuse, ~2 hours)  
- Day 5: NOR instruction + refactoring (cleanup, ~2 hours)

**Acceptance Criteria**:
- [ ] All 4 instructions decode correctly from machine code
- [ ] All 4 instructions parse correctly from assembly
- [ ] All 4 instructions execute with correct results
- [ ] Test coverage ≥ 95% for new code
- [ ] BDD scenarios pass for all logical operations
- [ ] Performance: < 10ns per instruction execution

### Sprint 2: Comparison Instructions (Days 6-7)
**Goal**: Implement `slt`, `sltu` with signed/unsigned handling

```cpp
// Template for SLT implementation
class SltInstruction : public RTypeInstruction {
public:
    void execute(Cpu& cpu) override {
        int32_t rs_signed = static_cast<int32_t>(cpu.getRegisterFile().getRegister(m_rs));
        int32_t rt_signed = static_cast<int32_t>(cpu.getRegisterFile().getRegister(m_rt));
        uint32_t result = (rs_signed < rt_signed) ? 1 : 0;
        cpu.getRegisterFile().setRegister(m_rd, result);
    }
};

class SltuInstruction : public RTypeInstruction {
public:
    void execute(Cpu& cpu) override {
        uint32_t rs_value = cpu.getRegisterFile().getRegister(m_rs);
        uint32_t rt_value = cpu.getRegisterFile().getRegister(m_rt);
        uint32_t result = (rs_value < rt_value) ? 1 : 0;
        cpu.getRegisterFile().setRegister(m_rd, result);
    }
};
```

### Sprint 3: Branch Instructions (Week 2)
**Goal**: Implement `bne`, `blez`, `bgtz` with program counter updates

```cpp
// Template for branch implementation
class BneInstruction : public BranchInstruction {
public:
    void execute(Cpu& cpu) override {
        uint32_t rs_value = cpu.getRegisterFile().getRegister(m_rs);
        uint32_t rt_value = cpu.getRegisterFile().getRegister(m_rt);
        
        if (rs_value != rt_value) {
            // Calculate branch target
            uint32_t pc = cpu.getProgramCounter();
            int32_t offset = signExtend16(m_offset) << 2;  // Word-aligned
            cpu.setProgramCounter(pc + offset);
        }
    }
};
```

## Quality Assurance Checklist

### Before Each Commit
- [ ] All existing tests still pass
- [ ] New tests added for new functionality  
- [ ] Code coverage ≥ 95% for modified files
- [ ] No compiler warnings in Debug build
- [ ] BDD scenarios updated and passing
- [ ] Performance benchmarks within limits
- [ ] Documentation updated

### Before Each Sprint Review
- [ ] Complete instruction set for sprint implemented
- [ ] Integration tests verify end-to-end functionality
- [ ] Performance regression tests completed
- [ ] User documentation updated
- [ ] Code review completed
- [ ] Demo prepared for stakeholder review

## Risk Mitigation Strategies

### Technical Risks
1. **Instruction Complexity**: Start with simplest logical operations first
2. **Test Coverage**: Aim for 100% coverage with edge cases
3. **Performance**: Profile early, optimize critical paths
4. **Integration**: Test assembler + decoder + CPU together frequently

### Academic Risks  
1. **Library Compliance**: Prepare fallback plan for GUI restrictions
2. **Scope Creep**: Focus on core instructions first, add advanced features later
3. **Time Management**: Use time-boxed sprints, cut features if necessary
4. **Quality vs Speed**: Never compromise on test coverage or documentation

---

**Implementation Guide Version**: 1.0  
**Target Completion**: End of August 2025  
**Success Metrics**: 95% instruction coverage, 95% test coverage, TA approval
