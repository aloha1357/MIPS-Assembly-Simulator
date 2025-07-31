#include "Memory.h"
#include <cstring>

namespace mips {

Memory::Memory() : m_data(MEMORY_SIZE, 0) {
}

uint32_t Memory::readWord(uint32_t address) const {
    if (!isValidAddress(address)) {
        return 0; // Invalid access returns 0
    }
    
    uint32_t value;
    std::memcpy(&value, &m_data[address], sizeof(uint32_t));
    return value;
}

void Memory::writeWord(uint32_t address, uint32_t value) {
    if (!isValidAddress(address)) {
        return; // Invalid access ignored
    }
    
    std::memcpy(&m_data[address], &value, sizeof(uint32_t));
}

void Memory::reset() {
    std::fill(m_data.begin(), m_data.end(), static_cast<uint8_t>(0));
}

bool Memory::isValidAddress(uint32_t address) const {
    return (address + sizeof(uint32_t) <= MEMORY_SIZE) && 
           (address % sizeof(uint32_t) == 0); // Word-aligned
}

} // namespace mips
