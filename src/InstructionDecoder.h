#pragma once

#include <cstdint>
#include <memory>

namespace mips
{

class Instruction;

/**
 * @brief 32-bit MIPS instruction decoder
 *
 * Converts binary machine code words into Instruction objects
 */
class InstructionDecoder
{
  public:
    /**
     * @brief Decode a 32-bit instruction word into an Instruction object
     * @param word 32-bit machine code instruction
     * @return Unique pointer to decoded instruction, or nullptr if invalid
     */
    static std::unique_ptr<Instruction> decode(uint32_t word);

  private:
    // Field extraction methods
    static uint32_t extractOpcode(uint32_t word);      // bits 31-26
    static uint32_t extractRs(uint32_t word);          // bits 25-21
    static uint32_t extractRt(uint32_t word);          // bits 20-16
    static uint32_t extractRd(uint32_t word);          // bits 15-11
    static uint32_t extractImmediate(uint32_t word);   // bits 15-0
    static uint32_t extractJumpTarget(uint32_t word);  // bits 25-0
    static uint32_t extractFunction(uint32_t word);    // bits 5-0
    static uint32_t extractShamt(uint32_t word);       // bits 10-6

    // Instruction type decoders
    static std::unique_ptr<Instruction> decodeRType(uint32_t word);
    static std::unique_ptr<Instruction> decodeIType(uint32_t word);
    static std::unique_ptr<Instruction> decodeJType(uint32_t word);
    static std::unique_ptr<Instruction> decodeTrapInstruction(uint32_t word);
};

}  // namespace mips
