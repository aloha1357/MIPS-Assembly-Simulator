#pragma once

#include <cstdint>

namespace mips {

/**
 * @brief Base class for pipeline stages
 */
class Stage {
public:
    virtual ~Stage() = default;
    
    /**
     * @brief Execute stage logic for current cycle
     */
    virtual void execute() = 0;
    
    /**
     * @brief Reset stage state
     */
    virtual void reset() = 0;
};

/**
 * @brief Pipeline register between stages
 */
class PipelineRegister {
public:
    PipelineRegister();
    ~PipelineRegister() = default;
    
    /**
     * @brief Check if register contains a bubble (NOP)
     */
    bool isBubble() const;
    
    /**
     * @brief Set register to bubble state
     */
    void setBubble();
    
    /**
     * @brief Reset register state
     */
    void reset();
    
private:
    bool m_isBubble;
    // TODO: Add actual instruction/data fields
};

} // namespace mips
