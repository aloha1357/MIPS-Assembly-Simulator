#include "Cpu.h"
#include "RegisterFile.h"
#include "Memory.h"
#include "Assembler.h"
#include "Instruction.h"

namespace mips {

Cpu::Cpu() 
    : m_registerFile(std::make_unique<RegisterFile>())
    , m_memory(std::make_unique<Memory>())
    , m_cycleCount(0)
    , m_pc(0)
{
    // TODO: Initialize pipeline stages and registers
}

Cpu::~Cpu() = default;

void Cpu::tick() {
    // Simple single-cycle execution for now
    if (m_pc < m_instructions.size()) {
        uint32_t oldPc = m_pc;
        m_instructions[m_pc]->execute(*this);
        
        // Only increment PC if instruction didn't change it (for non-branch instructions)
        if (m_pc == oldPc) {
            m_pc++;
        }
    }
    m_cycleCount++;
}

void Cpu::loadProgramFromString(const std::string& assembly) {
    Assembler assembler;
    m_instructions = assembler.assembleWithLabels(assembly, m_labelMap);
    m_pc = 0;
}

void Cpu::loadProgram(const std::string& path) {
    // TODO: Implement file loading
    (void)path; // Suppress unused parameter warning
}

void Cpu::run(int cycles) {
    for (int i = 0; i < cycles; ++i) {
        tick();
    }
}

RegisterFile& Cpu::getRegisterFile() {
    return *m_registerFile;
}

Memory& Cpu::getMemory() {
    return *m_memory;
}

int Cpu::getCycleCount() const {
    return m_cycleCount;
}

void Cpu::reset() {
    m_cycleCount = 0;
    m_pc = 0;
    m_instructions.clear();
    m_labelMap.clear();
    m_registerFile->reset();
    m_memory->reset();
}

void Cpu::setProgramCounter(uint32_t pc) {
    m_pc = pc;
}

uint32_t Cpu::getProgramCounter() const {
    return m_pc;
}

uint32_t Cpu::getLabelAddress(const std::string& label) const {
    auto it = m_labelMap.find(label);
    if (it != m_labelMap.end()) {
        return it->second;
    }
    return 0; // Default to address 0 if label not found
}

} // namespace mips
