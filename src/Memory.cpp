#include "Memory.h"
#include <cstring>
#include <iostream>
#include <iomanip>

namespace mips
{

Memory::Memory() : m_data(MEMORY_SIZE, 0) {}

uint32_t Memory::readWord(uint32_t address) const
{
    if (!isValidAddress(address))
    {
        return 0;  // Invalid access returns 0
    }

    uint32_t value;
    std::memcpy(&value, &m_data[address], sizeof(uint32_t));
    return value;
}

void Memory::writeWord(uint32_t address, uint32_t value)
{
    if (!isValidAddress(address))
    {
        return;  // Invalid access ignored
    }

    // Targeted debug: watch writes near the string/data area used in tests
    if (address >= 760 && address <= 780)
    {
        std::cerr << "DEBUG: writeWord addr=" << address << " value=0x" << std::hex << value << std::dec << std::endl;
    }

    std::memcpy(&m_data[address], &value, sizeof(uint32_t));
}

uint8_t Memory::readByte(uint32_t address) const
{
    if (address >= MEMORY_SIZE)
    {
        return 0;  // Invalid access returns 0
    }

    return m_data[address];
}

void Memory::writeByte(uint32_t address, uint8_t value)
{
    if (address >= MEMORY_SIZE)
    {
        return;  // Invalid access ignored
    }

    if (address >= 760 && address <= 780)
    {
        std::cerr << "DEBUG: writeByte addr=" << address << " byte=" << static_cast<uint32_t>(value) << std::endl;
    }

    m_data[address] = value;
}

uint16_t Memory::readHalfword(uint32_t address) const
{
    if (address + sizeof(uint16_t) > MEMORY_SIZE || address % sizeof(uint16_t) != 0)
    {
        return 0;  // Invalid access returns 0
    }

    uint16_t value;
    std::memcpy(&value, &m_data[address], sizeof(uint16_t));
    return value;
}

void Memory::writeHalfword(uint32_t address, uint16_t value)
{
    if (address + sizeof(uint16_t) > MEMORY_SIZE || address % sizeof(uint16_t) != 0)
    {
        return;  // Invalid access ignored
    }

    std::memcpy(&m_data[address], &value, sizeof(uint16_t));
}

void Memory::reset()
{
    std::fill(m_data.begin(), m_data.end(), static_cast<uint8_t>(0));
}

bool Memory::isValidAddress(uint32_t address) const
{
    return (address + sizeof(uint32_t) <= MEMORY_SIZE) &&
           (address % sizeof(uint32_t) == 0);  // Word-aligned
}

}  // namespace mips
