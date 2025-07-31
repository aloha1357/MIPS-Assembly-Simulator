#pragma once

#include <cstdint>
#include <vector>

namespace mips {

/**
 * @brief Memory subsystem for data and instruction storage
 */
class Memory {
public:
    static constexpr uint32_t MEMORY_SIZE = 0x100000; // 1MB

    Memory();

    /**
     * @brief Read word from memory
     * @param address Memory address (must be word-aligned)
     * @return Word value
     */
    uint32_t readWord(uint32_t address) const;

    /**
     * @brief Write word to memory
     * @param address Memory address (must be word-aligned)
     * @param value Word value to write
     */
    void writeWord(uint32_t address, uint32_t value);

    /**
     * @brief Reset memory to all zeros
     */
    void reset();

    /**
     * @brief Check if address is valid and aligned
     */
    bool isValidAddress(uint32_t address) const;

private:
    std::vector<uint8_t> m_data;
};

} // namespace mips
