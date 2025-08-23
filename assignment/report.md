# MIPS Simulator - Final Project Report

## Project Overview

This project implements a MIPS assembly simulator with a 5-stage pipeline architecture. The simulator supports core MIPS instructions and provides both command-line and graphical interfaces for educational purposes.

### Key Features
- 5-Stage Pipeline: IF → ID → EX → MEM → WB
- Command Line Interface (CLI)
- Graphical User Interface (GUI) using Qt6
- Comprehensive test suite using Catch2 and GoogleTest
- Support for 9 core MIPS instructions

## Architecture Design

### Core Components

#### CPU Pipeline
The simulator implements a classic 5-stage MIPS pipeline:
1. **Instruction Fetch (IF)**: Fetches instructions from memory
2. **Instruction Decode (ID)**: Decodes instructions and reads registers
3. **Execute (EX)**: Performs arithmetic and logic operations
4. **Memory Access (MEM)**: Handles load/store operations
5. **Write Back (WB)**: Writes results back to registers

#### Memory System
- 4KB addressable memory space
- Word-aligned memory access
- Support for load and store operations

#### Register File
- 32 general-purpose registers
- Program counter management
- Support for register read/write operations

## Supported Instructions

### R-Type Instructions
- `add rd, rs, rt` - Addition
- `sub rd, rs, rt` - Subtraction
- `sll rd, rt, shamt` - Shift left logical

### I-Type Instructions
- `addi rt, rs, immediate` - Add immediate
- `lw rt, offset(rs)` - Load word
- `sw rt, offset(rs)` - Store word
- `beq rs, rt, label` - Branch if equal

### J-Type Instructions
- `j label` - Jump

### System Instructions
- `syscall` - System call (print, read, exit)

## Implementation Details

### Assembler
Two-pass assembler that converts MIPS assembly code to machine code:
- First pass: builds symbol table and resolves labels
- Second pass: generates binary instructions

### GUI Interface
Qt6-based graphical interface providing:
- Assembly code editor
- Register value display
- Memory viewer
- Execution control (run, step, reset)
- Console output display

### Testing Strategy
Comprehensive testing using two frameworks:
- **Catch2**: Unit tests for core functionality
- **GoogleTest**: Integration and system tests
- Test coverage includes all supported instructions and pipeline operations

## Build System

### CMake Configuration
The project uses CMake with the following external dependencies:
- Qt6 (Core, Widgets) for GUI functionality
- Catch2 for unit testing
- GoogleTest for additional testing

### Build Targets
- `my_cli_tool`: Command-line MIPS simulator
- `my_gui_tool`: GUI-based MIPS simulator
- `my_catch2_tests`: Unit tests using Catch2
- `my_gtest_tests`: Unit tests using GoogleTest

## Educational Value

This simulator serves as an educational tool for learning:
- MIPS assembly language programming
- Computer architecture concepts
- Pipeline design and hazard handling
- Memory hierarchy and instruction execution

### Sample Programs
The project includes example MIPS assembly programs demonstrating:
- Basic arithmetic operations
- Memory access patterns
- Control flow instructions
- System call usage

## Testing and Validation

### Test Coverage
The test suite validates:
- Individual instruction execution
- Pipeline correctness
- Memory operations
- Register file operations
- Assembler functionality

### Performance
- Fast compilation and execution
- Efficient memory usage
- Real-time GUI updates

## Challenges and Solutions

### Technical Challenges
1. **Pipeline Hazards**: Implemented hazard detection and handling
2. **GUI Integration**: Seamless Qt6 integration with core simulator
3. **Cross-platform Support**: CMake configuration for multiple platforms

### Design Decisions
- Modular architecture for maintainability
- Separation of concerns between GUI and core logic
- Comprehensive testing for reliability

## Conclusion

This MIPS simulator successfully demonstrates:
- Complete implementation of MIPS pipeline architecture
- Educational value through interactive GUI
- Robust testing and validation
- Clean, maintainable code structure

The project provides an excellent platform for understanding computer architecture concepts through hands-on experimentation with MIPS assembly programming.

## Build Instructions

```bash
# Configure and build
cmake -B build
cmake --build build

# Run tests
./build/my_catch2_tests
./build/my_gtest_tests

# Execute simulators
./build/my_cli_tool
./build/my_gui_tool
```
