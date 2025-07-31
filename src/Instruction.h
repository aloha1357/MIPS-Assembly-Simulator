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
 * @brief Base class for R-type instructions (3 register operands)
 */
class RTypeInstruction : public Instruction {
public:
    RTypeInstruction(int rd, int rs, int rt);
    
protected:
    int m_rd; // Destination register
    int m_rs; // Source register 1
    int m_rt; // Source register 2
};

/**
 * @brief R-type ADD instruction
 */
class AddInstruction : public RTypeInstruction {
public:
    AddInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type SUB instruction
 */
class SubInstruction : public RTypeInstruction {
public:
    SubInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief Base class for I-type instructions (immediate operand)
 */
class ITypeInstruction : public Instruction {
public:
    ITypeInstruction(int rt, int rs, int16_t imm);
    
protected:
    int m_rt;      // Target register
    int m_rs;      // Source register  
    int16_t m_imm; // 16-bit immediate value
    
    /**
     * @brief Sign-extend 16-bit immediate to 32-bit
     */
    static uint32_t signExtend16(int16_t value);
};

/**
 * @brief I-type ADDI instruction (add immediate)
 */
class AddiInstruction : public ITypeInstruction {
public:
    AddiInstruction(int rt, int rs, int16_t imm);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type LW instruction (load word)
 */
class LwInstruction : public ITypeInstruction {
public:
    LwInstruction(int rt, int rs, int16_t offset);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SW instruction (store word)
 */
class SwInstruction : public ITypeInstruction {
public:
    SwInstruction(int rt, int rs, int16_t offset);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief Base class for branch instructions
 */
class BranchInstruction : public Instruction {
public:
    BranchInstruction(int rs, int rt, const std::string& label);
    
protected:
    int m_rs;              // Source register 1
    int m_rt;              // Source register 2  
    std::string m_label;   // Branch target label
};

/**
 * @brief Branch on equal instruction
 */
class BeqInstruction : public BranchInstruction {
public:
    BeqInstruction(int rs, int rt, const std::string& label);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief Jump instruction
 */
class JInstruction : public Instruction {
public:
    JInstruction(const std::string& label);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
    
private:
    std::string m_label;   // Jump target label
};

} // namespace mips
