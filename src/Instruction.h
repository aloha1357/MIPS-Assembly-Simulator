#pragma once

#include <cstdint>
#include <string>

namespace mips {

class Cpu; // Forward declaration

/**
 * @brief Base class for all MIPS instructions
 */
class Instruction {
public:
    virtual ~Instruction() = default;
    
    /**
     * @brief Execute the instruction
     * @param cpu Reference to the CPU instance
     */
    virtual void execute(Cpu& cpu) = 0;
    
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
 * @brief R-type ADDU instruction (add unsigned)
 */
class ADDUInstruction : public RTypeInstruction {
public:
    ADDUInstruction(int rd, int rs, int rt);
    
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
 * @brief R-type SUBU instruction (subtract unsigned)
 */
class SUBUInstruction : public RTypeInstruction {
public:
    SUBUInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type AND instruction (logical AND)
 */
class AndInstruction : public RTypeInstruction {
public:
    AndInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type OR instruction (logical OR)
 */
class OrInstruction : public RTypeInstruction {
public:
    OrInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type XOR instruction (logical XOR)
 */
class XorInstruction : public RTypeInstruction {
public:
    XorInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type NOR instruction (logical NOR)
 */
class NorInstruction : public RTypeInstruction {
public:
    NorInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type SLT instruction (set less than)
 */
class SltInstruction : public RTypeInstruction {
public:
    SltInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type SLTU instruction (set less than unsigned)
 */
class SltuInstruction : public RTypeInstruction {
public:
    SltuInstruction(int rd, int rs, int rt);
    
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
 * @brief I-type ADDIU instruction (add immediate unsigned)
 * Note: Despite the name "unsigned", the immediate value is sign-extended.
 * The difference from ADDI is that ADDIU does not generate overflow exceptions.
 */
class ADDIUInstruction : public ITypeInstruction {
public:
    ADDIUInstruction(int rt, int rs, int16_t imm);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SLTI instruction (set less than immediate)
 */
class SltiInstruction : public ITypeInstruction {
public:
    SltiInstruction(int rt, int rs, int16_t imm);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SLTIU instruction (set less than immediate unsigned)
 */
class SltiuInstruction : public ITypeInstruction {
public:
    SltiuInstruction(int rt, int rs, int16_t imm);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type ORI instruction (OR immediate)
 */
class OriInstruction : public ITypeInstruction {
public:
    OriInstruction(int rt, int rs, int16_t imm);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type ANDI instruction (AND immediate)
 */
class AndiInstruction : public ITypeInstruction {
public:
    AndiInstruction(int rt, int rs, int16_t imm);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type XORI instruction (XOR immediate)
 */
class XoriInstruction : public ITypeInstruction {
public:
    XoriInstruction(int rt, int rs, int16_t imm);
    
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
 * @brief I-type BNE instruction (branch not equal)
 */
class BneInstruction : public ITypeInstruction {
public:
    BneInstruction(int rs, int rt, int16_t offset);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type BLEZ instruction (branch less than or equal zero)
 */
class BLEZInstruction : public ITypeInstruction {
public:
    BLEZInstruction(int rs, int16_t offset);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type BGTZ instruction (branch greater than zero)
 */
class BGTZInstruction : public ITypeInstruction {
public:
    BGTZInstruction(int rs, int16_t offset);
    
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

/**
 * @brief Shift left logical instruction
 */
class SllInstruction : public Instruction {
public:
    SllInstruction(uint32_t rd, uint32_t rt, uint32_t shamt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
    
private:
    uint32_t m_rd;     // Destination register
    uint32_t m_rt;     // Source register
    uint32_t m_shamt;  // Shift amount
};

/**
 * @brief Shift right logical instruction
 */
class SrlInstruction : public Instruction {
public:
    SrlInstruction(uint32_t rd, uint32_t rt, uint32_t shamt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
    
private:
    uint32_t m_rd;     // Destination register
    uint32_t m_rt;     // Source register
    uint32_t m_shamt;  // Shift amount
};

/**
 * @brief Shift right arithmetic instruction
 */
class SraInstruction : public Instruction {
public:
    SraInstruction(uint32_t rd, uint32_t rt, uint32_t shamt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
    
private:
    uint32_t m_rd;     // Destination register
    uint32_t m_rt;     // Source register
    uint32_t m_shamt;  // Shift amount
};

/**
 * @brief System call instruction
 */
class SyscallInstruction : public Instruction {
public:
    SyscallInstruction();
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
    
private:
    void handlePrintInt(Cpu& cpu);
    void handlePrintString(Cpu& cpu);
    void handleReadInt(Cpu& cpu);
    void handleExit(Cpu& cpu);
};

} // namespace mips
