#pragma once

#include "Stage.h"

namespace mips {

class Cpu;
class PipelineRegister;

/**
 * @brief Instruction Decode (ID) stage
 * 
 * Responsibilities:
 * - Decode instruction fields
 * - Read register values
 * - Generate control signals
 * - Detect hazards
 */
class IDStage : public Stage {
public:
    explicit IDStage(Cpu* cpu);
    ~IDStage() override = default;
    
    /**
     * @brief Execute ID stage logic
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
    
    /**
     * @brief Check if stage should stall due to hazards
     */
    bool shouldStall() const override;

private:
    PipelineRegister* m_inputRegister;
    PipelineRegister* m_outputRegister;
    bool m_stalled;
    
    /**
     * @brief Decode instruction and extract fields
     */
    void decodeInstruction(PipelineData& data);
    
    /**
     * @brief Generate control signals
     */
    void generateControlSignals(PipelineData& data);
    
    /**
     * @brief Read register values
     */
    void readRegisters(PipelineData& data);
    
    /**
     * @brief Detect load-use hazard
     */
    bool detectLoadUseHazard(const PipelineData& data);
};

} // namespace mips
