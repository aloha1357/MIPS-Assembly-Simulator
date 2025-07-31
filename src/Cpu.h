#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "Stage.h"

namespace mips {

class RegisterFile;
class Memory;
class Instruction;

/**
 * @brief Main CPU class implementing 5-stage MIPS pipeline
 * 
 * Architecture: IF -> ID -> EX -> MEM -> WB
 * Handles instruction execution, hazard detection, and pipeline management
 */
class Cpu {
public:
    Cpu();
    ~Cpu();

    /**
     * @brief Execute one clock cycle
     */
    void tick();

    /**
     * @brief Load program from assembly string
     * @param assembly Assembly code as string
     */
    void loadProgramFromString(const std::string& assembly);

    /**
     * @brief Load program from assembly file
     * @param path Path to assembly file
     */
    void loadProgram(const std::string& path);

    /**
     * @brief Run for specified number of cycles
     * @param cycles Number of cycles to execute
     */
    void run(int cycles);

    /**
     * @brief Get register file for testing/debugging
     */
    RegisterFile& getRegisterFile();

    /**
     * @brief Get memory for testing/debugging
     */
    Memory& getMemory();

    /**
     * @brief Get current cycle count
     */
    int getCycleCount() const;

    /**
     * @brief Reset CPU state
     */
    void reset();

    /**
     * @brief Set program counter (for branch/jump instructions)
     */
    void setProgramCounter(uint32_t pc);

    /**
     * @brief Get program counter
     */
    uint32_t getProgramCounter() const;

    /**
     * @brief Get label address by name
     */
    uint32_t getLabelAddress(const std::string& label) const;

private:
    std::unique_ptr<RegisterFile> m_registerFile;
    std::unique_ptr<Memory> m_memory;
    
    // Program storage
    std::vector<std::unique_ptr<Instruction>> m_instructions;
    
    // Pipeline stages
    std::unique_ptr<Stage> m_ifStage;
    std::unique_ptr<Stage> m_idStage;
    std::unique_ptr<Stage> m_exStage;
    std::unique_ptr<Stage> m_memStage;
    std::unique_ptr<Stage> m_wbStage;
    
    // Pipeline registers
    std::unique_ptr<PipelineRegister> m_ifIdRegister;
    std::unique_ptr<PipelineRegister> m_idExRegister;
    std::unique_ptr<PipelineRegister> m_exMemRegister;
    std::unique_ptr<PipelineRegister> m_memWbRegister;
    
    int m_cycleCount;
    uint32_t m_pc; // Program counter
    
    // Label to instruction address mapping
    std::map<std::string, uint32_t> m_labelMap;
};

} // namespace mips
