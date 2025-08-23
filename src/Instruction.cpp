#include "Instruction.h"
#include "Cpu.h"
#include "Memory.h"
#include "RegisterFile.h"

namespace mips
{

RTypeInstruction::RTypeInstruction(int rd, int rs, int rt) : m_rd(rd), m_rs(rs), m_rt(rt) {}

AddInstruction::AddInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void AddInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = rsValue + rtValue;

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AddInstruction::getName() const
{
    return "add";
}

ADDUInstruction::ADDUInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void ADDUInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = rsValue + rtValue; // 無符號加法，不檢查溢位

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string ADDUInstruction::getName() const
{
    return "addu";
}

SubInstruction::SubInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void SubInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = rsValue - rtValue;

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SubInstruction::getName() const
{
    return "sub";
}

SUBUInstruction::SUBUInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void SUBUInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = rsValue - rtValue; // 無符號減法，不檢查下溢

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SUBUInstruction::getName() const
{
    return "subu";
}

AndInstruction::AndInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void AndInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = rsValue & rtValue; // Bitwise AND operation

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AndInstruction::getName() const
{
    return "and";
}

OrInstruction::OrInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void OrInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = rsValue | rtValue; // Bitwise OR operation

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string OrInstruction::getName() const
{
    return "or";
}

XorInstruction::XorInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void XorInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = rsValue ^ rtValue; // Bitwise XOR operation

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string XorInstruction::getName() const
{
    return "xor";
}

NorInstruction::NorInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void NorInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result  = ~(rsValue | rtValue); // Bitwise NOR operation (NOT OR)

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string NorInstruction::getName() const
{
    return "nor";
}

SltInstruction::SltInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void SltInstruction::execute(Cpu& cpu)
{
    // Read values as signed integers for proper comparison
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));

    // Set rd to 1 if rs < rt, otherwise 0
    uint32_t result = (rsValue < rtValue) ? 1 : 0;

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SltInstruction::getName() const
{
    return "slt";
}

SltuInstruction::SltuInstruction(int rd, int rs, int rt) : RTypeInstruction(rd, rs, rt) {}

void SltuInstruction::execute(Cpu& cpu)
{
    // Read register values as unsigned integers for unsigned comparison
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    // Set rd to 1 if rs < rt (unsigned), otherwise 0
    uint32_t result = (rsValue < rtValue) ? 1 : 0;

    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SltuInstruction::getName() const
{
    return "sltu";
}

MULTInstruction::MULTInstruction(int rs, int rt) : m_rs(rs), m_rt(rt) {}

void MULTInstruction::execute(Cpu& cpu)
{
    // Read register values as signed integers for signed multiplication
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));

    // Perform 64-bit signed multiplication
    int64_t result = static_cast<int64_t>(rsValue) * static_cast<int64_t>(rtValue);

    // Split 64-bit result into HI (upper 32 bits) and LO (lower 32 bits)
    uint32_t hi = static_cast<uint32_t>((result >> 32) & 0xFFFFFFFF);
    uint32_t lo = static_cast<uint32_t>(result & 0xFFFFFFFF);

    // Store results in HI and LO registers
    cpu.getRegisterFile().writeHI(hi);
    cpu.getRegisterFile().writeLO(lo);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string MULTInstruction::getName() const
{
    return "mult";
}

MULTUInstruction::MULTUInstruction(int rs, int rt) : m_rs(rs), m_rt(rt) {}

void MULTUInstruction::execute(Cpu& cpu)
{
    // Read register values as unsigned integers for unsigned multiplication
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    // Perform 64-bit unsigned multiplication
    uint64_t result = static_cast<uint64_t>(rsValue) * static_cast<uint64_t>(rtValue);

    // Split 64-bit result into HI (upper 32 bits) and LO (lower 32 bits)
    uint32_t hi = static_cast<uint32_t>((result >> 32) & 0xFFFFFFFF);
    uint32_t lo = static_cast<uint32_t>(result & 0xFFFFFFFF);

    // Store results in HI and LO registers
    cpu.getRegisterFile().writeHI(hi);
    cpu.getRegisterFile().writeLO(lo);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string MULTUInstruction::getName() const
{
    return "multu";
}

DIVInstruction::DIVInstruction(int rs, int rt) : m_rs(rs), m_rt(rt) {}

void DIVInstruction::execute(Cpu& cpu)
{
    // Read register values as signed integers for signed division
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));

    if (rtValue == 0)
    {
        // Handle divide by zero - set both HI and LO to zero (undefined behavior, but safe)
        cpu.getRegisterFile().writeHI(0);
        cpu.getRegisterFile().writeLO(0);
    }
    else
    {
        // Perform signed division
        int32_t quotient  = rsValue / rtValue;
        int32_t remainder = rsValue % rtValue;

        // Store results: LO = quotient, HI = remainder
        cpu.getRegisterFile().writeLO(static_cast<uint32_t>(quotient));
        cpu.getRegisterFile().writeHI(static_cast<uint32_t>(remainder));
    }

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string DIVInstruction::getName() const
{
    return "div";
}

DIVUInstruction::DIVUInstruction(int rs, int rt) : m_rs(rs), m_rt(rt) {}

void DIVUInstruction::execute(Cpu& cpu)
{
    // Read register values as unsigned integers for unsigned division
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    if (rtValue == 0)
    {
        // Handle divide by zero - set both HI and LO to zero (undefined behavior, but safe)
        cpu.getRegisterFile().writeHI(0);
        cpu.getRegisterFile().writeLO(0);
    }
    else
    {
        // Perform unsigned division
        uint32_t quotient  = rsValue / rtValue;
        uint32_t remainder = rsValue % rtValue;

        // Store results: LO = quotient, HI = remainder
        cpu.getRegisterFile().writeLO(quotient);
        cpu.getRegisterFile().writeHI(remainder);
    }

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string DIVUInstruction::getName() const
{
    return "divu";
}

SltiInstruction::SltiInstruction(int rt, int rs, int16_t imm) : ITypeInstruction(rt, rs, imm) {}

void SltiInstruction::execute(Cpu& cpu)
{
    // Read register value as signed integer for proper comparison
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));

    // Sign-extend immediate to 32-bit for comparison
    int32_t immValue = static_cast<int32_t>(m_imm);

    // Set rt to 1 if rs < imm, otherwise 0
    uint32_t result = (rsValue < immValue) ? 1 : 0;

    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SltiInstruction::getName() const
{
    return "slti";
}

SltiuInstruction::SltiuInstruction(int rt, int rs, int16_t imm) : ITypeInstruction(rt, rs, imm) {}

void SltiuInstruction::execute(Cpu& cpu)
{
    // Read register value as unsigned integer for unsigned comparison
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // Sign-extend immediate to 32-bit, then treat as unsigned
    // MIPS 立即值會被符號擴展，但比較時當作無符號數
    uint32_t immValue = static_cast<uint32_t>(static_cast<int32_t>(m_imm));

    // Set rt to 1 if rs < imm (unsigned comparison), otherwise 0
    uint32_t result = (rsValue < immValue) ? 1 : 0;

    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SltiuInstruction::getName() const
{
    return "sltiu";
}

OriInstruction::OriInstruction(int rt, int rs, int16_t imm) : ITypeInstruction(rt, rs, imm) {}

void OriInstruction::execute(Cpu& cpu)
{
    // Read source register value
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // Zero-extend 16-bit immediate to 32-bit for OR operation
    // ORI instruction uses zero extension, not sign extension
    uint32_t immValue = static_cast<uint32_t>(static_cast<uint16_t>(m_imm));

    // Perform bitwise OR operation
    uint32_t result = rsValue | immValue;

    // Write result to target register
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string OriInstruction::getName() const
{
    return "ori";
}

AndiInstruction::AndiInstruction(int rt, int rs, int16_t imm) : ITypeInstruction(rt, rs, imm) {}

void AndiInstruction::execute(Cpu& cpu)
{
    // Read source register value
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // Zero-extend 16-bit immediate to 32-bit for AND operation
    // ANDI instruction uses zero extension, not sign extension
    uint32_t immValue = static_cast<uint32_t>(static_cast<uint16_t>(m_imm));

    // Perform bitwise AND operation
    uint32_t result = rsValue & immValue;

    // Write result to target register
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AndiInstruction::getName() const
{
    return "andi";
}

XoriInstruction::XoriInstruction(int rt, int rs, int16_t imm) : ITypeInstruction(rt, rs, imm) {}

void XoriInstruction::execute(Cpu& cpu)
{
    // 讀取源暫存器值
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // 零擴展16位立即值到32位 (XORI使用零擴展，非符號擴展)
    uint32_t immValue = static_cast<uint32_t>(static_cast<uint16_t>(m_imm));

    // 執行位元XOR運算
    uint32_t result = rsValue ^ immValue;

    // 寫入目標暫存器
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string XoriInstruction::getName() const
{
    return "xori";
}

ITypeInstruction::ITypeInstruction(int rt, int rs, int16_t imm) : m_rt(rt), m_rs(rs), m_imm(imm) {}

uint32_t ITypeInstruction::signExtend16(int16_t value)
{
    return static_cast<uint32_t>(static_cast<int32_t>(value));
}

AddiInstruction::AddiInstruction(int rt, int rs, int16_t imm) : ITypeInstruction(rt, rs, imm) {}

void AddiInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue  = cpu.getRegisterFile().read(m_rs);
    uint32_t immValue = signExtend16(m_imm);
    uint32_t result   = rsValue + immValue;

    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AddiInstruction::getName() const
{
    return "addi";
}

// ADDIU instruction implementation
ADDIUInstruction::ADDIUInstruction(int rt, int rs, int16_t imm) : ITypeInstruction(rt, rs, imm) {}

void ADDIUInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue  = cpu.getRegisterFile().read(m_rs);
    uint32_t immValue = signExtend16(m_imm); // Sign-extend despite "unsigned" name
    uint32_t result   = rsValue + immValue;  // No overflow checking - key difference from ADDI

    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string ADDIUInstruction::getName() const
{
    return "addiu";
}

LwInstruction::LwInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void LwInstruction::execute(Cpu& cpu)
{
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset      = signExtend16(m_imm);
    uint32_t address     = baseAddress + offset;

    uint32_t value = cpu.getMemory().readWord(address);
    cpu.getRegisterFile().write(m_rt, value);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LwInstruction::getName() const
{
    return "lw";
}

LBInstruction::LBInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void LBInstruction::execute(Cpu& cpu)
{
    // 1. Calculate effective address
    uint32_t baseAddress      = cpu.getRegisterFile().read(m_rs);
    int32_t  signedOffset     = static_cast<int32_t>(static_cast<int16_t>(m_imm));
    uint32_t effectiveAddress = baseAddress + signedOffset;

    // 2. Load byte (8 bits)
    uint8_t byteValue = cpu.getMemory().readByte(effectiveAddress);

    // 3. Sign-extend to 32 bits
    int32_t signExtended = static_cast<int32_t>(static_cast<int8_t>(byteValue));

    // 4. Write to target register
    cpu.getRegisterFile().write(m_rt, static_cast<uint32_t>(signExtended));

    // 5. Update PC
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LBInstruction::getName() const
{
    return "lb";
}

SBInstruction::SBInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void SBInstruction::execute(Cpu& cpu)
{
    // 1. Calculate effective address
    uint32_t baseAddress      = cpu.getRegisterFile().read(m_rs);
    int32_t  signedOffset     = static_cast<int32_t>(static_cast<int16_t>(m_imm));
    uint32_t effectiveAddress = baseAddress + signedOffset;

    // 2. Read source register and extract low 8 bits
    uint32_t sourceValue = cpu.getRegisterFile().read(m_rt);
    uint8_t  byteToStore = static_cast<uint8_t>(sourceValue & 0xFF);

    // 3. Store byte to memory
    cpu.getMemory().writeByte(effectiveAddress, byteToStore);

    // 4. Update PC
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SBInstruction::getName() const
{
    return "sb";
}

LBUInstruction::LBUInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void LBUInstruction::execute(Cpu& cpu)
{
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset      = signExtend16(m_imm);
    uint32_t address     = baseAddress + offset;

    // Load byte unsigned with zero extension
    uint8_t  byteValue         = cpu.getMemory().readByte(address);
    uint32_t zeroExtendedValue = static_cast<uint32_t>(byteValue); // Automatic zero extension

    cpu.getRegisterFile().write(m_rt, zeroExtendedValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LBUInstruction::getName() const
{
    return "lbu";
}

LHInstruction::LHInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void LHInstruction::execute(Cpu& cpu)
{
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset      = signExtend16(m_imm);
    uint32_t address     = baseAddress + offset;

    // Load halfword signed with sign extension
    uint16_t halfwordValue = cpu.getMemory().readHalfword(address);
    uint32_t signExtendedValue;

    // Check if the high bit (bit 15) is set for sign extension
    if (halfwordValue & 0x8000)
    {
        // Negative value - sign extend with 1s
        signExtendedValue = 0xFFFF0000 | halfwordValue;
    }
    else
    {
        // Positive value - sign extend with 0s
        signExtendedValue = static_cast<uint32_t>(halfwordValue);
    }

    cpu.getRegisterFile().write(m_rt, signExtendedValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LHInstruction::getName() const
{
    return "lh";
}

SHInstruction::SHInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void SHInstruction::execute(Cpu& cpu)
{
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset      = signExtend16(m_imm);
    uint32_t address     = baseAddress + offset;

    // Get the low 16 bits from rt register to store
    uint32_t registerValue = cpu.getRegisterFile().read(m_rt);
    uint16_t halfwordValue = static_cast<uint16_t>(registerValue & 0xFFFF);

    // Store halfword to memory
    cpu.getMemory().writeHalfword(address, halfwordValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SHInstruction::getName() const
{
    return "sh";
}

LHUInstruction::LHUInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void LHUInstruction::execute(Cpu& cpu)
{
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset      = signExtend16(m_imm);
    uint32_t address     = baseAddress + offset;

    // Load halfword unsigned with zero extension
    uint16_t halfwordValue     = cpu.getMemory().readHalfword(address);
    uint32_t zeroExtendedValue = static_cast<uint32_t>(halfwordValue);

    cpu.getRegisterFile().write(m_rt, zeroExtendedValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LHUInstruction::getName() const
{
    return "lhu";
}

SwInstruction::SwInstruction(int rt, int rs, int16_t offset) : ITypeInstruction(rt, rs, offset) {}

void SwInstruction::execute(Cpu& cpu)
{
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset      = signExtend16(m_imm);
    uint32_t address     = baseAddress + offset;

    uint32_t value = cpu.getRegisterFile().read(m_rt);
    cpu.getMemory().writeWord(address, value);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SwInstruction::getName() const
{
    return "sw";
}

BranchInstruction::BranchInstruction(int rs, int rt, const std::string& label)
    : m_rs(rs), m_rt(rt), m_label(label)
{
}

BeqInstruction::BeqInstruction(int rs, int rt, const std::string& label)
    : BranchInstruction(rs, rt, label)
{
}

void BeqInstruction::execute(Cpu& cpu)
{
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    if (rsValue == rtValue)
    {
        // Branch taken - jump to label
        uint32_t targetAddress = cpu.getLabelAddress(m_label);
        cpu.setProgramCounter(targetAddress);
    }
    else
    {
        // Branch not taken - continue with next instruction
        cpu.setProgramCounter(cpu.getProgramCounter() + 1);
    }
}

std::string BeqInstruction::getName() const
{
    return "beq";
}

BneInstruction::BneInstruction(int rs, int rt, int16_t offset) : ITypeInstruction(0, 0, offset)
{ // 暫時設定 rt=0, rs=0
    // BNE 指令的格式是 bne $rs, $rt, offset
    // 正確設定暫存器編號
    m_rs = rs;
    m_rt = rt;
    // m_imm 已經在基礎類別中正確設定為 offset
}

void BneInstruction::execute(Cpu& cpu)
{
    // 讀取兩個源暫存器的值
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    if (rsValue != rtValue)
    {
        // 分支條件成立：rs != rt
        // 計算分支目標地址
        // PC = PC + 4 + (sign_extend(offset) << 2)
        uint32_t currentPC          = cpu.getProgramCounter();
        int32_t  signExtendedOffset = static_cast<int32_t>(m_imm); // 符號擴展
        uint32_t targetPC           = currentPC + 4 + (signExtendedOffset << 2);

        cpu.setProgramCounter(targetPC);
    }
    else
    {
        // 分支條件不成立：rs == rt
        // 正常遞增 PC
        cpu.setProgramCounter(cpu.getProgramCounter() + 4);
    }
}

std::string BneInstruction::getName() const
{
    return "bne";
}

BLEZInstruction::BLEZInstruction(int rs, int16_t offset) : ITypeInstruction(0, rs, offset)
{
    // BLEZ 指令的格式是 blez $rs, offset
    // rt 總是 0 (未使用)，只檢查 rs 暫存器
}

void BLEZInstruction::execute(Cpu& cpu)
{
    // 讀取源暫存器的值
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // 將無符號值轉換為有符號值以進行比較
    int32_t signedRsValue = static_cast<int32_t>(rsValue);

    if (signedRsValue <= 0)
    {
        // 分支條件成立：rs <= 0
        // 計算分支目標地址
        // PC = PC + 4 + (sign_extend(offset) << 2)
        uint32_t currentPC          = cpu.getProgramCounter();
        int32_t  signExtendedOffset = static_cast<int32_t>(m_imm); // 符號擴展
        uint32_t targetPC           = currentPC + 4 + (signExtendedOffset << 2);

        cpu.setProgramCounter(targetPC);
    }
    else
    {
        // 分支條件不成立：rs > 0
        // 正常遞增 PC
        cpu.setProgramCounter(cpu.getProgramCounter() + 4);
    }
}

std::string BLEZInstruction::getName() const
{
    return "blez";
}

BGTZInstruction::BGTZInstruction(int rs, int16_t offset) : ITypeInstruction(0, rs, offset)
{
    // BGTZ 指令的格式是 bgtz $rs, offset
    // rt 總是 0 (未使用)，只檢查 rs 暫存器
}

void BGTZInstruction::execute(Cpu& cpu)
{
    // 讀取源暫存器的值
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // 將無符號值轉換為有符號值以進行比較
    int32_t signedRsValue = static_cast<int32_t>(rsValue);

    if (signedRsValue > 0)
    {
        // 分支條件成立：rs > 0
        // 計算分支目標地址
        // PC = PC + 4 + (sign_extend(offset) << 2)
        uint32_t currentPC          = cpu.getProgramCounter();
        int32_t  signExtendedOffset = static_cast<int32_t>(m_imm); // 符號擴展
        uint32_t targetPC           = currentPC + 4 + (signExtendedOffset << 2);

        cpu.setProgramCounter(targetPC);
    }
    else
    {
        // 分支條件不成立：rs <= 0
        // 正常遞增 PC
        cpu.setProgramCounter(cpu.getProgramCounter() + 4);
    }
}

std::string BGTZInstruction::getName() const
{
    return "bgtz";
}

JInstruction::JInstruction(const std::string& label) : m_label(label) {}

void JInstruction::execute(Cpu& cpu)
{
    // Unconditional jump to label
    uint32_t targetAddress = cpu.getLabelAddress(m_label);
    cpu.setProgramCounter(targetAddress);
}

std::string JInstruction::getName() const
{
    return "j";
}

SllInstruction::SllInstruction(uint32_t rd, uint32_t rt, uint32_t shamt)
    : m_rd(rd), m_rt(rt), m_shamt(shamt)
{
}

void SllInstruction::execute(Cpu& cpu)
{
    // Read source register value
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    // Perform left shift
    uint32_t result = rtValue << m_shamt;

    // Write result to destination register
    cpu.getRegisterFile().write(m_rd, result);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SllInstruction::getName() const
{
    return "sll";
}

SrlInstruction::SrlInstruction(uint32_t rd, uint32_t rt, uint32_t shamt)
    : m_rd(rd), m_rt(rt), m_shamt(shamt)
{
}

void SrlInstruction::execute(Cpu& cpu)
{
    // Read source register value
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    // Perform logical right shift (zero-fill from left)
    uint32_t result = rtValue >> m_shamt;

    // Write result to destination register
    cpu.getRegisterFile().write(m_rd, result);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SrlInstruction::getName() const
{
    return "srl";
}

SraInstruction::SraInstruction(uint32_t rd, uint32_t rt, uint32_t shamt)
    : m_rd(rd), m_rt(rt), m_shamt(shamt)
{
}

void SraInstruction::execute(Cpu& cpu)
{
    // Read source register value as signed for arithmetic shift
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));

    // Perform arithmetic right shift (sign-extend from left)
    int32_t result = rtValue >> m_shamt;

    // Write result to destination register (convert back to unsigned)
    cpu.getRegisterFile().write(m_rd, static_cast<uint32_t>(result));

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SraInstruction::getName() const
{
    return "sra";
}

SLLVInstruction::SLLVInstruction(int rd, int rt, int rs) : RTypeInstruction(rd, rs, rt) {}

void SLLVInstruction::execute(Cpu& cpu)
{
    // Read source register value to be shifted
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    // Read shift amount from rs register (only use low 5 bits)
    uint32_t rsValue     = cpu.getRegisterFile().read(m_rs);
    uint32_t shiftAmount = rsValue & 0x1F; // Mask to 0-31 range

    // Perform left shift
    uint32_t result = rtValue << shiftAmount;

    // Write result to destination register
    cpu.getRegisterFile().write(m_rd, result);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SLLVInstruction::getName() const
{
    return "sllv";
}

SRLVInstruction::SRLVInstruction(int rd, int rt, int rs) : RTypeInstruction(rd, rs, rt) {}

void SRLVInstruction::execute(Cpu& cpu)
{
    // Read source register value to be shifted
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);

    // Read shift amount from rs register (only use low 5 bits)
    uint32_t rsValue     = cpu.getRegisterFile().read(m_rs);
    uint32_t shiftAmount = rsValue & 0x1F; // Mask to 0-31 range

    // Perform logical right shift (zero extension from left)
    uint32_t result = rtValue >> shiftAmount;

    // Write result to destination register
    cpu.getRegisterFile().write(m_rd, result);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SRLVInstruction::getName() const
{
    return "srlv";
}

SRAVInstruction::SRAVInstruction(int rd, int rt, int rs) : RTypeInstruction(rd, rs, rt) {}

void SRAVInstruction::execute(Cpu& cpu)
{
    // Read source register value to be shifted (treat as signed for arithmetic shift)
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));

    // Read shift amount from rs register (only use low 5 bits)
    uint32_t rsValue     = cpu.getRegisterFile().read(m_rs);
    uint32_t shiftAmount = rsValue & 0x1F; // Mask to 0-31 range

    // Perform arithmetic right shift (sign extension from left)
    int32_t result = rtValue >> shiftAmount;

    // Write result to destination register (convert back to unsigned)
    cpu.getRegisterFile().write(m_rd, static_cast<uint32_t>(result));

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SRAVInstruction::getName() const
{
    return "srav";
}

// ===== JR Instruction =====

JRInstruction::JRInstruction(int rs) : RTypeInstruction(0, rs, 0)
{ // rd=0, rt=0 (unused for JR)
}

void JRInstruction::execute(Cpu& cpu)
{
    // Read target address from source register
    uint32_t targetAddress = cpu.getRegisterFile().read(m_rs);

    // Set PC to target address (convert to word address by dividing by 4)
    cpu.setProgramCounter(targetAddress / 4);
}

std::string JRInstruction::getName() const
{
    return "jr";
}

// ===== JAL Instruction =====

JALInstruction::JALInstruction(uint32_t target) : m_target(target) {}

void JALInstruction::execute(Cpu& cpu)
{
    // Save return address in $ra (register 31)
    uint32_t returnAddress = (cpu.getProgramCounter() + 1) * 4; // Next instruction address
    cpu.getRegisterFile().write(31, returnAddress);

    // Jump to target address
    cpu.setProgramCounter(m_target);
}

std::string JALInstruction::getName() const
{
    return "jal";
}

// ===== JALR Instruction =====

JALRInstruction::JALRInstruction(int rd, int rs) : RTypeInstruction(rd, rs, 0)
{ // rt=0 (unused for JALR)
}

void JALRInstruction::execute(Cpu& cpu)
{
    // Read target address from source register
    uint32_t targetAddress = cpu.getRegisterFile().read(m_rs);

    // Save return address in destination register
    uint32_t returnAddress = (cpu.getProgramCounter() + 1) * 4; // Next instruction address
    cpu.getRegisterFile().write(m_rd, returnAddress);

    // Jump to target address (convert to word address by dividing by 4)
    cpu.setProgramCounter(targetAddress / 4);
}

std::string JALRInstruction::getName() const
{
    return "jalr";
}

// ===== MFHI Instruction =====

MFHIInstruction::MFHIInstruction(int rd) : m_rd(rd) {}

void MFHIInstruction::execute(Cpu& cpu)
{
    // Read the value from HI register
    uint32_t hiValue = cpu.getRegisterFile().readHI();

    // Store the HI value in the destination register
    cpu.getRegisterFile().write(m_rd, hiValue);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string MFHIInstruction::getName() const
{
    return "mfhi";
}

// ===== MTHI Instruction =====

MTHIInstruction::MTHIInstruction(int rs) : m_rs(rs) {}

void MTHIInstruction::execute(Cpu& cpu)
{
    // Read the value from source register
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // Store the register value in HI register
    cpu.getRegisterFile().writeHI(rsValue);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string MTHIInstruction::getName() const
{
    return "mthi";
}

// ===== MFLO Instruction =====

MFLOInstruction::MFLOInstruction(int rd) : m_rd(rd) {}

void MFLOInstruction::execute(Cpu& cpu)
{
    // Read the value from LO register
    uint32_t loValue = cpu.getRegisterFile().readLO();

    // Store the LO value in the destination register
    cpu.getRegisterFile().write(m_rd, loValue);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string MFLOInstruction::getName() const
{
    return "mflo";
}

// ===== MTLO Instruction =====

MTLOInstruction::MTLOInstruction(int rs) : m_rs(rs) {}

void MTLOInstruction::execute(Cpu& cpu)
{
    // Read the value from source register
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);

    // Store the register value in LO register
    cpu.getRegisterFile().writeLO(rsValue);

    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string MTLOInstruction::getName() const
{
    return "mtlo";
}

// ===== Syscall Instruction =====

SyscallInstruction::SyscallInstruction() {}

void SyscallInstruction::execute(Cpu& cpu)
{
    // System call number is in $v0 (register 2)
    uint32_t syscallNumber = cpu.getRegisterFile().read(2); // $v0

    switch (syscallNumber)
    {
    case 1:
        handlePrintInt(cpu);
        break;
    case 4:
        handlePrintString(cpu);
        break;
    case 5:
        handleReadInt(cpu);
        break;
    case 10:
        handleExit(cpu);
        break;
    case 11: // print_character (avoiding conflict with original syscall 1)
        handlePrintCharacter(cpu);
        break;
    case 12: // read_character (avoiding conflict with original syscall 4)
        handleReadCharacter(cpu);
        break;
    default:
        // Unknown system call - just continue execution
        break;
    }

    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

void SyscallInstruction::handlePrintInt(Cpu& cpu)
{
    // Integer to print is in $a0 (register 4)
    uint32_t value = cpu.getRegisterFile().read(4); // $a0
    cpu.printInt(value);
}

void SyscallInstruction::handlePrintString(Cpu& cpu)
{
    // String address is in $a0 (register 4)
    uint32_t stringAddress = cpu.getRegisterFile().read(4); // $a0

    // Read string from memory character by character until null terminator
    std::string str;
    uint32_t    currentAddr = stringAddress;

    try
    {
        while (true)
        {
            uint32_t word = cpu.getMemory().readWord(currentAddr);

            // Extract bytes from word (little-endian)
            for (int i = 0; i < 4; i++)
            {
                char c = (word >> (i * 8)) & 0xFF;
                if (c == '\0')
                {
                    cpu.printString(str);
                    return;
                }
                str += c;
            }
            currentAddr += 4;
        }
    }
    catch (...)
    {
        // Memory access failed, just print what we have
        cpu.printString(str);
    }
}

void SyscallInstruction::handleReadInt(Cpu& cpu)
{
    // Read integer from input and store in $v0
    uint32_t value = cpu.readInt();
    cpu.getRegisterFile().write(2, value); // $v0
}

void SyscallInstruction::handleExit(Cpu& cpu)
{
    // Set termination flag in CPU
    cpu.terminate();
}

void SyscallInstruction::handlePrintCharacter(Cpu& cpu)
{
    // Character code to print is in $a0 (register 4)
    uint32_t charCode = cpu.getRegisterFile().read(4); // $a0
    char character = static_cast<char>(charCode & 0xFF); // Use only lower 8 bits
    cpu.printChar(character);
}

void SyscallInstruction::handleReadCharacter(Cpu& cpu)
{
    char character = cpu.readChar();
    cpu.getRegisterFile().write(2, static_cast<uint32_t>(character)); // $v0 = character
}

std::string SyscallInstruction::getName() const
{
    return "syscall";
}

// ===== LLO Instruction =====

LLOInstruction::LLOInstruction(int rt, uint16_t immediate)
    : ITypeInstruction(rt, 0, static_cast<int16_t>(immediate))
{
}

void LLOInstruction::execute(Cpu& cpu)
{
    uint32_t currentValue = cpu.getRegisterFile().read(m_rt);
    uint32_t newValue = (currentValue & 0xFFFF0000u) | (static_cast<uint32_t>(m_imm) & 0xFFFF);
    cpu.getRegisterFile().write(m_rt, newValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LLOInstruction::getName() const
{
    return "llo";
}

// ===== LHI Instruction =====

LHIInstruction::LHIInstruction(int rt, uint16_t immediate)
    : ITypeInstruction(rt, 0, static_cast<int16_t>(immediate))
{
}

void LHIInstruction::execute(Cpu& cpu)
{
    uint32_t currentValue = cpu.getRegisterFile().read(m_rt);
    uint32_t newValue = (currentValue & 0x0000FFFFu) | ((static_cast<uint32_t>(m_imm) & 0xFFFF) << 16);
    cpu.getRegisterFile().write(m_rt, newValue);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LHIInstruction::getName() const
{
    return "lhi";
}

// ===== TRAP Instruction =====

TrapInstruction::TrapInstruction(uint32_t trapCode) : m_trapCode(trapCode)
{
}

void TrapInstruction::execute(Cpu& cpu)
{
    // Generate trap exception - for now just print trap code
    cpu.printString("TRAP: " + std::to_string(m_trapCode));
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string TrapInstruction::getName() const
{
    return "trap";
}

} // namespace mips
