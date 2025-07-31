#pragma once

#include <cstdint>
#include <array>

namespace mips {

/**
 * @brief MIPS register file with 32 general-purpose registers
 */
class RegisterFile {
public:
    static constexpr int NUM_REGISTERS = 32;

    RegisterFile();

    /**
     * @brief Read register value
     * @param regNum Register number (0-31)
     * @return Register value
     */
    uint32_t read(int regNum) const;

    /**
     * @brief Write register value
     * @param regNum Register number (0-31, $zero writes are ignored)
     * @param value Value to write
     */
    void write(int regNum, uint32_t value);

    /**
     * @brief Reset all registers to zero
     */
    void reset();

private:
    std::array<uint32_t, NUM_REGISTERS> m_registers;
};

} // namespace mips
