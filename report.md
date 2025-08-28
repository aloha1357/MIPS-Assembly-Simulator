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

The project uses Google Test framework for comprehensive testing:

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

# Run single test
./build/tests/mips_tests.exe --gtest_filter="ArithmeticTest.AddInstruction"
```

### Test Output Validation

```bash
# Generate actual output
./build/src/clean_runner.exe assignment/test/instructions.asm > actual_output.txt

# Compare with expected output
Compare-Object (Get-Content actual_output.txt) (Get-Content assignment/test/instructions.out)
```

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


