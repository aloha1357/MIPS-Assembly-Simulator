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
        m_instructions[m_pc]->execute(*this);
        m_pc++;
    }
    m_cycleCount++;
}

void Cpu::loadProgramFromString(const std::string& assembly) {
    Assembler assembler;
    m_instructions = assembler.assemble(assembly);
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
    m_registerFile->reset();
    m_memory->reset();
}

} // namespace mips
