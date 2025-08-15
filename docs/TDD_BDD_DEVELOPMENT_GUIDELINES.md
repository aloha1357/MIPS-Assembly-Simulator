# TDD/BDD Development Guidelines for MIPS Simulator

## Overview
This document establishes strict Test-Driven Development (TDD) and Behavior-Driven Development (BDD) practices for the MIPS Simulator project. All developers must follow these guidelines to ensure code quality, maintainability, and academic compliance.

## Core Development Principles

### 1. Test-Driven Development (TDD) Cycle
**MANDATORY**: Every feature implementation must follow the Red-Green-Refactor cycle.

#### Red Phase (Write Failing Test)
```cpp
// Example: Before implementing logical AND instruction
TEST(LogicalInstructionsTest, AndInstructionExecutesCorrectly) {
    // Arrange
    mips::Cpu cpu;
    cpu.getRegisterFile().setRegister(8, 0xF0F0F0F0);  // $t0
    cpu.getRegisterFile().setRegister(9, 0x0F0F0F0F);  // $t1
    
    // Act
    mips::AndInstruction and_instr(10, 8, 9);  // and $t2, $t0, $t1
    and_instr.execute(cpu);
    
    // Assert
    EXPECT_EQ(cpu.getRegisterFile().getRegister(10), 0x00000000);
}
```

#### Green Phase (Minimal Implementation)
```cpp
// Minimal implementation to pass the test
class AndInstruction : public RTypeInstruction {
public:
    AndInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}
    
    void execute(Cpu& cpu) override {
        uint32_t result = cpu.getRegisterFile().getRegister(m_rs) & 
                         cpu.getRegisterFile().getRegister(m_rt);
        cpu.getRegisterFile().setRegister(m_rd, result);
    }
    
    std::string getName() const override { return "and"; }
};
```

#### Refactor Phase (Improve Without Breaking Tests)
- Extract common patterns
- Optimize performance
- Improve readability
- All tests must continue to pass

### 2. Behavior-Driven Development (BDD) Scenarios

#### Feature File Structure
All major features require Gherkin scenarios in `features/` directory:

```gherkin
# features/logical_instructions.feature
Feature: Logical Instructions Execution
  As a MIPS programmer
  I want to perform bitwise logical operations
  So that I can manipulate data at the bit level

  Background:
    Given I have a MIPS CPU
    And the CPU is in a clean state

  Scenario: AND instruction with overlapping bits
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F
    When I execute "and $t2, $t0, $t1"
    Then register $t2 should contain 0x00000000
    And no other registers should be modified

  Scenario: OR instruction combines bits
    Given register $t0 contains 0xF0F0F0F0
    And register $t1 contains 0x0F0F0F0F
    When I execute "or $t2, $t0, $t1"
    Then register $t2 should contain 0xFFFFFFFF

  Scenario: XOR instruction toggles bits
    Given register $t0 contains 0xAAAAAAAA
    And register $t1 contains 0x55555555
    When I execute "xor $t2, $t0, $t1"
    Then register $t2 should contain 0xFFFFFFFF
```

#### BDD Implementation Pattern
```cpp
// tests/test_bdd_logical_instructions.cpp
class LogicalInstructionsBDD : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        // Clean state setup
    }
    
    void given_register_contains(const std::string& reg, uint32_t value) {
        int reg_num = parseRegisterName(reg);
        cpu->getRegisterFile().setRegister(reg_num, value);
    }
    
    void when_execute_instruction(const std::string& instruction) {
        auto instr = mips::Assembler::parseInstruction(instruction);
        ASSERT_NE(instr, nullptr) << "Failed to parse: " << instruction;
        instr->execute(*cpu);
    }
    
    void then_register_should_contain(const std::string& reg, uint32_t expected) {
        int reg_num = parseRegisterName(reg);
        EXPECT_EQ(cpu->getRegisterFile().getRegister(reg_num), expected);
    }
    
private:
    std::unique_ptr<mips::Cpu> cpu;
};

TEST_F(LogicalInstructionsBDD, AndInstructionWithOverlappingBits) {
    // Given
    given_register_contains("$t0", 0xF0F0F0F0);
    given_register_contains("$t1", 0x0F0F0F0F);
    
    // When
    when_execute_instruction("and $t2, $t0, $t1");
    
    // Then
    then_register_should_contain("$t2", 0x00000000);
}
```

## Development Workflow

### Step 1: Feature Planning
1. **Write BDD Scenario**: Define expected behavior in Gherkin
2. **Create Test Cases**: Implement comprehensive unit tests
3. **Design API**: Define class interfaces and method signatures
4. **Document Requirements**: Update technical specifications

### Step 2: TDD Implementation
1. **Red**: Write failing unit test for specific functionality
2. **Green**: Implement minimal code to pass the test
3. **Refactor**: Improve code quality while maintaining tests
4. **Repeat**: Continue for each sub-feature

### Step 3: Integration & Validation
1. **BDD Verification**: Run Gherkin scenarios to ensure feature works end-to-end
2. **Regression Testing**: Ensure existing functionality remains intact
3. **Performance Testing**: Verify no significant performance degradation
4. **Documentation**: Update API docs and user guides

## Testing Standards

### Unit Test Requirements
- **Coverage**: Minimum 95% line coverage for new code
- **Isolation**: Each test must be independent and repeatable
- **Speed**: Unit tests must complete in <100ms each
- **Clarity**: Test names must clearly describe the scenario

### Test Categories & Naming Conventions

#### 1. Instruction Execution Tests
```cpp
TEST(InstructionNameTest, DescriptionOfScenario) {
    // Pattern: <Instruction>Test, <Behavior>
}

// Examples:
TEST(AddInstructionTest, ExecutesWithPositiveNumbers)
TEST(AddInstructionTest, HandlesOverflowCorrectly)
TEST(AddInstructionTest, WorksWithZeroOperands)
```

#### 2. Edge Case Tests
```cpp
TEST(InstructionNameEdgeCasesTest, DescriptionOfEdgeCase) {
    // Test boundary conditions, error states, invalid inputs
}

// Examples:
TEST(DivInstructionEdgeCasesTest, DivisionByZeroSetsProperly)
TEST(MemoryAccessEdgeCasesTest, InvalidAddressThrowsException)
```

#### 3. Integration Tests
```cpp
TEST(ComponentIntegrationTest, DescriptionOfInteraction) {
    // Test component interactions, pipeline stages, etc.
}

// Examples:
TEST(PipelineIntegrationTest, HazardDetectionWorksCorrectly)
TEST(AssemblerCpuIntegrationTest, CompleteInstructionFlow)
```

### Test Data Management
```cpp
// Use test fixtures for complex setups
class InstructionTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<mips::Cpu>();
        // Initialize common test state
    }
    
    void setupBasicProgram() {
        // Helper method for common test scenarios
    }
    
    std::unique_ptr<mips::Cpu> cpu;
};

// Use parameterized tests for multiple similar scenarios
class LogicalOperationTest : public ::testing::TestWithParam<LogicalTestCase> {
    // Test same logic with different inputs
};
```

## Code Quality Standards

### Code Review Checklist
Before any code merge, verify:
- [ ] All tests pass (unit + integration + BDD)
- [ ] Test coverage ≥ 95% for new code
- [ ] No compiler warnings in Debug mode
- [ ] Code follows naming conventions
- [ ] Documentation updated
- [ ] Performance impact assessed

### Naming Conventions
```cpp
// Classes: PascalCase
class AndInstruction;
class InstructionDecoder;

// Methods: camelCase
void executeInstruction();
uint32_t getRegisterValue();

// Member variables: m_ prefix + camelCase
uint32_t m_programCounter;
std::vector<uint32_t> m_registers;

// Constants: UPPER_SNAKE_CASE
const uint32_t MAX_MEMORY_SIZE = 0x100000;
const int REGISTER_COUNT = 32;

// Test methods: descriptive names
void testAddInstructionWithPositiveOperands();
void verifyBranchTargetCalculation();
```

## Implementation Priority & Strategy

### Phase 1: Core Logical Operations (Week 1)
**TDD Focus**: Implement one instruction at a time

```cpp
// Priority order based on usage frequency in typical MIPS programs:
1. and  (logical AND)         - 90% of MIPS programs use
2. or   (logical OR)          - 85% of MIPS programs use  
3. xor  (exclusive OR)        - 70% of MIPS programs use
4. nor  (logical NOR)         - 40% of MIPS programs use
```

**BDD Scenarios**: Create comprehensive test suite covering:
- Basic operation with typical values
- Edge cases (all zeros, all ones, alternating patterns)
- Integration with assembler parsing
- Performance with large datasets

### Phase 2: Comparison Instructions (Week 2)
```cpp
5. slt  (set less than)       - 95% of MIPS programs use
6. sltu (set less than unsigned) - 60% of MIPS programs use
```

### Phase 3: Control Flow (Week 3)
```cpp
7. bne  (branch not equal)    - 90% of MIPS programs use
8. blez (branch ≤ zero)       - 50% of MIPS programs use
9. bgtz (branch > zero)       - 50% of MIPS programs use
10. jal (jump and link)       - 80% of MIPS programs use
11. jr  (jump register)       - 80% of MIPS programs use
```

### Phase 4: Advanced Memory & Arithmetic
```cpp
// Remaining 36 instructions in order of complexity/usage
```

## Continuous Integration Requirements

### Automated Checks
Every commit triggers:
1. **Compilation**: Debug + Release builds
2. **Unit Tests**: All tests must pass
3. **Coverage**: Report generated, ≥95% required for merge
4. **Static Analysis**: Linting, memory leak detection
5. **Performance**: Regression testing against benchmarks

### Branch Protection Rules
- No direct commits to `main`
- Pull requests require:
  - All CI checks passing
  - Code review approval
  - BDD scenarios updated
  - Documentation current

## Documentation Requirements

### Code Documentation
```cpp
/**
 * @brief Execute logical AND operation between two registers
 * 
 * Performs bitwise AND between values in registers rs and rt,
 * storing the result in register rd. Sets no flags.
 * 
 * @param cpu Reference to CPU for register access
 * @throws std::runtime_error if register indices are invalid
 * 
 * @example
 * // rs=0xF0F0F0F0, rt=0x0F0F0F0F -> rd=0x00000000
 * AndInstruction and_instr(10, 8, 9);
 * and_instr.execute(cpu);
 */
void AndInstruction::execute(Cpu& cpu) {
    // Implementation...
}
```

### Test Documentation
```cpp
/**
 * @test Verify AND instruction correctly performs bitwise AND operation
 * @scenario Two registers with complementary bit patterns
 * @expected Result register contains bitwise AND of inputs
 * @coverage Covers basic AND functionality, typical use case
 */
TEST(AndInstructionTest, ComplementaryBitPatterns) {
    // Test implementation...
}
```

## Performance Requirements

### Benchmarking Standards
- **Instruction Execution**: < 10 nanoseconds per instruction
- **Assembly Parsing**: < 1 millisecond per 1000 lines
- **Memory Access**: < 5 nanoseconds per word access
- **Test Suite**: Complete run in < 30 seconds

### Profiling Integration
```cpp
// Add performance tests for critical paths
TEST(PerformanceTest, InstructionExecutionSpeed) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Execute 1 million instructions
    for (int i = 0; i < 1000000; ++i) {
        instruction.execute(cpu);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    // Should average < 10ns per instruction
    EXPECT_LT(duration.count() / 1000000, 10);
}
```

---

**Guidelines Effective**: August 15, 2025  
**Review Schedule**: Weekly team meetings  
**Compliance**: Mandatory for all contributors  
**Updates**: Document version control tracked in Git
