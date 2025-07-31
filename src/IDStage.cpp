#include "IDStage.h"
#include "Cpu.h"
#include "RegisterFile.h"
#include "Instruction.h"
#include "Stage.h"

namespace mips {

IDStage::IDStage(Cpu* cpu) 
    : m_inputRegister(nullptr)
    , m_outputRegister(nullptr)
    , m_stalled(false) {
    m_cpu = cpu;
}

void IDStage::execute() {
    if (!m_inputRegister || !m_outputRegister) {
        return;
    }
    
    // Check if input contains a bubble
    if (m_inputRegister->isBubble()) {
        m_outputRegister->setBubble();
        return;
    }
    
    // Get input data
    PipelineData data = m_inputRegister->getData();
    
    // Check for load-use hazard
    if (detectLoadUseHazard(data)) {
        // Stall: don't advance instruction, insert bubble in output
        m_stalled = true;
        m_outputRegister->setBubble();
        return;
    }
    
    m_stalled = false;
    
    // Decode instruction
    decodeInstruction(data);
    
    // Generate control signals
    generateControlSignals(data);
    
    // Read register values
    readRegisters(data);
    
    // Pass data to next stage
    m_outputRegister->setData(data);
}

void IDStage::reset() {
    m_stalled = false;
    if (m_outputRegister) {
        m_outputRegister->setBubble();
    }
}

void IDStage::flush() {
    m_stalled = false;
    if (m_outputRegister) {
        m_outputRegister->setBubble();
    }
}

void IDStage::setInputRegister(PipelineRegister* inputReg) {
    m_inputRegister = inputReg;
}

void IDStage::setOutputRegister(PipelineRegister* outputReg) {
    m_outputRegister = outputReg;
}

bool IDStage::shouldStall() const {
    return m_stalled;
}

void IDStage::decodeInstruction(PipelineData& data) {
    // For now, we'll use a simplified approach since our instructions
    // already contain the decoded information
    const std::string& name = data.instruction->getName();
    
    if (name == "add" || name == "sub") {
        // R-type instruction format
        data.opcode = 0; // R-type has opcode 0
        // Note: Real implementation would decode from 32-bit instruction word
        // For now, we'll extract info from our instruction objects
    } else if (name == "addi" || name == "lw" || name == "sw") {
        // I-type instruction format
        data.opcode = (name == "addi") ? 8 : ((name == "lw") ? 35 : 43);
    } else if (name == "beq") {
        // Branch instruction
        data.opcode = 4;
    } else if (name == "j") {
        // Jump instruction
        data.opcode = 2;
    }
}

void IDStage::generateControlSignals(PipelineData& data) {
    const std::string& name = data.instruction->getName();
    
    // Set default values
    data.regWrite = false;
    data.memRead = false;
    data.memWrite = false;
    data.branch = false;
    data.jump = false;
    data.aluSrc = false;
    data.regDst = 0;
    data.memToReg = 0;
    
    if (name == "add" || name == "sub") {
        // R-type arithmetic
        data.regWrite = true;
        data.regDst = 1; // Write to rd
        data.memToReg = 0; // Use ALU result
    } else if (name == "addi") {
        // I-type arithmetic
        data.regWrite = true;
        data.aluSrc = true; // Use immediate
        data.regDst = 0; // Write to rt
        data.memToReg = 0; // Use ALU result
    } else if (name == "lw") {
        // Load word
        data.regWrite = true;
        data.memRead = true;
        data.aluSrc = true; // Use immediate for address calculation
        data.regDst = 0; // Write to rt
        data.memToReg = 1; // Use memory data
    } else if (name == "sw") {
        // Store word
        data.memWrite = true;
        data.aluSrc = true; // Use immediate for address calculation
    } else if (name == "beq") {
        // Branch equal
        data.branch = true;
    } else if (name == "j") {
        // Jump
        data.jump = true;
    }
}

void IDStage::readRegisters(PipelineData& data) {
    // This is simplified - in real implementation we'd decode rs/rt from instruction word
    // For now, we'll use placeholder values since our instruction objects handle this
    
    // Read register values (will be properly implemented when we have instruction decoding)
    data.rsValue = 0;
    data.rtValue = 0;
}

bool IDStage::detectLoadUseHazard(const PipelineData& data) {
    // Simplified hazard detection
    // TODO: Implement proper load-use hazard detection
    // This would check if the previous instruction is a load and this instruction uses its result
    (void)data; // Suppress unused parameter warning
    return false;
}

} // namespace mips
