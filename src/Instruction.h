#pragma once

#include <cstdint>
#include <string>

namespace mips
{

class Cpu;  // Forward declaration

/**
 * @brief Base class for all MIPS instructions
 */
class Instruction
{
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
class RTypeInstruction : public Instruction
{
  public:
    RTypeInstruction(int rd, int rs, int rt);

  protected:
    int m_rd;  // Destination register
    int m_rs;  // Source register 1
    int m_rt;  // Source register 2
};

/**
 * @brief R-type ADD instruction
 */
class AddInstruction : public RTypeInstruction
{
  public:
    AddInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type ADDU instruction (add unsigned)
 */
class ADDUInstruction : public RTypeInstruction
{
  public:
    ADDUInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type SUB instruction
 */
class SubInstruction : public RTypeInstruction
{
  public:
    SubInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type SUBU instruction (subtract unsigned)
 */
class SUBUInstruction : public RTypeInstruction
{
  public:
    SUBUInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type AND instruction (logical AND)
 */
class AndInstruction : public RTypeInstruction
{
  public:
    AndInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type OR instruction (logical OR)
 */
class OrInstruction : public RTypeInstruction
{
  public:
    OrInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type XOR instruction (logical XOR)
 */
class XorInstruction : public RTypeInstruction
{
  public:
    XorInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type NOR instruction (logical NOR)
 */
class NorInstruction : public RTypeInstruction
{
  public:
    NorInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type SLT instruction (set less than)
 */
class SltInstruction : public RTypeInstruction
{
  public:
    SltInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief R-type SLTU instruction (set less than unsigned)
 */
class SltuInstruction : public RTypeInstruction
{
  public:
    SltuInstruction(int rd, int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief MULT instruction (multiply signed)
 * Function Code: 0x18
 * Format: mult $rs, $rt
 * Operation: HI:LO = $rs × $rt (signed 64-bit result)
 */
class MULTInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a MULT instruction
     * @param rs Source register 1
     * @param rt Source register 2
     */
    MULTInstruction(int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rs;  // Source register 1
    int m_rt;  // Source register 2
};

/**
 * @brief MULTU instruction (multiply unsigned)
 * Function Code: 0x19
 * Format: multu $rs, $rt
 * Operation: HI:LO = $rs × $rt (unsigned 64-bit result)
 */
class MULTUInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a MULTU instruction
     * @param rs Source register 1
     * @param rt Source register 2
     */
    MULTUInstruction(int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rs;  // Source register 1
    int m_rt;  // Source register 2
};

/**
 * @brief DIV instruction (divide signed)
 * Function Code: 0x1A
 * Format: div $rs, $rt
 * Operation: LO = $rs ÷ $rt, HI = $rs mod $rt (signed division)
 */
class DIVInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a DIV instruction
     * @param rs Source register 1 (dividend)
     * @param rt Source register 2 (divisor)
     */
    DIVInstruction(int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rs;  // Source register 1 (dividend)
    int m_rt;  // Source register 2 (divisor)
};

/**
 * @brief DIVU instruction (divide unsigned)
 * Function Code: 0x1B
 * Format: divu $rs, $rt
 * Operation: LO = $rs ÷ $rt, HI = $rs mod $rt (unsigned division)
 */
class DIVUInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a DIVU instruction
     * @param rs Source register 1 (dividend)
     * @param rt Source register 2 (divisor)
     */
    DIVUInstruction(int rs, int rt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rs;  // Source register 1 (dividend)
    int m_rt;  // Source register 2 (divisor)
};

/**
 * @brief Base class for I-type instructions (immediate operand)
 */
class ITypeInstruction : public Instruction
{
  public:
    ITypeInstruction(int rt, int rs, int16_t imm);

  protected:
    int     m_rt;   // Target register
    int     m_rs;   // Source register
    int16_t m_imm;  // 16-bit immediate value

    /**
     * @brief Sign-extend 16-bit immediate to 32-bit
     */
    static uint32_t signExtend16(int16_t value);
};

/**
 * @brief I-type ADDI instruction (add immediate)
 */
class AddiInstruction : public ITypeInstruction
{
  public:
    AddiInstruction(int rt, int rs, int16_t imm);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type ADDIU instruction (add immediate unsigned)
 * Note: Despite the name "unsigned", the immediate value is sign-extended.
 * The difference from ADDI is that ADDIU does not generate overflow exceptions.
 */
class ADDIUInstruction : public ITypeInstruction
{
  public:
    ADDIUInstruction(int rt, int rs, int16_t imm);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SLTI instruction (set less than immediate)
 */
class SltiInstruction : public ITypeInstruction
{
  public:
    SltiInstruction(int rt, int rs, int16_t imm);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SLTIU instruction (set less than immediate unsigned)
 */
class SltiuInstruction : public ITypeInstruction
{
  public:
    SltiuInstruction(int rt, int rs, int16_t imm);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type ORI instruction (OR immediate)
 */
class OriInstruction : public ITypeInstruction
{
  public:
    OriInstruction(int rt, int rs, int16_t imm);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type ANDI instruction (AND immediate)
 */
class AndiInstruction : public ITypeInstruction
{
  public:
    AndiInstruction(int rt, int rs, int16_t imm);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type XORI instruction (XOR immediate)
 */
class XoriInstruction : public ITypeInstruction
{
  public:
    XoriInstruction(int rt, int rs, int16_t imm);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type LW instruction (load word)
 */
class LwInstruction : public ITypeInstruction
{
  public:
    LwInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type LB instruction (load byte)
 * Loads a byte from memory and sign-extends it to 32 bits
 */
class LBInstruction : public ITypeInstruction
{
  public:
    LBInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SB instruction (store byte)
 * Stores the low-order 8 bits of a register to memory
 */
class SBInstruction : public ITypeInstruction
{
  public:
    SBInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type LBU instruction (load byte unsigned)
 * Opcode: 0x24
 * Format: lbu $rt, offset($rs)
 * Operation: rt = zero_extend(memory[rs + offset][7:0])
 */
class LBUInstruction : public ITypeInstruction
{
  public:
    LBUInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type LH instruction (load halfword)
 * Opcode: 0x21
 * Format: lh $rt, offset($rs)
 * Operation: rt = sign_extend(memory[rs + offset][15:0])
 */
class LHInstruction : public ITypeInstruction
{
  public:
    LHInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SH instruction (store halfword)
 * Stores the low 16 bits of rt to memory address computed as rs + offset
 */
class SHInstruction : public ITypeInstruction
{
  public:
    SHInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type LHU instruction (load halfword unsigned)
 * Loads halfword from memory with zero extension (unsigned load)
 */
class LHUInstruction : public ITypeInstruction
{
  public:
    LHUInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type SW instruction (store word)
 */
class SwInstruction : public ITypeInstruction
{
  public:
    SwInstruction(int rt, int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief Base class for branch instructions
 */
class BranchInstruction : public Instruction
{
  public:
    BranchInstruction(int rs, int rt, const std::string& label);

  protected:
    int         m_rs;     // Source register 1
    int         m_rt;     // Source register 2
    std::string m_label;  // Branch target label
};

/**
 * @brief Branch on equal instruction
 */
class BeqInstruction : public BranchInstruction
{
  public:
    BeqInstruction(int rs, int rt, const std::string& label);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type BNE instruction (branch not equal)
 */
class BneInstruction : public ITypeInstruction
{
  public:
    BneInstruction(int rs, int rt, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type BLEZ instruction (branch less than or equal zero)
 */
class BLEZInstruction : public ITypeInstruction
{
  public:
    BLEZInstruction(int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief I-type BGTZ instruction (branch greater than zero)
 */
class BGTZInstruction : public ITypeInstruction
{
  public:
    BGTZInstruction(int rs, int16_t offset);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief Jump instruction
 */
class JInstruction : public Instruction
{
  public:
    JInstruction(const std::string& label);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    std::string m_label;  // Jump target label
};

/**
 * @brief Shift left logical instruction
 */
class SllInstruction : public Instruction
{
  public:
    SllInstruction(uint32_t rd, uint32_t rt, uint32_t shamt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    uint32_t m_rd;     // Destination register
    uint32_t m_rt;     // Source register
    uint32_t m_shamt;  // Shift amount
};

/**
 * @brief Shift right logical instruction
 */
class SrlInstruction : public Instruction
{
  public:
    SrlInstruction(uint32_t rd, uint32_t rt, uint32_t shamt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    uint32_t m_rd;     // Destination register
    uint32_t m_rt;     // Source register
    uint32_t m_shamt;  // Shift amount
};

/**
 * @brief Shift right arithmetic instruction
 */
class SraInstruction : public Instruction
{
  public:
    SraInstruction(uint32_t rd, uint32_t rt, uint32_t shamt);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    uint32_t m_rd;     // Destination register
    uint32_t m_rt;     // Source register
    uint32_t m_shamt;  // Shift amount
};

/**
 * @brief Shift left logical variable instruction
 */
class SLLVInstruction : public RTypeInstruction
{
  public:
    SLLVInstruction(int rd, int rt, int rs);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief Shift right logical variable instruction
 */
class SRLVInstruction : public RTypeInstruction
{
  public:
    SRLVInstruction(int rd, int rt, int rs);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief Shift right arithmetic variable instruction
 */
class SRAVInstruction : public RTypeInstruction
{
  public:
    SRAVInstruction(int rd, int rt, int rs);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief JR (Jump Register) instruction - R-type instruction with special format
 * Function Code: 0x08
 * Format: jr $rs
 * Operation: PC = $rs (jump to address in register)
 */
class JRInstruction : public RTypeInstruction
{
  public:
    /**
     * @brief Construct a JR instruction
     * @param rs Source register containing target address
     */
    explicit JRInstruction(int rs);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief JAL (Jump And Link) instruction - J-type instruction
 * Opcode: 0x03
 * Format: jal target
 * Operation: $ra = PC+4; PC = target (jump to immediate address and save return address)
 */
class JALInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a JAL instruction
     * @param target Target address (26-bit immediate value)
     */
    explicit JALInstruction(uint32_t target);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    uint32_t m_target;  // 26-bit target address
};

/**
 * @brief JALR (Jump And Link Register) instruction - R-type instruction with special format
 * Function Code: 0x09
 * Format: jalr $rd, $rs or jalr $rs (default $rd=$ra)
 * Operation: $rd = PC+4; PC = $rs (jump to register address and save return address)
 */
class JALRInstruction : public RTypeInstruction
{
  public:
    /**
     * @brief Construct a JALR instruction
     * @param rd Destination register for return address
     * @param rs Source register containing target address
     */
    JALRInstruction(int rd, int rs);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief MFHI instruction (Move From HI)
 * Function Code: 0x10
 * Format: mfhi $rd
 * Operation: $rd = HI (將HI暫存器內容移動到通用暫存器)
 */
class MFHIInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a MFHI instruction
     * @param rd Destination register to receive HI value
     */
    MFHIInstruction(int rd);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rd;  // Destination register
};

/**
 * @brief MTHI instruction (Move To HI)
 * Function Code: 0x11
 * Format: mthi $rs
 * Operation: HI = $rs (將通用暫存器內容移動到HI暫存器)
 */
class MTHIInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a MTHI instruction
     * @param rs Source register to provide value for HI
     */
    MTHIInstruction(int rs);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rs;  // Source register
};

/**
 * @brief MFLO instruction (Move From LO)
 * Function Code: 0x12
 * Format: mflo $rd
 * Operation: $rd = LO (將LO暫存器內容移動到通用暫存器)
 */
class MFLOInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a MFLO instruction
     * @param rd Destination register to receive LO value
     */
    MFLOInstruction(int rd);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rd;  // Destination register
};

/**
 * @brief MTLO instruction (Move To LO)
 * Function Code: 0x13
 * Format: mtlo $rs
 * Operation: LO = $rs (將通用暫存器內容移動到LO暫存器)
 */
class MTLOInstruction : public Instruction
{
  public:
    /**
     * @brief Construct a MTLO instruction
     * @param rs Source register to provide value for LO
     */
    MTLOInstruction(int rs);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    int m_rs;  // Source register
};

/**
 * @brief System call instruction
 */
class SyscallInstruction : public Instruction
{
  public:
    SyscallInstruction();

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    void handlePrintInt(Cpu& cpu);
    void handlePrintString(Cpu& cpu);
    void handleReadInt(Cpu& cpu);
    void handleExit(Cpu& cpu);
    void handlePrintCharacter(Cpu& cpu);
    void handleReadCharacter(Cpu& cpu);
};

/**
 * @brief LLO instruction (Load Low Immediate)
 * Opcode: 0x18
 * Format: llo $rt, immediate
 * Operation: rt = rt & 0xffff0000u | imm
 */
class LLOInstruction : public ITypeInstruction
{
  public:
    LLOInstruction(int rt, uint16_t immediate);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief LHI instruction (Load High Immediate)
 * Opcode: 0x19
 * Format: lhi $rt, immediate
 * Operation: rt = rt & 0x0000ffffu | (imm << 16)
 */
class LHIInstruction : public ITypeInstruction
{
  public:
    LHIInstruction(int rt, uint16_t immediate);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;
};

/**
 * @brief TRAP instruction
 * Opcode: 0x1A
 * Format: trap syscall
 * Operation: trap(imm)
 */
class TrapInstruction : public Instruction
{
  public:
    TrapInstruction(uint32_t trapCode);

    void        execute(Cpu& cpu) override;
    std::string getName() const override;

  private:
    uint32_t m_trapCode;
};

}  // namespace mips
