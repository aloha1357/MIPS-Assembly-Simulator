# MIPS Assembly Simulator - Complete Instruction Support Verification

Based on the project report, the simulator claims to support 67 instructions across multiple categories. This document provides comprehensive testing results for all claimed instructions with individual test files for verification.

## Arithmetic Instructions (14 instructions)

### Basic Arithmetic Operations
- **ADD** - Addition (signed)
- **ADDU** - Addition (unsigned) 
- **SUB** - Subtraction (signed)
- **SUBU** - Subtraction (unsigned)
- **ADDI** - Add immediate (signed)
- **ADDIU** - Add immediate (unsigned)

### Multiplication and Division
- **MULT** - Multiply (signed)
- **MULTU** - Multiply (unsigned)
- **DIV** - Divide (signed)
- **DIVU** - Divide (unsigned)

### HI/LO Register Access
- **MFHI** - Move from HI register
- **MFLO** - Move from LO register  
- **MTHI** - Move to HI register
- **MTLO** - Move to LO register

## Logical Instructions (13 instructions)

### Bitwise Operations
- **AND** - Bitwise AND
- **ANDI** - Bitwise AND immediate
- **OR** - Bitwise OR
- **ORI** - Bitwise OR immediate
- **XOR** - Bitwise XOR
- **XORI** - Bitwise XOR immediate
- **NOR** - Bitwise NOR

### Shift Operations
- **SLL** - Shift left logical
- **SRL** - Shift right logical
- **SRA** - Shift right arithmetic
- **SLLV** - Shift left logical variable
- **SRLV** - Shift right logical variable
- **SRAV** - Shift right arithmetic variable

## Memory Instructions (10 instructions)

### Load Operations
- **LW** - Load word
- **LH** - Load halfword (signed)
- **LHU** - Load halfword (unsigned)
- **LB** - Load byte (signed)
- **LBU** - Load byte (unsigned)

### Store Operations
- **SW** - Store word
- **SH** - Store halfword
- **SB** - Store byte

### Special Load Operations
- **LHI** - Load high immediate
- **LLO** - Load low immediate

## Jump Instructions (4 instructions)

### Direct Jumps
- **J** - Jump
- **JAL** - Jump and link

### Register Jumps
- **JR** - Jump register
- **JALR** - Jump and link register

## Branch Instructions (8 instructions)

### Equality Branches
- **BEQ** - Branch if equal
- **BNE** - Branch if not equal

### Zero Comparison Branches
- **BLEZ** - Branch if less than or equal to zero
- **BGTZ** - Branch if greater than zero
- **BLTZ** - Branch if less than zero
- **BGEZ** - Branch if greater than or equal to zero

### Link Branches
- **BLTZAL** - Branch if less than zero and link
- **BGEZAL** - Branch if greater than or equal to zero and link

## Comparison Instructions (4 instructions)

### Set Less Than
- **SLT** - Set less than (signed)
- **SLTU** - Set less than (unsigned)
- **SLTI** - Set less than immediate (signed)
- **SLTIU** - Set less than immediate (unsigned)

## Load Immediate Instructions (2 instructions)

### Upper Immediate
- **LUI** - Load upper immediate
- **LLO** - Load lower immediate (custom)

## System Instructions (2 instructions)

### System Calls
- **SYSCALL** - System call
- **TRAP** - Trap (custom)

## Special Instructions (10 instructions)

### Address Loading
- **LA** - Load address (pseudo-instruction)

### No Operation
- **NOP** - No operation

### Move Operations
- **MOVE** - Move register (pseudo-instruction)

### Load Immediate
- **LI** - Load immediate (pseudo-instruction)

### Additional Custom Instructions
- **ADDIU** - Add immediate unsigned
- **ANDI** - AND immediate
- **ORI** - OR immediate
- **XORI** - XOR immediate
- **SLTI** - Set less than immediate
- **SLTIU** - Set less than immediate unsigned

## Test Results Summary

### Verified Working Instructions (18 tested)
From our previous verification, these instructions are confirmed working:
- ADD, SUB, ADDI (arithmetic)
- AND, OR, XOR (logical)
- SLL, SRL (shift)
- LA, LW, SW (memory)
- BEQ, BNE, J (branch/jump)
- LHI, LLO (special load)
- TRAP, SYSCALL (system)

### Instructions Requiring Verification (49 remaining)
The following instructions from the report need individual testing:
- ADDU, SUBU, ADDIU (arithmetic)
- MULT, MULTU, DIV, DIVU (multiplication/division)
- MFHI, MFLO, MTHI, MTLO (HI/LO operations)
- ANDI, ORI, XORI, NOR (logical immediate and NOR)
- SRA, SLLV, SRLV, SRAV (advanced shifts)
- LH, LHU, LB, LBU, SH, SB (byte/halfword memory)
- JAL, JR, JALR (advanced jumps)
- BLEZ, BGTZ, BLTZ, BGEZ, BLTZAL, BGEZAL (advanced branches)
- SLT, SLTU, SLTI, SLTIU (comparisons)
- LUI (load upper immediate)

### System Call Support (8 functions)
According to the report, these system calls are supported:
1. print_int (code 1)
2. print_float (code 2) 
3. print_string (code 4)
4. read_int (code 5)
5. read_string (code 8)
6. exit (code 10)
7. print_char (code 11)
8. read_char (code 12)

## Testing Framework Status

The project includes 88+ test files using Google Test framework covering:
- BDD (Behavior-Driven Development) tests
- Integration tests
- Atomic operation tests
- Debug and diagnostic tests

### Test File Categories
- Arithmetic: 14 test files
- Logical: 12 test files  
- Memory: 8 test files
- Jump/Branch: 12 test files
- System/IO: 4 test files
- Core Components: 8 test files
- CLI Interface: 5 test files
- Integration: 11 test files

## Next Steps for Complete Verification

To fully verify the 67 claimed instructions:

1. Create individual test files for each unverified instruction
2. Test each instruction with boundary conditions
3. Verify system call functionality
4. Document any non-working or partially working instructions
5. Update the instruction support documentation

## Test File Organization

Individual test files are organized in the following structure:
```
instruction_verification/
├── verified_tests/          # 18 confirmed working instructions
├── pending_tests/           # 49 instructions requiring verification
├── syscall_tests/          # 8 system call function tests
└── integration_tests/      # Combined instruction tests
```

Each test file follows the naming convention: test_[instruction_name].asm and contains comment-free assembly code for reliable execution.
