# MIPS Assembly Simulator – Project Overview

## 🎯 Current Status (July 31, 2025) - CORE COMPLETE + GUI FULLY FUNCTIONAL! 🎉

**Sprint 5 Complete**: Dear ImGui GUI Development 100% DONE! 🚀  
**Core Status**: ✅ 64/64 tests passing (100%) - ALL CORE FEATURES COMPLETE!  
**GUI Status**: 🎨 Dear ImGui Integration 100% Complete + FULLY EXECUTABLE!  
**Implemented Instructions**: ADD, SUB, ADDI, LW, SW, BEQ, J, SYSCALL  
**Pipeline Status**: 5-stage pipeline fully operational  
**New Achievement**: Dear ImGui + SDL2 + OpenGL GUI with COMPLETE EXECUTION CAPABILITY

### 📊 Development Progress Overview
```
Core MIPS Simulator: 100% ✅ COMPLETE
├── 7 MIPS Instructions: Fully implemented & tested
├── 5-Stage Pipeline: Fully operational 
├── Dual Execution Modes: Single-cycle + Pipeline
├── System Calls: 4 syscalls supported
├── Memory System: Word-aligned 4KB memory
└── Test Suite: 64/64 tests passing

GUI Development: 100% ✅ COMPLETE & EXECUTABLE  
├── BDD Specifications: 37 scenarios ✅ COMPLETE
├── Test Framework: Full test suite ✅ COMPLETE
├── Interface Design: Complete API design ✅ COMPLETE
├── Base Implementation: Headless version ✅ COMPLETE
├── Dear ImGui Architecture: SDL2 + OpenGL ✅ COMPLETE
├── GUI Application: Final integration ✅ COMPLETE
└── ⭐ CODE EXECUTION: Full MIPS program execution ✅ NEW!
```

### ⭐ New GUI Execution Features
- **🚀 Full Program Execution**: Complete MIPS programs can now run in GUI
- **🔧 Step-by-Step Debugging**: Single-instruction execution with real-time visualization
- **📊 Live Register/Memory Updates**: Watch values change during execution
- **📋 Demo Program Loading**: Built-in example programs for quick testing
- **🖥️ System Call Support**: Full syscall output display in console
- **🔄 Reset Functionality**: Complete simulator state reset

### 📚 **Essential Documentation for Developers**
- **📄 `docs/README_DOCS.md`** - **START HERE!** Complete documentation index and navigation guide
- **📄 `docs/CURRENT_PROJECT_STATUS_2025.md`** - Current project status overview and achievements
- **📄 `docs/user-guides/GUI_USER_MANUAL.md`** - Complete GUI user manual and learning guide
- **📄 `docs/user-guides/QUICK_REFERENCE.md`** - Quick command reference for daily development
- **📄 `docs/development-reports/DEVELOPMENT_HANDOVER_REPORT.md`** - Developer handover guide

### 🚀 Quick Start
```powershell
# Build and test
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug
ctest --test-dir build

# Run GUI simulator
.\build\src\mips-sim-gui.exe

# Run CLI simulator  
.\build\src\mips-sim.exe
```

## 🧪 Testing & Verification

### Test Suite Overview
- **64 Unit Tests**: 100% passing ✅
- **37 BDD Scenarios**: All scenarios verified ✅
- **Code Coverage**: >90% line coverage ✅
- **Integration Tests**: GUI + Core functionality ✅

### Sample Programs & Test Cases

#### 📁 `asmtest/` Directory - Example Programs
| File | Purpose | Instructions Tested | Expected Output |
|------|---------|-------------------|-----------------|
| `gui_demo_test.asm` | GUI demonstration | ADD, SUB, ADDI, LW, SW, SYSCALL | Prints: 40, 10 |
| `demo_syscalls.asm` | System call examples | SYSCALL (all 4 types) | Interactive I/O demo |
| `test_addi.asm` | Immediate arithmetic | ADDI | Register value testing |
| `test_control.asm` | Control flow | BEQ, J | Branch/jump behavior |
| `test_memory.asm` | Memory operations | LW, SW | Memory load/store |

#### 🎯 Verified Functionality Examples

**1. Basic Arithmetic (`gui_demo_test.asm`)**
```mips
# Load two numbers and perform operations
addi $t0, $zero, 15     # $t0 = 15
addi $t1, $zero, 25     # $t1 = 25
add $t2, $t0, $t1       # $t2 = 40 (15+25)
sub $t3, $t1, $t0       # $t3 = 10 (25-15)

# Store results in memory
sw $t2, 0($zero)        # Store 40 at address 0
sw $t3, 4($zero)        # Store 10 at address 4

# Print results via syscalls
addi $v0, $zero, 1      # print_int syscall
add $a0, $zero, $t2     # Print 40
syscall
add $a0, $zero, $t3     # Print 10  
syscall
```
**Expected Results**: 
- Register $t2 = 40, $t3 = 10
- Memory[0] = 40, Memory[4] = 10
- Console output: "40" "10"

**2. System Call Demo (`demo_syscalls.asm`)**
```mips
# Print integer 42
addi $v0, $zero, 1      # syscall 1: print_int
addi $a0, $zero, 42     
syscall                 # Outputs: "42"

# Read integer from user
addi $v0, $zero, 5      # syscall 5: read_int
syscall                 # Waits for user input
add $t0, $zero, $v0     # Store input in $t0

# Exit program
addi $v0, $zero, 10     # syscall 10: exit
syscall
```
**Expected Results**:
- Prints "42" to console
- Prompts for integer input
- Stores input value in $t0
- Program exits cleanly

**3. Control Flow Testing (`test_control.asm`)**
```mips
addi $t0, $zero, 5      # $t0 = 5
addi $t1, $zero, 5      # $t1 = 5
beq $t0, $t1, equal     # Branch taken (5 == 5)
addi $t2, $zero, 99     # Should be skipped
equal:
addi $t2, $zero, 42     # $t2 = 42
```
**Expected Results**:
- Branch is taken (PC jumps to 'equal' label)
- Register $t2 = 42 (not 99)
- Verifies branch condition logic

### 🔬 How to Verify Each Feature

#### Using GUI Mode:
1. **Launch GUI**: `.\build\src\mips-sim-gui.exe`
2. **Load Demo**: File → Load Demo Program
3. **Execute**: Click "Execute" button or press F5
4. **Observe**: Watch register values, memory content, and console output update in real-time
5. **Debug**: Use "Step" button for instruction-by-instruction execution

#### Using CLI Mode:
1. **Copy test file**: `Copy-Item asmtest\gui_demo_test.asm test_program.asm`
2. **Run simulator**: `.\build\src\mips-sim.exe` 
3. **Verify output**: Check console for expected results

#### Running Unit Tests:
```powershell
# Run all tests
ctest --test-dir build --output-on-failure

# Run specific test categories
ctest --test-dir build -R "CoreInstructions"
ctest --test-dir build -R "Pipeline"
ctest --test-dir build -R "Syscall"
```

## 📖 Additional Resources

**For detailed information, see the complete documentation:**
- 📚 **[docs/README_DOCS.md](docs/README_DOCS.md)** - Complete documentation index
- 🎮 **[docs/user-guides/GUI_USER_MANUAL.md](docs/user-guides/GUI_USER_MANUAL.md)** - GUI usage guide  
- 🔧 **[docs/user-guides/QUICK_REFERENCE.md](docs/user-guides/QUICK_REFERENCE.md)** - Quick reference
- 📊 **[docs/CURRENT_PROJECT_STATUS_2025.md](docs/CURRENT_PROJECT_STATUS_2025.md)** - Project status

## 🎯 Project Purpose

A cycle-accurate educational MIPS simulator with modern GUI for learning computer architecture:

- **Educational Focus**: Designed for students learning MIPS assembly and CPU architecture
- **Visual Learning**: Real-time pipeline visualization and register/memory monitoring
- **Complete Implementation**: 8 MIPS instructions with full 5-stage pipeline support
- **Interactive Debugging**: Step-by-step execution with immediate feedback

## 🏗️ Architecture Overview

**Core Components:**
- **CPU**: 5-stage pipeline (IF → ID → EX → MEM → WB) with dual execution modes
- **Memory**: 4KB word-aligned memory system
- **Assembler**: Two-pass assembler with label support
- **GUI**: Dear ImGui interface with SDL2/OpenGL backend

**Supported Instructions:**
- **R-Type**: `add`, `sub`
- **I-Type**: `addi`, `lw`, `sw`, `beq`  
- **J-Type**: `j`
- **System**: `syscall` (print_int, print_string, read_int, exit)

---

**🎉 Ready to use! Start with the GUI for the best experience, or explore the documentation for detailed information.**
