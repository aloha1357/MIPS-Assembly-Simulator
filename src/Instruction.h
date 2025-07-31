#pragma once

#include <cstdint>
#include <string>

namespace mips {

/**
 * @brief Base class for MIPS instructions
 */
class Instruction {
public:
    virtual ~Instruction() = default;
    
    /**
     * @brief Execute the instruction
     * @param cpu Reference to CPU for register/memory access
     */
    virtual void execute(class Cpu& cpu) = 0;
    
    /**
     * @brief Get instruction name for debugging
     */
    virtual std::string getName() const = 0;
};

/**
 * @brief R-type ADD instruction
 */
class AddInstruction : public Instruction {
public:
    AddInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
    
private:
    int m_rd; // Destination register
    int m_rs; // Source register 1
    int m_rt; // Source register 2
};

} // namespace mips
