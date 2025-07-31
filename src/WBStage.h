#pragma once

#include "Stage.h"

namespace mips {

class Cpu;
class PipelineRegister;

/**
 * @brief Write Back (WB) stage
 * 
 * Responsibilities:
 * - Write results back to register file
 * - Complete instruction execution
 */
class WBStage : public Stage {
public:
    explicit WBStage(Cpu* cpu);
    ~WBStage() override = default;
    
    /**
     * @brief Execute WB stage logic
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
     * @brief Set input pipeline register
     */
    void setInputRegister(PipelineRegister* inputReg);

private:
    PipelineRegister* m_inputRegister;
};

} // namespace mips
