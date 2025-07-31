#include "Instruction.h"
#include "Cpu.h"
#include "RegisterFile.h"
#include "Memory.h"

namespace mips {

RTypeInstruction::RTypeInstruction(int rd, int rs, int rt) 
    : m_rd(rd), m_rs(rs), m_rt(rt) {
}

AddInstruction::AddInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void AddInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue + rtValue;
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AddInstruction::getName() const {
    return "add";
}

SubInstruction::SubInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void SubInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue - rtValue;
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SubInstruction::getName() const {
    return "sub";
}

ITypeInstruction::ITypeInstruction(int rt, int rs, int16_t imm)
    : m_rt(rt), m_rs(rs), m_imm(imm) {
}

uint32_t ITypeInstruction::signExtend16(int16_t value) {
    return static_cast<uint32_t>(static_cast<int32_t>(value));
}

AddiInstruction::AddiInstruction(int rt, int rs, int16_t imm)
    : ITypeInstruction(rt, rs, imm) {
}

void AddiInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t immValue = signExtend16(m_imm);
    uint32_t result = rsValue + immValue;
    
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AddiInstruction::getName() const {
    return "addi";
}

LwInstruction::LwInstruction(int rt, int rs, int16_t offset)
    : ITypeInstruction(rt, rs, offset) {
}

void LwInstruction::execute(Cpu& cpu) {
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset = signExtend16(m_imm);
    uint32_t address = baseAddress + offset;
    
    uint32_t value = cpu.getMemory().readWord(address);
    cpu.getRegisterFile().write(m_rt, value);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LwInstruction::getName() const {
    return "lw";
}

SwInstruction::SwInstruction(int rt, int rs, int16_t offset)
    : ITypeInstruction(rt, rs, offset) {
}

void SwInstruction::execute(Cpu& cpu) {
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset = signExtend16(m_imm);
    uint32_t address = baseAddress + offset;
    
    uint32_t value = cpu.getRegisterFile().read(m_rt);
    cpu.getMemory().writeWord(address, value);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SwInstruction::getName() const {
    return "sw";
}

BranchInstruction::BranchInstruction(int rs, int rt, const std::string& label)
    : m_rs(rs), m_rt(rt), m_label(label) {
}

BeqInstruction::BeqInstruction(int rs, int rt, const std::string& label)
    : BranchInstruction(rs, rt, label) {
}

void BeqInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    
    if (rsValue == rtValue) {
        // Branch taken - jump to label
        uint32_t targetAddress = cpu.getLabelAddress(m_label);
        cpu.setProgramCounter(targetAddress);
    } else {
        // Branch not taken - continue with next instruction
        cpu.setProgramCounter(cpu.getProgramCounter() + 1);
    }
}

std::string BeqInstruction::getName() const {
    return "beq";
}

JInstruction::JInstruction(const std::string& label)
    : m_label(label) {
}

void JInstruction::execute(Cpu& cpu) {
    // Unconditional jump to label
    uint32_t targetAddress = cpu.getLabelAddress(m_label);
    cpu.setProgramCounter(targetAddress);
}

std::string JInstruction::getName() const {
    return "j";
}

SllInstruction::SllInstruction(uint32_t rd, uint32_t rt, uint32_t shamt) 
    : m_rd(rd), m_rt(rt), m_shamt(shamt) {
}

void SllInstruction::execute(Cpu& cpu) {
    // Read source register value
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    
    // Perform left shift
    uint32_t result = rtValue << m_shamt;
    
    // Write result to destination register
    cpu.getRegisterFile().write(m_rd, result);
    
    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SllInstruction::getName() const {
    return "sll";
}

SyscallInstruction::SyscallInstruction() {
}

void SyscallInstruction::execute(Cpu& cpu) {
    // System call number is in $v0 (register 2)
    uint32_t syscallNumber = cpu.getRegisterFile().read(2); // $v0
    
    switch (syscallNumber) {
        case 1:
            handlePrintInt(cpu);
            break;
        case 4:
            handlePrintString(cpu);
            break;
        case 5:
            handleReadInt(cpu);
            break;
        case 10:
            handleExit(cpu);
            break;
        default:
            // Unknown system call - just continue execution
            break;
    }
    
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

void SyscallInstruction::handlePrintInt(Cpu& cpu) {
    // Integer to print is in $a0 (register 4)
    uint32_t value = cpu.getRegisterFile().read(4); // $a0
    cpu.printInt(value);
}

void SyscallInstruction::handlePrintString(Cpu& cpu) {
    // String address is in $a0 (register 4)
    uint32_t stringAddress = cpu.getRegisterFile().read(4); // $a0
    
    // Read string from memory character by character until null terminator
    std::string str;
    uint32_t currentAddr = stringAddress;
    
    try {
        while (true) {
            uint32_t word = cpu.getMemory().readWord(currentAddr);
            
            // Extract bytes from word (little-endian)
            for (int i = 0; i < 4; i++) {
                char c = (word >> (i * 8)) & 0xFF;
                if (c == '\0') {
                    cpu.printString(str);
                    return;
                }
                str += c;
            }
            currentAddr += 4;
        }
    } catch (...) {
        // Memory access failed, just print what we have
        cpu.printString(str);
    }
}

void SyscallInstruction::handleReadInt(Cpu& cpu) {
    // Read integer from input and store in $v0
    uint32_t value = cpu.readInt();
    cpu.getRegisterFile().write(2, value); // $v0
}

void SyscallInstruction::handleExit(Cpu& cpu) {
    // Set termination flag in CPU
    cpu.terminate();
}

std::string SyscallInstruction::getName() const {
    return "syscall";
}

} // namespace mips
