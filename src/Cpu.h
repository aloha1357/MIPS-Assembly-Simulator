#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <map>

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

    /**
     * @brief Enable/disable pipeline mode (future implementation)
     */
    void setPipelineMode(bool enabled);
    
    /**
     * @brief Check if pipeline mode is enabled (future implementation)
     */
    bool isPipelineMode() const;

    /**
     * @brief Print integer to console (for syscall support)
     */
    void printInt(uint32_t value);
    
    /**
     * @brief Print string to console (for syscall support)
     */
    void printString(const std::string& str);
    
    /**
     * @brief Read integer from console (for syscall support)
     */
    uint32_t readInt();
    
    /**
     * @brief Set program termination flag (for syscall support)
     */
    void terminate();
    
    /**
     * @brief Check if program should terminate
     */
    bool shouldTerminate() const;
    
    /**
     * @brief Get console output for testing
     */
    const std::string& getConsoleOutput() const;
    
    /**
     * @brief Set console input for testing
     */
    void setConsoleInput(const std::string& input);

private:
    std::unique_ptr<RegisterFile> m_registerFile;
    std::unique_ptr<Memory> m_memory;
    
    // Program storage
    std::vector<std::unique_ptr<Instruction>> m_instructions;
    
    int m_cycleCount;
    uint32_t m_pc; // Program counter
    bool m_pipelineMode; // Pipeline vs single-cycle mode (for future)
    bool m_terminated; // Program termination flag
    
    // Console I/O for syscall support
    std::string m_consoleOutput;
    std::string m_consoleInput;
    size_t m_inputPosition;
    
    // Label to instruction address mapping
    std::map<std::string, uint32_t> m_labelMap;

    // Pipeline components will be added later
    // For now, maintain single-cycle compatibility
};

} // namespace mips
