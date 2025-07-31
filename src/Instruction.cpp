#include "Instruction.h"
#include "Cpu.h"
#include "RegisterFile.h"

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
}

std::string SubInstruction::getName() const {
    return "sub";
}

} // namespace mips
