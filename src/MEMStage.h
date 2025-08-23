#pragma once

#include "Stage.h"

namespace mips
{

class Cpu;
class PipelineRegister;

/**
 * @brief Memory (MEM) stage
 *
 * Responsibilities:
 * - Perform memory read/write operations
 * - Pass ALU results through
 */
class MEMStage : public Stage
{
  public:
    explicit MEMStage(Cpu* cpu);
    ~MEMStage() override = default;

    /**
     * @brief Execute MEM stage logic
     */
    void execute() override;

    /**
     * @brief Reset stage state
     */
    void reset() override;

    /**
     * @brief Flush stage (for branch misprediction)
     */
    void flush() override;

    /**
     * @brief Set input and output pipeline registers
     */
    void setInputRegister(PipelineRegister* inputReg);
    void setOutputRegister(PipelineRegister* outputReg);

  private:
    PipelineRegister* m_inputRegister;
    PipelineRegister* m_outputRegister;
};

}  // namespace mips
