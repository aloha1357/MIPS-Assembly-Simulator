#include "EXStage.h"
#include "Cpu.h"
#include "Stage.h"

namespace mips
{

EXStage::EXStage(Cpu* cpu) : m_inputRegister(nullptr), m_outputRegister(nullptr)
{
    m_cpu = cpu;
}

void EXStage::execute()
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

    // Perform ALU operation
    data.aluResult = performALUOperation(data);

    // Handle branch/jump logic
    if (data.branch)
    {
        uint32_t branchTarget = calculateBranchTarget(data);
        if (shouldTakeBranch(data))
        {
            // Branch taken - update PC and flush pipeline
            m_cpu->setProgramCounter(branchTarget);
            // TODO: Signal pipeline flush
        }
    }
    else if (data.jump)
    {
        // Unconditional jump
        uint32_t jumpTarget = calculateBranchTarget(data);
        m_cpu->setProgramCounter(jumpTarget);
        // TODO: Signal pipeline flush
    }

    // Pass data to next stage
    m_outputRegister->setData(data);
}

void EXStage::reset()
{
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void EXStage::flush()
{
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void EXStage::setInputRegister(PipelineRegister* inputReg)
{
    m_inputRegister = inputReg;
}

void EXStage::setOutputRegister(PipelineRegister* outputReg)
{
    m_outputRegister = outputReg;
}

uint32_t EXStage::performALUOperation(const PipelineData& data)
{
    if (!data.instruction)
    {
        return 0;
    }

    const std::string& name = data.instruction->getName();

    if (name == "add" || name == "addi")
    {
        return data.rsValue + (data.aluSrc ? data.immediate : data.rtValue);
    }
    else if (name == "sub")
    {
        return data.rsValue - data.rtValue;
    }
    else if (name == "lw" || name == "sw")
    {
        // Calculate memory address
        return data.rsValue + data.immediate;
    }

    return 0;
}

uint32_t EXStage::calculateBranchTarget(const PipelineData& data)
{
    if (data.branch)
    {
        // Branch target = PC + 1 + immediate (relative addressing)
        return data.pc + 1 + data.immediate;
    }
    else if (data.jump)
    {
        // Jump target is handled by instruction itself
        // For now, return current PC (will be updated by instruction execution)
        return data.pc;
    }

    return data.pc + 1;
}

bool EXStage::shouldTakeBranch(const PipelineData& data)
{
    if (!data.branch || !data.instruction)
    {
        return false;
    }

    const std::string& name = data.instruction->getName();
    if (name == "beq")
    {
        return data.rsValue == data.rtValue;
    }

    return false;
}

}  // namespace mips
