#pragma once

#include <cstdint>
#include <string>

namespace mips
{

/**
 * @brief Base class for MIPS instructions
 */
class Instruction
{
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
class RTypeInstruction : public Instruction
{
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
class AddInstruction : public RTypeInstruction
{
  public:
    AddInstruction(int rd, int rs, int rt);

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

} // namespace mips
