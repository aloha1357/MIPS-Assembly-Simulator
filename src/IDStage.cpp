#include "IDStage.h"
#include "Cpu.h"
#include "Instruction.h"
#include "RegisterFile.h"
#include "Stage.h"

namespace mips
{

IDStage::IDStage(Cpu* cpu) : m_inputRegister(nullptr), m_outputRegister(nullptr), m_stalled(false)
{
    m_cpu = cpu;
}

void IDStage::execute()
{
    if (!m_inputRegister || !m_outputRegister)
    {
        return;
    }

    // Check if input contains a bubble
    if (m_inputRegister->isBubble())
    {
        m_outputRegister->setBubble();
        return;
    }

    // Get input data
    PipelineData data = m_inputRegister->getData();

    // Check for load-use hazard
    if (detectLoadUseHazard(data))
    {
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

void IDStage::reset()
{
    m_stalled = false;
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void IDStage::flush()
{
    m_stalled = false;
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void IDStage::setInputRegister(PipelineRegister* inputReg)
{
    m_inputRegister = inputReg;
}

void IDStage::setOutputRegister(PipelineRegister* outputReg)
{
    m_outputRegister = outputReg;
}

bool IDStage::shouldStall() const
{
    return m_stalled;
}

void IDStage::decodeInstruction(PipelineData& data)
{
    // For now, we'll use a simplified approach since our instructions
    // already contain the decoded information
    const std::string& name = data.instruction->getName();

    if (name == "add" || name == "sub")
    {
        // R-type instruction format
        data.opcode = 0;  // R-type has opcode 0
        // Note: Real implementation would decode from 32-bit instruction word
        // For now, we'll extract info from our instruction objects
    }
    else if (name == "addi" || name == "lw" || name == "sw")
    {
        // I-type instruction format
        data.opcode = (name == "addi") ? 8 : ((name == "lw") ? 35 : 43);
    }
    else if (name == "beq")
    {
        // Branch instruction
        data.opcode = 4;
    }
    else if (name == "j")
    {
        // Jump instruction
        data.opcode = 2;
    }
}

void IDStage::generateControlSignals(PipelineData& data)
{
    const std::string& name = data.instruction->getName();

    // Set default values
    data.regWrite = false;
    data.memRead  = false;
    data.memWrite = false;
    data.branch   = false;
    data.jump     = false;
    data.aluSrc   = false;
    data.regDst   = 0;
    data.memToReg = 0;

    if (name == "add" || name == "sub")
    {
        // R-type arithmetic
        data.regWrite = true;
        data.regDst   = 1;  // Write to rd
        data.memToReg = 0;  // Use ALU result
    }
    else if (name == "addi")
    {
        // I-type arithmetic
        data.regWrite = true;
        data.aluSrc   = true;  // Use immediate
        data.regDst   = 0;     // Write to rt
        data.memToReg = 0;     // Use ALU result
    }
    else if (name == "lw")
    {
        // Load word
        data.regWrite = true;
        data.memRead  = true;
        data.aluSrc   = true;  // Use immediate for address calculation
        data.regDst   = 0;     // Write to rt
        data.memToReg = 1;     // Use memory data
    }
    else if (name == "sw")
    {
        // Store word
        data.memWrite = true;
        data.aluSrc   = true;  // Use immediate for address calculation
    }
    else if (name == "beq")
    {
        // Branch equal
        data.branch = true;
    }
    else if (name == "j")
    {
        // Jump
        data.jump = true;
    }
}

void IDStage::readRegisters(PipelineData& data)
{
    if (!data.instruction || !m_cpu)
    {
        return;
    }

    // For now, we'll use a simple approach to extract register information
    // In a real implementation, we'd decode rs/rt/rd from the 32-bit instruction word

    const std::string& name = data.instruction->getName();

    // We need to execute the instruction to get the actual values
    // This is a simplified approach - in reality we'd decode the instruction format
    // For now, let's use the instruction's execute method to get the values we need

    // Since our current instruction system is designed for single-cycle execution,
    // we'll need to adapt it for pipeline use. For now, let's set some basic values:

    if (name == "add" || name == "sub")
    {
        // R-type: rs and rt are source registers
        data.rsValue = 0;   // Would read from register rs
        data.rtValue = 0;   // Would read from register rt
        data.rd      = 10;  // Destination register (example: $t2)
    }
    else if (name == "addi")
    {
        // I-type: rs is source, rt is destination
        data.rsValue   = 0;   // Would read from register rs
        data.immediate = 10;  // Example immediate value
        data.rt        = 8;   // Destination register (example: $t0)
    }
    else if (name == "lw" || name == "sw")
    {
        // Memory operations
        data.rsValue   = 0;    // Base register value
        data.rtValue   = 100;  // Data to store (for sw)
        data.immediate = 0;    // Offset
        data.rt        = 9;    // Target register
    }
}

bool IDStage::detectLoadUseHazard(const PipelineData& data)
{
    // Simplified hazard detection
    // TODO: Implement proper load-use hazard detection
    // This would check if the previous instruction is a load and this instruction uses its result
    (void)data;  // Suppress unused parameter warning
    return false;
}

}  // namespace mips
