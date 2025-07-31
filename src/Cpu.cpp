#include "Cpu.h"
#include "RegisterFile.h"
#include "Memory.h"
#include "Assembler.h"
#include "Instruction.h"
#include <string>
#include <cctype>

namespace mips {

Cpu::Cpu() 
    : m_registerFile(std::make_unique<RegisterFile>())
    , m_memory(std::make_unique<Memory>())
    , m_cycleCount(0)
    , m_pc(0)
    , m_pipelineMode(false) // Default to single-cycle mode
    , m_terminated(false)
    , m_inputPosition(0)
{
}

Cpu::~Cpu() = default;

void Cpu::tick() {
    // Check if program should terminate
    if (m_terminated) {
        return;
    }
    
    // For now, only single-cycle execution
    // Pipeline support will be added later
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
    m_terminated = false;
    m_consoleOutput.clear();
    m_consoleInput.clear();
    m_inputPosition = 0;
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

void Cpu::setPipelineMode(bool enabled) {
    // For now, this is a placeholder
    // Pipeline implementation will be added later
    m_pipelineMode = enabled;
}

bool Cpu::isPipelineMode() const {
    return m_pipelineMode;
}

void Cpu::printInt(uint32_t value) {
    m_consoleOutput += std::to_string(value);
}

void Cpu::printString(const std::string& str) {
    m_consoleOutput += str;
}

uint32_t Cpu::readInt() {
    // Find next integer in input buffer
    while (m_inputPosition < m_consoleInput.length()) {
        // Skip non-digit characters
        if (!std::isdigit(m_consoleInput[m_inputPosition]) && 
            m_consoleInput[m_inputPosition] != '-') {
            m_inputPosition++;
            continue;
        }
        
        // Parse integer
        size_t endPos;
        int value = std::stoi(m_consoleInput.substr(m_inputPosition), &endPos);
        m_inputPosition += endPos;
        return static_cast<uint32_t>(value);
    }
    
    // If no more input, return 0
    return 0;
}

void Cpu::terminate() {
    m_terminated = true;
}

bool Cpu::shouldTerminate() const {
    return m_terminated;
}

const std::string& Cpu::getConsoleOutput() const {
    return m_consoleOutput;
}

void Cpu::setConsoleInput(const std::string& input) {
    m_consoleInput = input;
    m_inputPosition = 0;
}

} // namespace mips
