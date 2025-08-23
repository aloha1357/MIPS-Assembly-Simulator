#pragma once

#include "Instruction.h" // Include for complete definition
#include "Stage.h"

namespace mips
{

class Cpu;
class PipelineRegister;

/**
 * @brief Execute (EX) stage
 *
 * Responsibilities:
 * - Perform ALU operations
 * - Calculate branch targets
 * - Handle forwarding
 */
class EXStage : public Stage
{
  public:
    explicit EXStage(Cpu* cpu);
    ~EXStage() override = default;

    /**
     * @brief Execute EX stage logic
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

    /**
     * @brief Perform ALU operation
     */
    uint32_t performALUOperation(const PipelineData& data);

    /**
     * @brief Calculate branch target address
     */
    uint32_t calculateBranchTarget(const PipelineData& data);

    /**
     * @brief Check if branch should be taken
     */
    bool shouldTakeBranch(const PipelineData& data);
};

} // namespace mips
