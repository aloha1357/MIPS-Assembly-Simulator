#include "InstructionDecoder.h"
#include "Instruction.h"

namespace mips
{

std::unique_ptr<Instruction> InstructionDecoder::decode(uint32_t word)
{
    uint32_t opcode = extractOpcode(word);

    switch (opcode)
    {
    case 0x00: // R-type instructions
        return decodeRType(word);
    case 0x02: // J instruction
    case 0x03: // JAL instruction (not implemented yet)
        return decodeJType(word);
    case 0x08: // ADDI instruction
    case 0x09: // ADDIU instruction
    case 0x0A: // SLTI instruction
    case 0x0B: // SLTIU instruction
    case 0x0C: // ANDI instruction
    case 0x0D: // ORI instruction
    case 0x0E: // XORI instruction
    case 0x20: // LB instruction
    case 0x21: // LH instruction
    case 0x23: // LW instruction
    case 0x24: // LBU instruction
    case 0x25: // LHU instruction
    case 0x28: // SB instruction
    case 0x29: // SH instruction
    case 0x2B: // SW instruction
    case 0x04: // BEQ instruction
    case 0x05: // BNE instruction
    case 0x06: // BLEZ instruction
    case 0x07: // BGTZ instruction
        return decodeIType(word);
    default:
        return nullptr; // Unknown instruction
    }
}

uint32_t InstructionDecoder::extractOpcode(uint32_t word)
{
    return (word >> 26) & 0x3F; // bits 31-26
}

uint32_t InstructionDecoder::extractRs(uint32_t word)
{
    return (word >> 21) & 0x1F; // bits 25-21
}

uint32_t InstructionDecoder::extractRt(uint32_t word)
{
    return (word >> 16) & 0x1F; // bits 20-16
}

uint32_t InstructionDecoder::extractRd(uint32_t word)
{
    return (word >> 11) & 0x1F; // bits 15-11
}

uint32_t InstructionDecoder::extractImmediate(uint32_t word)
{
    return word & 0xFFFF; // bits 15-0
}

uint32_t InstructionDecoder::extractJumpTarget(uint32_t word)
{
    return word & 0x3FFFFFF; // bits 25-0
}

uint32_t InstructionDecoder::extractFunction(uint32_t word)
{
    return word & 0x3F; // bits 5-0
}

uint32_t InstructionDecoder::extractShamt(uint32_t word)
{
    return (word >> 6) & 0x1F; // bits 10-6
}

std::unique_ptr<Instruction> InstructionDecoder::decodeRType(uint32_t word)
{
    uint32_t rs       = extractRs(word);
    uint32_t rt       = extractRt(word);
    uint32_t rd       = extractRd(word);
    uint32_t shamt    = extractShamt(word);
    uint32_t function = extractFunction(word);

    switch (function)
    {
    case 0x00: // SLL instruction
        return std::make_unique<SllInstruction>(rd, rt, shamt);
    case 0x02: // SRL instruction
        return std::make_unique<SrlInstruction>(rd, rt, shamt);
    case 0x03: // SRA instruction
        return std::make_unique<SraInstruction>(rd, rt, shamt);
    case 0x04: // SLLV instruction
        return std::make_unique<SLLVInstruction>(rd, rt, rs);
    case 0x06: // SRLV instruction
        return std::make_unique<SRLVInstruction>(rd, rt, rs);
    case 0x07: // SRAV instruction
        return std::make_unique<SRAVInstruction>(rd, rt, rs);
    case 0x08: // JR instruction
        return std::make_unique<JRInstruction>(rs);
    case 0x09: // JALR instruction
        return std::make_unique<JALRInstruction>(rd, rs);
    case 0x20: // ADD instruction
        return std::make_unique<AddInstruction>(rd, rs, rt);
    case 0x21: // ADDU instruction
        return std::make_unique<ADDUInstruction>(rd, rs, rt);
    case 0x22: // SUB instruction
        return std::make_unique<SubInstruction>(rd, rs, rt);
    case 0x23: // SUBU instruction
        return std::make_unique<SUBUInstruction>(rd, rs, rt);
    case 0x24: // AND instruction
        return std::make_unique<AndInstruction>(rd, rs, rt);
    case 0x25: // OR instruction
        return std::make_unique<OrInstruction>(rd, rs, rt);
    case 0x26: // XOR instruction
        return std::make_unique<XorInstruction>(rd, rs, rt);
    case 0x27: // NOR instruction
        return std::make_unique<NorInstruction>(rd, rs, rt);
    case 0x2A: // SLT instruction
        return std::make_unique<SltInstruction>(rd, rs, rt);
    case 0x2B: // SLTU instruction
        return std::make_unique<SltuInstruction>(rd, rs, rt);
    case 0x18: // MULT instruction
        return std::make_unique<MULTInstruction>(rs, rt);
    case 0x19: // MULTU instruction
        return std::make_unique<MULTUInstruction>(rs, rt);
    case 0x1A: // DIV instruction
        return std::make_unique<DIVInstruction>(rs, rt);
    case 0x1B: // DIVU instruction
        return std::make_unique<DIVUInstruction>(rs, rt);
    case 0x10: // MFHI instruction
        return std::make_unique<MFHIInstruction>(rd);
    case 0x11: // MTHI instruction
        return std::make_unique<MTHIInstruction>(rs);
    case 0x12: // MFLO instruction
        return std::make_unique<MFLOInstruction>(rd);
    case 0x13: // MTLO instruction
        return std::make_unique<MTLOInstruction>(rs);
    case 0x0C: // SYSCALL instruction
        return std::make_unique<SyscallInstruction>();
    default:
        return nullptr; // Unknown R-type instruction
    }
}

std::unique_ptr<Instruction> InstructionDecoder::decodeIType(uint32_t word)
{
    uint32_t opcode    = extractOpcode(word);
    uint32_t rs        = extractRs(word);
    uint32_t rt        = extractRt(word);
    uint32_t immediate = extractImmediate(word);

    // Sign-extend 16-bit immediate to 16-bit signed value
    int16_t signedImmediate = static_cast<int16_t>(immediate);

    switch (opcode)
    {
    case 0x08: // ADDI instruction
        return std::make_unique<AddiInstruction>(rt, rs, signedImmediate);
    case 0x09: // ADDIU instruction
        return std::make_unique<ADDIUInstruction>(rt, rs, signedImmediate);
    case 0x0A: // SLTI instruction
        return std::make_unique<SltiInstruction>(rt, rs, signedImmediate);
    case 0x0B: // SLTIU instruction
        return std::make_unique<SltiuInstruction>(rt, rs, signedImmediate);
    case 0x20: // LB instruction
        return std::make_unique<LBInstruction>(rt, rs, signedImmediate);
    case 0x21: // LH instruction
        return std::make_unique<LHInstruction>(rt, rs, signedImmediate);
    case 0x23: // LW instruction
        return std::make_unique<LwInstruction>(rt, rs, signedImmediate);
    case 0x24: // LBU instruction
        return std::make_unique<LBUInstruction>(rt, rs, signedImmediate);
    case 0x25: // LHU instruction
        return std::make_unique<LHUInstruction>(rt, rs, signedImmediate);
    case 0x28: // SB instruction
        return std::make_unique<SBInstruction>(rt, rs, signedImmediate);
    case 0x29: // SH instruction
        return std::make_unique<SHInstruction>(rt, rs, signedImmediate);
    case 0x2B: // SW instruction
        return std::make_unique<SwInstruction>(rt, rs, signedImmediate);
    case 0x04: // BEQ instruction
        // For BEQ, we need to convert immediate to a label (simplified approach)
        // In a real implementation, this would need a symbol table
        return std::make_unique<BeqInstruction>(rs, rt, "label_" + std::to_string(signedImmediate));
    case 0x05: // BNE instruction
        // For BNE, use the signed immediate directly as offset
        return std::make_unique<BneInstruction>(rs, rt, signedImmediate);
    case 0x06: // BLEZ instruction
        // For BLEZ, only rs is used, rt is ignored (should be 0)
        return std::make_unique<BLEZInstruction>(rs, signedImmediate);
    case 0x07: // BGTZ instruction
        // For BGTZ, only rs is used, rt is ignored (should be 0)
        return std::make_unique<BGTZInstruction>(rs, signedImmediate);
    case 0x0C: // ANDI instruction
        return std::make_unique<AndiInstruction>(rt, rs, signedImmediate);
    case 0x0D: // ORI instruction
        return std::make_unique<OriInstruction>(rt, rs, signedImmediate);
    case 0x0E: // XORI instruction
        return std::make_unique<XoriInstruction>(rt, rs, signedImmediate);
    default:
        return nullptr; // Unknown I-type instruction
    }
}

std::unique_ptr<Instruction> InstructionDecoder::decodeJType(uint32_t word)
{
    uint32_t opcode     = extractOpcode(word);
    uint32_t jumpTarget = extractJumpTarget(word);

    switch (opcode)
    {
    case 0x02: // J instruction
        // Convert jump target to label (simplified approach)
        return std::make_unique<JInstruction>("label_" + std::to_string(jumpTarget));
    case 0x03: // JAL instruction
        return std::make_unique<JALInstruction>(jumpTarget);
    default:
        return nullptr; // Unknown J-type instruction
    }
}

} // namespace mips
