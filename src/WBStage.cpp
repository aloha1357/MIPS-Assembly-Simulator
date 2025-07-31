#include "WBStage.h"
#include "Cpu.h"
#include "RegisterFile.h"
#include "Instruction.h"
#include "Stage.h"

namespace mips {

WBStage::WBStage(Cpu* cpu) 
    : m_inputRegister(nullptr) {
    m_cpu = cpu;
}

void WBStage::execute() {
    if (!m_inputRegister) {
        return;
    }
    
    // Check if input contains a bubble
    if (m_inputRegister->isBubble()) {
        return;
    }
    
    // Get input data
    const PipelineData& data = m_inputRegister->getData();
    
    // For simplicity in this implementation, let's execute the instruction directly
    // This bridges our single-cycle instruction model with the pipeline architecture
    if (data.instruction && m_cpu) {
        // Execute the instruction directly
        data.instruction->execute(*m_cpu);
    }
    
    // Legacy register file write-back logic (commented out for now)
    /*
    if (data.regWrite) {
        uint32_t writeData;
        uint32_t writeRegister;
        
        // Select data source
        if (data.memToReg == 1) {
            writeData = data.memoryData; // From memory
        } else {
            writeData = data.aluResult; // From ALU
        }
        
        // Select destination register
        if (data.regDst == 1) {
            writeRegister = data.rd; // R-type instructions
        } else {
            writeRegister = data.rt; // I-type instructions
        }
        
        // Write to register file
        m_cpu->getRegisterFile().write(writeRegister, writeData);
    }
    */
}

void WBStage::reset() {
    // Nothing to reset in WB stage
}

void WBStage::flush() {
    // Nothing to flush in WB stage - instruction completes normally
}

void WBStage::setInputRegister(PipelineRegister* inputReg) {
    m_inputRegister = inputReg;
}

} // namespace mips
