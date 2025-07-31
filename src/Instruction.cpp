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

} // namespace mips
