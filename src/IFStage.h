#pragma once

#include "Stage.h"
#include <memory>
#include <vector>

namespace mips
{

class Instruction;
class Cpu;
class PipelineRegister;

/**
 * @brief Instruction Fetch (IF) stage
 *
 * Responsibilities:
 * - Fetch instruction from instruction memory
 * - Update program counter
 * - Handle branch/jump target updates
 */
class IFStage : public Stage
{
  public:
    explicit IFStage(Cpu* cpu);
    ~IFStage() override = default;

    /**
     * @brief Execute IF stage logic
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
     * @brief Set output pipeline register
     */
    void setOutputRegister(PipelineRegister* outputReg);

    /**
     * @brief Set instruction memory
     */
    void setInstructions(const std::vector<std::unique_ptr<Instruction>>* instructions);

    /**
     * @brief Check if PC should be updated (not stalled)
     */
    bool canUpdatePC() const;

    /**
     * @brief Set PC update enable (for stall control)
     */
    void setPCUpdateEnable(bool enable);

  private:
    PipelineRegister*                                m_outputRegister;
    const std::vector<std::unique_ptr<Instruction>>* m_instructions;
    bool                                             m_pcUpdateEnable;
    bool                                             m_flushed;
};

} // namespace mips
