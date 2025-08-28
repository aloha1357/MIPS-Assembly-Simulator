# MIPS Simulator Project Report

## Structure

The MIPS Assembly Simulator project has the following directory structure:

```
MIPS-Assembly-Simulator/
├── src/                    # Core simulator code
│   ├── Cpu.h/.cpp         # CPU pipeline implementation (5 stages)
│   ├── Memory.h/.cpp      # Memory management system
│   ├── RegisterFile.h/.cpp # Register file
│   ├── Assembler.h/.cpp   # Assembler (two-pass)
│   ├── Instruction.h/.cpp # Instruction class hierarchy
│   ├── IFStage.h/.cpp     # Instruction Fetch stage
│   ├── IDStage.h/.cpp     # Instruction Decode stage
│   ├── EXStage.h/.cpp     # Execute stage
│   ├── MEMStage.h/.cpp    # Memory stage
│   └── WBStage.h/.cpp     # Write Back stage
├── tests/                 # Test suite
│   └── test_*.cpp         # Individual instruction tests
├── assignment/            # Final project deliverables
│   └── test/              # Final test files
│       ├── instructions.asm    # Complete test suite
│       └── instructions.out    # Expected output
├── CMakeLists.txt        # Main build configuration
├── README.md             # Project documentation
└── build/                # Build directory
```

## CLI Introduction

The project provides two sets of CLI tools for different use cases:

### Legacy Simple Tools (src/tools/)

1. **clean_runner.exe** - Clean output runner
   - Executes MIPS programs and generates detailed execution traces
   - Usage: `./build/src/clean_runner.exe <asm-file>`

2. **cli_trace_runner.exe** - Trace runner
   - Executes programs step by step showing PC and return address
   - Usage: `./build/src/cli_trace_runner.exe <asm-file> <maxSteps>`

3. **dump_labels.exe** - Label analyzer
   - Analyzes labels, instructions, and data structures in programs
   - Usage: `./build/src/dump_labels.exe <asm-file>`

4. **jal_debug_runner.exe** - JAL debugger
   - Specialized tool for debugging function calls (jal/jr)
   - Usage: `./build/src/jal_debug_runner.exe <asm-file> [maxSteps]`

5. **mips_cli.exe** - Main CLI
   - Basic MIPS program loader and executor
   - Usage: `./build/src/mips_cli.exe <assembly_file>`

### Modern Complete CLI Tools (cli/)

6. **mipsim.exe** - Complete CLI tool
   - Full-featured MIPS simulator CLI
   - Commands: run, assemble, disasm, repl, dump, help, version
   - Usage: `./build/cli/mipsim.exe <command> [options]`

## Testing Framework

The project implements a comprehensive testing framework using Google Test (gtest) with 88+ individual test files covering all aspects of the MIPS simulator. The tests follow Behavior-Driven Development (BDD) principles and are organized by functionality.

### Test Categories and Coverage

#### 1. **Arithmetic Instructions (14 tests)**
- **Basic Operations:** `test_addiu_instruction_bdd_minimal.cpp`, `test_addu_instruction_bdd_minimal.cpp`
- **Multiplication/Division:** `test_mult_instruction_bdd_minimal.cpp`, `test_multu_instruction_bdd_minimal.cpp`, `test_div_instruction_bdd_minimal.cpp`, `test_divu_instruction_bdd_minimal.cpp`
- **Register Access:** `test_mfhi_instruction_bdd_minimal.cpp`, `test_mflo_instruction_bdd_minimal.cpp`, `test_mthi_instruction_bdd_minimal.cpp`, `test_mtlo_instruction_bdd_minimal.cpp`

#### 2. **Logical Instructions (12 tests)**
- **Bitwise Operations:** `test_and_instruction.cpp`, `test_andi_instruction.cpp`, `test_andi_instruction_integration.cpp`
- **Logical Operations:** `test_logical_and_bdd_minimal.cpp`, `test_logical_andi_bdd_minimal.cpp`, `test_logical_or_bdd_minimal.cpp`, `test_logical_ori_bdd_minimal.cpp`
- **Advanced Logic:** `test_logical_nor_bdd_minimal.cpp`, `test_logical_xor_bdd_minimal.cpp`, `test_logical_xori_bdd_minimal.cpp`
- **Shift Operations:** `test_logical_sll_bdd_minimal.cpp`, `test_logical_sra_bdd_minimal.cpp`, `test_logical_srl_bdd_minimal.cpp`, `test_sll_instruction.cpp`, `test_sra_instruction.cpp`, `test_srl_instruction.cpp`

#### 3. **Memory Instructions (8 tests)**
- **Load Operations:** `test_lb_instruction_bdd_minimal.cpp`, `test_lbu_instruction_bdd_minimal.cpp`, `test_lh_instruction_bdd_minimal.cpp`, `test_lhu_instruction_bdd_minimal.cpp`
- **Store Operations:** `test_sb_instruction_bdd_minimal.cpp`, `test_sh_instruction_bdd_minimal.cpp`
- **Load Immediate:** `test_lhi_instruction.cpp`, `test_llo_instruction.cpp`

#### 4. **Jump and Branch Instructions (12 tests)**
- **Jump Instructions:** `test_jal_instruction_bdd_minimal.cpp`, `test_jr_instruction_bdd_minimal.cpp`, `test_jalr_instruction_bdd_minimal.cpp`, `test_jalr_instruction_bdd_minimal_fixed.cpp`, `test_jalr_instruction_bdd_minimal_v2.cpp`
- **Branch Instructions:** `test_bgtz_instruction_bdd_minimal.cpp`, `test_bgtz_instruction_integration.cpp`, `test_blez_instruction_bdd_minimal.cpp`, `test_blez_instruction_bdd_minimal_clean.cpp`, `test_bne_instruction.cpp`
- **Integration Tests:** `test_jal_jr_integration.cpp`, `test_jalr_instruction_integration.cpp`, `test_jr_instruction_integration.cpp`

#### 5. **System Calls and I/O (4 tests)**
- **Character Operations:** `test_character_syscalls.cpp`
- **System Integration:** `test_syscalls.cpp`, `test_syscall_debug.cpp`
- **Console Output:** `test_mips_core_console.cpp`

#### 6. **Core Components (8 tests)**
- **CPU Core:** `test_cpu.cpp`, `test_bdd_core_instructions.cpp`
- **Instruction Decoder:** `test_instruction_decoder.cpp`
- **Assembler:** `test_assembler_debug.cpp`
- **Pipeline:** `test_pipeline.cpp`, `test_pipeline_execution.cpp`, `test_pipeline_integration.cpp`
- **Register File:** `test_register_file_hilo_bdd.cpp`

#### 7. **CLI and Interface (5 tests)**
- **CLI Arguments:** `test_cli_argument_parsing.cpp`
- **CLI Commands:** `test_cli_assemble_command.cpp`, `test_cli_run_command.cpp`
- **GUI Interface:** `test_gui_interface.cpp`, `test_enhanced_gui_console.cpp`, `test_gui_console_output.cpp`

#### 8. **Integration and Special Cases (11 tests)**
- **Extended Instructions:** `test_extended_instructions_atomic.cpp`
- **Logical Instructions:** `test_logical_instructions_atomic.cpp`
- **Missing Instructions:** `test_missing_instructions_integration.cpp`
- **Mismatch Cases:** `test_mismatch_cases.cpp`
- **Failing Segments:** `test_failing_segments.cpp`
- **Hello World:** `test_hello_mips.cpp`

### Build and Run Tests

```bash
# Build for testing
cmake -B build-full -DFULL_TESTS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-full --config Release

# Run all tests
./build/tests/mips_tests.exe

# Run specific test categories
./build/tests/mips_tests.exe --gtest_filter="*Arithmetic*"
./build/tests/mips_tests.exe --gtest_filter="*Memory*"
./build/tests/mips_tests.exe --gtest_filter="*Jump*"
./build/tests/mips_tests.exe --gtest_filter="*Logical*"
./build/tests/mips_tests.exe --gtest_filter="*Branch*"

# Run single test
./build/tests/mips_tests.exe --gtest_filter="ArithmeticTest.AddInstruction"
./build/tests/mips_tests.exe --gtest_filter="MemoryTest.LoadByte"
./build/tests/mips_tests.exe --gtest_filter="JumpTest.JalInstruction"
```

### Test Methodology

#### BDD (Behavior-Driven Development) Approach
- **Minimal Tests:** Focus on single instruction behavior
- **Integration Tests:** Test instruction combinations and interactions
- **Atomic Tests:** Test fundamental operations in isolation
- **Debug Tests:** Specialized tests for debugging scenarios

#### Test Naming Convention
- `test_<instruction>_bdd_minimal.cpp` - Basic BDD tests for individual instructions
- `test_<instruction>_integration.cpp` - Integration tests combining multiple instructions
- `test_<category>_atomic.cpp` - Atomic tests for fundamental operations
- `test_<component>_debug.cpp` - Debug and diagnostic tests

### Test Output Validation

```bash
# Generate actual output
./build/src/clean_runner.exe assignment/test/instructions.asm > actual_output.txt

# Compare with expected output
Compare-Object (Get-Content actual_output.txt) (Get-Content assignment/test/instructions.out)

# Run specific test suites
./build/tests/mips_tests.exe --gtest_filter="*BDD*"      # All BDD tests
./build/tests/mips_tests.exe --gtest_filter="*Integration*"  # All integration tests
./build/tests/mips_tests.exe --gtest_filter="*Atomic*"   # All atomic tests
```

### Test Statistics

- **Total Test Files:** 88+
- **Test Categories:** 8 major categories
- **Coverage Areas:** Arithmetic, Logical, Memory, Jump, Branch, System Calls, Core Components, CLI/Interface
- **Test Methods:** BDD, Integration, Atomic, Debug
- **Framework:** Google Test (gtest)
- **Build System:** CMake with test configuration

## Results

### MIPS Architecture Implementation

The simulator implements a complete 5-stage pipeline:
- **IF** (Instruction Fetch) → **ID** (Instruction Decode) → **EX** (Execute) → **MEM** (Memory) → **WB** (Write Back)

### Supported Instructions

**Arithmetic Instructions (14):**
- add/sub/addu/subu rd, rs, rt - Addition and subtraction
- addi/addiu rt, rs, immediate - Add immediate
- mult/multu/div/divu rs, rt - Multiplication and division
- mfhi/mflo rd - Read HI/LO registers

**Logical Instructions (7):**
- and/or/xor/nor rd, rs, rt - Bitwise operations
- andi/ori/xori rt, rs, immediate - Bitwise operations with immediate

**Memory Instructions (8):**
- lw/sw rt, offset(rs) - Load/store word
- lb/lbu rt, offset(rs) - Load byte (signed/unsigned)
- lh/lhu rt, offset(rs) - Load halfword (signed/unsigned)
- sb/sh rt, offset(rs) - Store byte/halfword

**Jump Instructions (4):**
- j label - Jump
- jal label - Jump and link
- jr rs - Jump register
- jalr rd, rs - Jump and link register

**Branch Instructions (6):**
- beq/bne rs, rt, label - Branch equal/not equal
- blez/bgtz rs, label - Branch less/equal zero, greater than zero
- bltz/bgez rs, label - Branch less than, greater/equal zero

**Shift Instructions (6):**
- sll/srl/sra rd, rt, shamt - Logical/arithmetic shifts
- sllv/srlv/srav rd, rt, rs - Variable shifts

**Comparison Instructions (3):**
- slt/sltu rd, rs, rt - Set less than (signed/unsigned)
- slti/sltiu rt, rs, immediate - Set less than immediate

**Load Immediate (2):**
- lui rt, immediate - Load upper half
- llo rt, immediate - Load lower half

**System Calls (8):**
- syscall with codes: 1(print_int), 4(print_string), 5(read_int), 8(read_string), 10(exit), 11(print_char), 12(read_char), 2(print_float)

### Testing Results

All CLI tools have been tested and are working correctly:

 **Legacy Tools:**
- clean_runner.exe - Successfully executes programs with detailed tracing
- cli_trace_runner.exe - Shows step-by-step PC and $ra tracking
- dump_labels.exe - Correctly analyzes program structure and labels
- jal_debug_runner.exe - Properly handles function call debugging
- mips_cli.exe - Executes programs for default 10 cycles

 **Modern Tools:**
- mipsim.exe - Full CLI functionality with help system and run command

 **Test Files:**
- assignment/test/instructions.asm - Complete test suite file exists and works with all tools

### Performance and Features

- **Memory System:** 4KB address space with word alignment
- **Register File:** 32 general-purpose registers plus special registers (ra, sp, etc.)
- **Assembler:** Two-pass assembler with label resolution
- **Debugging:** Comprehensive tracing and step-by-step execution
- **Output:** Clean execution results and detailed debug information


