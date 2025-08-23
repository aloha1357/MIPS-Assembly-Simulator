#include "IFStage.h"
#include "Cpu.h"
#include "Instruction.h"
#include "Stage.h"
#include <iostream>

namespace mips
{

IFStage::IFStage(Cpu* cpu)
    : m_outputRegister(nullptr), m_instructions(nullptr), m_pcUpdateEnable(true), m_flushed(false)
{
    m_cpu = cpu;
}

void IFStage::execute()
{
    // std::cout << "IF: execute() called with PC=" << m_cpu->getProgramCounter() << std::endl;

    if (m_flushed)
    {
        // Insert bubble if flushed
        if (m_outputRegister)
        {
            m_outputRegister->setBubble();
        }
        m_flushed = false;
        return;
    }

    if (!m_pcUpdateEnable)
    {
        // Stalled - don't fetch new instruction
        return;
    }

    if (!m_instructions || !m_outputRegister)
    {
        return;
    }

    uint32_t pc = m_cpu->getProgramCounter();

    // Check if PC is within instruction memory bounds
    if (pc >= m_instructions->size())
    {
        // std::cout << "IF: PC=" << pc << " >= size=" << m_instructions->size() << ", inserting
        // bubble" << std::endl;
        m_outputRegister->setBubble();
        return;
    }

    // Fetch instruction
    const auto& instruction = (*m_instructions)[pc];
    if (!instruction)
    {
        m_outputRegister->setBubble();
        return;
    }

    // Create pipeline data
    PipelineData data;
    data.instruction = instruction.get(); // Get raw pointer from unique_ptr
    data.pc          = pc;

    // Debug output
    // std::cout << "IF: Fetching instruction at PC=" << pc << " name=" << instruction->getName() <<
    // std::endl;

    // Set data to output register
    m_outputRegister->setData(data);
}

void IFStage::reset()
{
    m_pcUpdateEnable = true;
    m_flushed        = false;
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void IFStage::flush()
{
    m_flushed = true;
    if (m_outputRegister)
    {
        m_outputRegister->setBubble();
    }
}

void IFStage::setOutputRegister(PipelineRegister* outputReg)
{
    m_outputRegister = outputReg;
}

void IFStage::setInstructions(const std::vector<std::unique_ptr<Instruction>>* instructions)
{
    m_instructions = instructions;
}

bool IFStage::canUpdatePC() const
{
    return m_pcUpdateEnable && !m_flushed;
}

void IFStage::setPCUpdateEnable(bool enable)
{
    m_pcUpdateEnable = enable;
}

} // namespace mips
