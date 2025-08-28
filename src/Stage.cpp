#include "Stage.h"
#include "Instruction.h"
#include <iostream>

namespace mips
{

PipelineRegister::PipelineRegister() : m_isBubble(true)
{
    m_currentData.reset();
    m_nextData.reset();
}

bool PipelineRegister::isBubble() const
{
    return m_isBubble;
}

void PipelineRegister::setBubble()
{
    m_isBubble = true;
    m_nextData.reset();
}

void PipelineRegister::reset()
{
    m_isBubble = true;
    m_currentData.reset();
    m_nextData.reset();
}

const PipelineData& PipelineRegister::getData() const
{
    return m_currentData;
}

void PipelineRegister::setData(const PipelineData& data)
{
    m_nextData = data;
    m_isBubble = false;
}

void PipelineRegister::clockUpdate()
{
    m_currentData = m_nextData;
    if (m_currentData.pc >= 30 && m_currentData.pc <= 140)
    {
        // Log when pipeline register transfers PC in loop region
        std::cerr << "DEBUG: PipelineRegister::clockUpdate pc=" << m_currentData.pc << " instr='"
                  << (m_currentData.instruction ? m_currentData.instruction->getName() : "<bubble>")
                  << "'" << std::endl;
    }
    if (m_nextData.instruction == nullptr)
    {
        m_isBubble = true;
    }
    else
    {
        m_isBubble = false;
    }
}

}  // namespace mips
