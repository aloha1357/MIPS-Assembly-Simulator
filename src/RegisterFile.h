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

    /**
     * @brief Read HI register value
     * @return HI register value
     */
    uint32_t readHI() const;

    /**
     * @brief Write HI register value
     * @param value Value to write
     */
    void writeHI(uint32_t value);

    /**
     * @brief Read LO register value
     * @return LO register value
     */
    uint32_t readLO() const;

    /**
     * @brief Write LO register value
     * @param value Value to write
     */
    void writeLO(uint32_t value);

private:
    std::array<uint32_t, NUM_REGISTERS> m_registers;
    uint32_t m_hi;  // HI register for multiply/divide
    uint32_t m_lo;  // LO register for multiply/divide
};

} // namespace mips
