#include "RegisterFile.h"

namespace mips {

RegisterFile::RegisterFile() {
    reset();
}

uint32_t RegisterFile::read(int regNum) const {
    if (regNum < 0 || regNum >= NUM_REGISTERS) {
        return 0; // Invalid register access returns 0
    }
    return m_registers[regNum];
}

void RegisterFile::write(int regNum, uint32_t value) {
    if (regNum <= 0 || regNum >= NUM_REGISTERS) {
        return; // $zero (reg 0) is hardwired to 0, invalid regs ignored
    }
    m_registers[regNum] = value;
}

void RegisterFile::reset() {
    m_registers.fill(0);
    m_hi = 0;
    m_lo = 0;
}

uint32_t RegisterFile::readHI() const {
    return m_hi;
}

void RegisterFile::writeHI(uint32_t value) {
    m_hi = value;
}

uint32_t RegisterFile::readLO() const {
    return m_lo;
}

void RegisterFile::writeLO(uint32_t value) {
    m_lo = value;
}

} // namespace mips
