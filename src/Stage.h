#pragma once

#include <cstdint>
#include <memory>

namespace mips
{

class Instruction;
class Cpu;

/**
 * @brief Base class for pipeline stages
 */
class Stage
{
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

    /**
     * @brief Check if stage should stall
     */
    virtual bool shouldStall() const
    {
        return false;
    }

    /**
     * @brief Flush stage (for branch misprediction)
     */
    virtual void flush() = 0;

  protected:
    Cpu* m_cpu = nullptr;
};

/**
 * @brief Pipeline register data structure
 */
struct PipelineData
{
    // Instruction information
    Instruction* instruction = nullptr;  // Raw pointer for pipeline references
    uint32_t     pc          = 0;

    // Decoded instruction fields
    uint32_t opcode    = 0;
    uint32_t rs        = 0;
    uint32_t rt        = 0;
    uint32_t rd        = 0;
    uint32_t immediate = 0;
    uint32_t shamt     = 0;
    uint32_t funct     = 0;

    // Register values
    uint32_t rsValue = 0;
    uint32_t rtValue = 0;

    // ALU result
    uint32_t aluResult = 0;

    // Memory data
    uint32_t memoryData = 0;

    // Control signals
    bool     regWrite = false;
    bool     memRead  = false;
    bool     memWrite = false;
    bool     branch   = false;
    bool     jump     = false;
    bool     aluSrc   = false;
    uint32_t regDst   = 0;  // 0=rt, 1=rd
    uint32_t memToReg = 0;  // 0=alu, 1=memory

    void reset()
    {
        instruction = nullptr;
        pc          = 0;
        opcode = rs = rt = rd = immediate = shamt = funct = 0;
        rsValue = rtValue = aluResult = memoryData = 0;
        regWrite = memRead = memWrite = branch = jump = aluSrc = false;
        regDst = memToReg = 0;
    }
};

/**
 * @brief Pipeline register between stages
 */
class PipelineRegister
{
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

    /**
     * @brief Get pipeline data
     */
    const PipelineData& getData() const;

    /**
     * @brief Set pipeline data
     */
    void setData(const PipelineData& data);

    /**
     * @brief Update register on clock edge
     */
    void clockUpdate();

  private:
    bool         m_isBubble;
    PipelineData m_currentData;
    PipelineData m_nextData;
};

}  // namespace mips
