#include "WBStage.h"
#include "Cpu.h"
#include "Instruction.h"
#include "RegisterFile.h"
#include "Stage.h"
#include <iostream>

namespace mips
{

WBStage::WBStage(Cpu* cpu) : m_inputRegister(nullptr)
{
    m_cpu = cpu;
}

void WBStage::execute()
{
    if (!m_inputRegister)
    {
        return;
    }

    // Check if input contains a bubble
    if (m_inputRegister->isBubble())
    {
        return;
    }

    // Get input data
    const PipelineData& data = m_inputRegister->getData();

    // For simplicity in this implementation, let's execute the instruction directly
    // This bridges our single-cycle instruction model with the pipeline architecture
    if (data.instruction && m_cpu)
    {
        // Debug output
        // std::cout << "WB: Executing " << data.instruction->getName() << std::endl;

        // In pipeline mode, we need to prevent most instructions from updating PC
        // Save current PC, execute instruction, then conditionally restore PC
        uint32_t savedPC = m_cpu->getProgramCounter();

        // Execute the instruction directly
        data.instruction->execute(*m_cpu);

        // For pipeline mode we only want to keep instruction-updated PC when
        // the instruction performed a control-flow change. Most instructions
        // will simply increment PC by 1; in that case restore the saved PC so
        // the pipeline can manage the PC updates. If the instruction set PC
        // to something else (e.g., jr/jal/jalr/branches), leave it.
        uint32_t afterPC = m_cpu->getProgramCounter();

        // Targeted logging for the loop region to help debug unexpected jumps
        if (savedPC >= 30 && savedPC <= 140)
        {
            std::cerr << "DEBUG: WBStage exec - instr='" 
                      << (data.instruction ? data.instruction->getName() : "<null>")
                      << "' savedPC=" << savedPC << " afterPC=" << afterPC << std::endl;
        }

        if (afterPC == savedPC + 1)
        {
            // Non-control-flow: restore saved PC so the IF stage increments it
            m_cpu->setProgramCounter(savedPC);
        }
        else
        {
            // Control-flow change detected - log for debugging
            std::cerr << "DEBUG: WBStage control-flow - instr='" 
                      << (data.instruction ? data.instruction->getName() : "<null>")
                      << "' savedPC=" << savedPC << " afterPC=" << afterPC << std::endl;
        }
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

void WBStage::reset()
{
    // Nothing to reset in WB stage
}

void WBStage::flush()
{
    // Nothing to flush in WB stage - instruction completes normally
}

void WBStage::setInputRegister(PipelineRegister* inputReg)
{
    m_inputRegister = inputReg;
}

}  // namespace mips
