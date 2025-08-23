#include "MEMStage.h"
#include "Cpu.h"
#include "Memory.h"
#include "Stage.h"

namespace mips
{

MEMStage::MEMStage(Cpu* cpu) : m_inputRegister(nullptr), m_outputRegister(nullptr)
{
    m_cpu = cpu;
}

void MEMStage::execute()
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

    // Perform memory operations
    if (data.memRead)
    {
        // Load operation
        data.memoryData = m_cpu->getMemory().readWord(data.aluResult);
    }
    else if (data.memWrite)
    {
        // Store operation
        m_cpu->getMemory().writeWord(data.aluResult, data.rtValue);
    }

    // Pass data to next stage
    m_outputRegister->setData(data);
}

void MEMStage::reset()
{
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void MEMStage::flush()
{
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void MEMStage::setInputRegister(PipelineRegister* inputReg)
{
    m_inputRegister = inputReg;
}

void MEMStage::setOutputRegister(PipelineRegister* outputReg)
{
    m_outputRegister = outputReg;
}

} // namespace mips
