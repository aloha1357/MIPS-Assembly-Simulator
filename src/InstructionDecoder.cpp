#include "InstructionDecoder.h"
#include "Instruction.h"

namespace mips {

std::unique_ptr<Instruction> InstructionDecoder::decode(uint32_t word) {
    uint32_t opcode = extractOpcode(word);
    
    switch (opcode) {
        case 0x00:  // R-type instructions
            return decodeRType(word);
        case 0x02:  // J instruction
        case 0x03:  // JAL instruction (not implemented yet)
            return decodeJType(word);
        case 0x08:  // ADDI instruction
        case 0x23:  // LW instruction
        case 0x2B:  // SW instruction
        case 0x04:  // BEQ instruction
            return decodeIType(word);
        default:
            return nullptr; // Unknown instruction
    }
}

uint32_t InstructionDecoder::extractOpcode(uint32_t word) {
    return (word >> 26) & 0x3F; // bits 31-26
}

uint32_t InstructionDecoder::extractRs(uint32_t word) {
    return (word >> 21) & 0x1F; // bits 25-21
}

uint32_t InstructionDecoder::extractRt(uint32_t word) {
    return (word >> 16) & 0x1F; // bits 20-16
}

uint32_t InstructionDecoder::extractRd(uint32_t word) {
    return (word >> 11) & 0x1F; // bits 15-11
}

uint32_t InstructionDecoder::extractImmediate(uint32_t word) {
    return word & 0xFFFF; // bits 15-0
}

uint32_t InstructionDecoder::extractJumpTarget(uint32_t word) {
    return word & 0x3FFFFFF; // bits 25-0
}

uint32_t InstructionDecoder::extractFunction(uint32_t word) {
    return word & 0x3F; // bits 5-0
}

uint32_t InstructionDecoder::extractShamt(uint32_t word) {
    return (word >> 6) & 0x1F; // bits 10-6
}

std::unique_ptr<Instruction> InstructionDecoder::decodeRType(uint32_t word) {
    uint32_t rs = extractRs(word);
    uint32_t rt = extractRt(word);
    uint32_t rd = extractRd(word);
    uint32_t shamt = extractShamt(word);
    uint32_t function = extractFunction(word);
    
    switch (function) {
        case 0x00:  // SLL instruction
            return std::make_unique<SllInstruction>(rd, rt, shamt);
        case 0x20:  // ADD instruction
            return std::make_unique<AddInstruction>(rd, rs, rt);
        case 0x22:  // SUB instruction
            return std::make_unique<SubInstruction>(rd, rs, rt);
        case 0x0C:  // SYSCALL instruction
            return std::make_unique<SyscallInstruction>();
        default:
            return nullptr; // Unknown R-type instruction
    }
}

std::unique_ptr<Instruction> InstructionDecoder::decodeIType(uint32_t word) {
    uint32_t opcode = extractOpcode(word);
    uint32_t rs = extractRs(word);
    uint32_t rt = extractRt(word);
    uint32_t immediate = extractImmediate(word);
    
    // Sign-extend 16-bit immediate to 16-bit signed value
    int16_t signedImmediate = static_cast<int16_t>(immediate);
    
    switch (opcode) {
        case 0x08:  // ADDI instruction
            return std::make_unique<AddiInstruction>(rt, rs, signedImmediate);
        case 0x23:  // LW instruction
            return std::make_unique<LwInstruction>(rt, rs, signedImmediate);
        case 0x2B:  // SW instruction
            return std::make_unique<SwInstruction>(rt, rs, signedImmediate);
        case 0x04:  // BEQ instruction
            // For BEQ, we need to convert immediate to a label (simplified approach)
            // In a real implementation, this would need a symbol table
            return std::make_unique<BeqInstruction>(rs, rt, "label_" + std::to_string(signedImmediate));
        default:
            return nullptr; // Unknown I-type instruction
    }
}

std::unique_ptr<Instruction> InstructionDecoder::decodeJType(uint32_t word) {
    uint32_t opcode = extractOpcode(word);
    uint32_t jumpTarget = extractJumpTarget(word);
    
    switch (opcode) {
        case 0x02:  // J instruction
            // Convert jump target to label (simplified approach)
            return std::make_unique<JInstruction>("label_" + std::to_string(jumpTarget));
        default:
            return nullptr; // Unknown J-type instruction
    }
}

} // namespace mips
