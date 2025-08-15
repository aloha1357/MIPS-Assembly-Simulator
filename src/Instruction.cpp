#include "Instruction.h"
#include "Cpu.h"
#include "RegisterFile.h"
#include "Memory.h"

namespace mips {

RTypeInstruction::RTypeInstruction(int rd, int rs, int rt) 
    : m_rd(rd), m_rs(rs), m_rt(rt) {
}

AddInstruction::AddInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void AddInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue + rtValue;
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AddInstruction::getName() const {
    return "add";
}

SubInstruction::SubInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void SubInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue - rtValue;
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SubInstruction::getName() const {
    return "sub";
}

AndInstruction::AndInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void AndInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue & rtValue;  // Bitwise AND operation
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AndInstruction::getName() const {
    return "and";
}

OrInstruction::OrInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void OrInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue | rtValue;  // Bitwise OR operation
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string OrInstruction::getName() const {
    return "or";
}

XorInstruction::XorInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void XorInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue ^ rtValue;  // Bitwise XOR operation
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string XorInstruction::getName() const {
    return "xor";
}

NorInstruction::NorInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void NorInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = ~(rsValue | rtValue);  // Bitwise NOR operation (NOT OR)
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string NorInstruction::getName() const {
    return "nor";
}

SltInstruction::SltInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void SltInstruction::execute(Cpu& cpu) {
    // Read values as signed integers for proper comparison
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));
    
    // Set rd to 1 if rs < rt, otherwise 0
    uint32_t result = (rsValue < rtValue) ? 1 : 0;
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SltInstruction::getName() const {
    return "slt";
}

SltuInstruction::SltuInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void SltuInstruction::execute(Cpu& cpu) {
    // Read register values as unsigned integers for unsigned comparison
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    
    // Set rd to 1 if rs < rt (unsigned), otherwise 0
    uint32_t result = (rsValue < rtValue) ? 1 : 0;
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SltuInstruction::getName() const {
    return "sltu";
}

SltiInstruction::SltiInstruction(int rt, int rs, int16_t imm)
    : ITypeInstruction(rt, rs, imm) {
}

void SltiInstruction::execute(Cpu& cpu) {
    // Read register value as signed integer for proper comparison
    int32_t rsValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rs));
    
    // Sign-extend immediate to 32-bit for comparison
    int32_t immValue = static_cast<int32_t>(m_imm);
    
    // Set rt to 1 if rs < imm, otherwise 0
    uint32_t result = (rsValue < immValue) ? 1 : 0;
    
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SltiInstruction::getName() const {
    return "slti";
}

SltiuInstruction::SltiuInstruction(int rt, int rs, int16_t imm)
    : ITypeInstruction(rt, rs, imm) {
}

void SltiuInstruction::execute(Cpu& cpu) {
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

std::string SltiuInstruction::getName() const {
    return "sltiu";
}

OriInstruction::OriInstruction(int rt, int rs, int16_t imm)
    : ITypeInstruction(rt, rs, imm) {
}

void OriInstruction::execute(Cpu& cpu) {
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

std::string OriInstruction::getName() const {
    return "ori";
}

AndiInstruction::AndiInstruction(int rt, int rs, int16_t imm)
    : ITypeInstruction(rt, rs, imm) {
}

void AndiInstruction::execute(Cpu& cpu) {
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

std::string AndiInstruction::getName() const {
    return "andi";
}

XoriInstruction::XoriInstruction(int rt, int rs, int16_t imm)
    : ITypeInstruction(rt, rs, imm) {
}

void XoriInstruction::execute(Cpu& cpu) {
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

std::string XoriInstruction::getName() const {
    return "xori";
}

ITypeInstruction::ITypeInstruction(int rt, int rs, int16_t imm)
    : m_rt(rt), m_rs(rs), m_imm(imm) {
}

uint32_t ITypeInstruction::signExtend16(int16_t value) {
    return static_cast<uint32_t>(static_cast<int32_t>(value));
}

AddiInstruction::AddiInstruction(int rt, int rs, int16_t imm)
    : ITypeInstruction(rt, rs, imm) {
}

void AddiInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t immValue = signExtend16(m_imm);
    uint32_t result = rsValue + immValue;
    
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string AddiInstruction::getName() const {
    return "addi";
}

LwInstruction::LwInstruction(int rt, int rs, int16_t offset)
    : ITypeInstruction(rt, rs, offset) {
}

void LwInstruction::execute(Cpu& cpu) {
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset = signExtend16(m_imm);
    uint32_t address = baseAddress + offset;
    
    uint32_t value = cpu.getMemory().readWord(address);
    cpu.getRegisterFile().write(m_rt, value);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string LwInstruction::getName() const {
    return "lw";
}

SwInstruction::SwInstruction(int rt, int rs, int16_t offset)
    : ITypeInstruction(rt, rs, offset) {
}

void SwInstruction::execute(Cpu& cpu) {
    uint32_t baseAddress = cpu.getRegisterFile().read(m_rs);
    uint32_t offset = signExtend16(m_imm);
    uint32_t address = baseAddress + offset;
    
    uint32_t value = cpu.getRegisterFile().read(m_rt);
    cpu.getMemory().writeWord(address, value);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SwInstruction::getName() const {
    return "sw";
}

BranchInstruction::BranchInstruction(int rs, int rt, const std::string& label)
    : m_rs(rs), m_rt(rt), m_label(label) {
}

BeqInstruction::BeqInstruction(int rs, int rt, const std::string& label)
    : BranchInstruction(rs, rt, label) {
}

void BeqInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    
    if (rsValue == rtValue) {
        // Branch taken - jump to label
        uint32_t targetAddress = cpu.getLabelAddress(m_label);
        cpu.setProgramCounter(targetAddress);
    } else {
        // Branch not taken - continue with next instruction
        cpu.setProgramCounter(cpu.getProgramCounter() + 1);
    }
}

std::string BeqInstruction::getName() const {
    return "beq";
}

BneInstruction::BneInstruction(int rs, int rt, int16_t offset)
    : ITypeInstruction(0, 0, offset) {  // 暫時設定 rt=0, rs=0
    // BNE 指令的格式是 bne $rs, $rt, offset
    // 正確設定暫存器編號
    m_rs = rs;
    m_rt = rt;
    // m_imm 已經在基礎類別中正確設定為 offset
}

void BneInstruction::execute(Cpu& cpu) {
    // 讀取兩個源暫存器的值
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    
    if (rsValue != rtValue) {
        // 分支條件成立：rs != rt
        // 計算分支目標地址
        // PC = PC + 4 + (sign_extend(offset) << 2)
        uint32_t currentPC = cpu.getProgramCounter();
        int32_t signExtendedOffset = static_cast<int32_t>(m_imm);  // 符號擴展
        uint32_t targetPC = currentPC + 4 + (signExtendedOffset << 2);
        
        cpu.setProgramCounter(targetPC);
    } else {
        // 分支條件不成立：rs == rt
        // 正常遞增 PC
        cpu.setProgramCounter(cpu.getProgramCounter() + 4);
    }
}

std::string BneInstruction::getName() const {
    return "bne";
}

BLEZInstruction::BLEZInstruction(int rs, int16_t offset)
    : ITypeInstruction(0, rs, offset) {
    // BLEZ 指令的格式是 blez $rs, offset
    // rt 總是 0 (未使用)，只檢查 rs 暫存器
}

void BLEZInstruction::execute(Cpu& cpu) {
    // 讀取源暫存器的值
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    
    // 將無符號值轉換為有符號值以進行比較
    int32_t signedRsValue = static_cast<int32_t>(rsValue);
    
    if (signedRsValue <= 0) {
        // 分支條件成立：rs <= 0
        // 計算分支目標地址
        // PC = PC + 4 + (sign_extend(offset) << 2)
        uint32_t currentPC = cpu.getProgramCounter();
        int32_t signExtendedOffset = static_cast<int32_t>(m_imm);  // 符號擴展
        uint32_t targetPC = currentPC + 4 + (signExtendedOffset << 2);
        
        cpu.setProgramCounter(targetPC);
    } else {
        // 分支條件不成立：rs > 0
        // 正常遞增 PC
        cpu.setProgramCounter(cpu.getProgramCounter() + 4);
    }
}

std::string BLEZInstruction::getName() const {
    return "blez";
}

JInstruction::JInstruction(const std::string& label)
    : m_label(label) {
}

void JInstruction::execute(Cpu& cpu) {
    // Unconditional jump to label
    uint32_t targetAddress = cpu.getLabelAddress(m_label);
    cpu.setProgramCounter(targetAddress);
}

std::string JInstruction::getName() const {
    return "j";
}

SllInstruction::SllInstruction(uint32_t rd, uint32_t rt, uint32_t shamt) 
    : m_rd(rd), m_rt(rt), m_shamt(shamt) {
}

void SllInstruction::execute(Cpu& cpu) {
    // Read source register value
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    
    // Perform left shift
    uint32_t result = rtValue << m_shamt;
    
    // Write result to destination register
    cpu.getRegisterFile().write(m_rd, result);
    
    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SllInstruction::getName() const {
    return "sll";
}

SrlInstruction::SrlInstruction(uint32_t rd, uint32_t rt, uint32_t shamt) 
    : m_rd(rd), m_rt(rt), m_shamt(shamt) {
}

void SrlInstruction::execute(Cpu& cpu) {
    // Read source register value
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    
    // Perform logical right shift (zero-fill from left)
    uint32_t result = rtValue >> m_shamt;
    
    // Write result to destination register
    cpu.getRegisterFile().write(m_rd, result);
    
    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SrlInstruction::getName() const {
    return "srl";
}

SraInstruction::SraInstruction(uint32_t rd, uint32_t rt, uint32_t shamt) 
    : m_rd(rd), m_rt(rt), m_shamt(shamt) {
}

void SraInstruction::execute(Cpu& cpu) {
    // Read source register value as signed for arithmetic shift
    int32_t rtValue = static_cast<int32_t>(cpu.getRegisterFile().read(m_rt));
    
    // Perform arithmetic right shift (sign-extend from left)
    int32_t result = rtValue >> m_shamt;
    
    // Write result to destination register (convert back to unsigned)
    cpu.getRegisterFile().write(m_rd, static_cast<uint32_t>(result));
    
    // Increment program counter
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string SraInstruction::getName() const {
    return "sra";
}

SyscallInstruction::SyscallInstruction() {
}

void SyscallInstruction::execute(Cpu& cpu) {
    // System call number is in $v0 (register 2)
    uint32_t syscallNumber = cpu.getRegisterFile().read(2); // $v0
    
    switch (syscallNumber) {
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
        default:
            // Unknown system call - just continue execution
            break;
    }
    
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

void SyscallInstruction::handlePrintInt(Cpu& cpu) {
    // Integer to print is in $a0 (register 4)
    uint32_t value = cpu.getRegisterFile().read(4); // $a0
    cpu.printInt(value);
}

void SyscallInstruction::handlePrintString(Cpu& cpu) {
    // String address is in $a0 (register 4)
    uint32_t stringAddress = cpu.getRegisterFile().read(4); // $a0
    
    // Read string from memory character by character until null terminator
    std::string str;
    uint32_t currentAddr = stringAddress;
    
    try {
        while (true) {
            uint32_t word = cpu.getMemory().readWord(currentAddr);
            
            // Extract bytes from word (little-endian)
            for (int i = 0; i < 4; i++) {
                char c = (word >> (i * 8)) & 0xFF;
                if (c == '\0') {
                    cpu.printString(str);
                    return;
                }
                str += c;
            }
            currentAddr += 4;
        }
    } catch (...) {
        // Memory access failed, just print what we have
        cpu.printString(str);
    }
}

void SyscallInstruction::handleReadInt(Cpu& cpu) {
    // Read integer from input and store in $v0
    uint32_t value = cpu.readInt();
    cpu.getRegisterFile().write(2, value); // $v0
}

void SyscallInstruction::handleExit(Cpu& cpu) {
    // Set termination flag in CPU
    cpu.terminate();
}

std::string SyscallInstruction::getName() const {
    return "syscall";
}

} // namespace mips
