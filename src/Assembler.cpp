#include "Assembler.h"
#include "Instruction.h"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace mips {

Assembler::Assembler() {
    // Initialize register name mappings
    m_registerMap["$zero"] = 0; m_registerMap["$at"] = 1;
    m_registerMap["$v0"] = 2; m_registerMap["$v1"] = 3;
    m_registerMap["$a0"] = 4; m_registerMap["$a1"] = 5; 
    m_registerMap["$a2"] = 6; m_registerMap["$a3"] = 7;
    m_registerMap["$t0"] = 8; m_registerMap["$t1"] = 9; 
    m_registerMap["$t2"] = 10; m_registerMap["$t3"] = 11;
    m_registerMap["$t4"] = 12; m_registerMap["$t5"] = 13; 
    m_registerMap["$t6"] = 14; m_registerMap["$t7"] = 15;
    m_registerMap["$s0"] = 16; m_registerMap["$s1"] = 17; 
    m_registerMap["$s2"] = 18; m_registerMap["$s3"] = 19;
    m_registerMap["$s4"] = 20; m_registerMap["$s5"] = 21; 
    m_registerMap["$s6"] = 22; m_registerMap["$s7"] = 23;
    m_registerMap["$t8"] = 24; m_registerMap["$t9"] = 25;
    m_registerMap["$k0"] = 26; m_registerMap["$k1"] = 27;
    m_registerMap["$gp"] = 28; m_registerMap["$sp"] = 29; 
    m_registerMap["$fp"] = 30; m_registerMap["$ra"] = 31;
}

std::vector<std::unique_ptr<Instruction>> Assembler::assemble(const std::string& assembly) {
    std::vector<std::unique_ptr<Instruction>> instructions;
    std::istringstream stream(assembly);
    std::string line;
    
    while (std::getline(stream, line)) {
        line = trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        auto instruction = parseInstruction(line);
        if (instruction) {
            instructions.push_back(std::move(instruction));
        }
    }
    
    return instructions;
}

std::unique_ptr<Instruction> Assembler::parseInstruction(const std::string& line) {
    // Remove comments
    std::string cleanLine = line;
    size_t commentPos = cleanLine.find('#');
    if (commentPos != std::string::npos) {
        cleanLine = cleanLine.substr(0, commentPos);
    }
    cleanLine = trim(cleanLine);
    
    if (cleanLine.empty()) {
        return nullptr;
    }
    
    // Split by spaces
    std::vector<std::string> tokens;
    std::istringstream tokenStream(cleanLine);
    std::string token;
    while (tokenStream >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.empty()) {
        return nullptr;
    }
    
    std::string opcode = tokens[0];
    
    if (opcode == "add" && tokens.size() >= 4) {
        // Parse: add $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];
        
        // Remove commas
        if (rdStr.back() == ',') rdStr.pop_back();
        if (rsStr.back() == ',') rsStr.pop_back();
        if (rtStr.back() == ',') rtStr.pop_back();
        
        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);
        
        if (rd >= 0 && rs >= 0 && rt >= 0) {
            return std::make_unique<AddInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "sub" && tokens.size() >= 4) {
        // Parse: sub $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];
        
        // Remove commas
        if (rdStr.back() == ',') rdStr.pop_back();
        if (rsStr.back() == ',') rsStr.pop_back();
        if (rtStr.back() == ',') rtStr.pop_back();
        
        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);
        
        if (rd >= 0 && rs >= 0 && rt >= 0) {
            return std::make_unique<SubInstruction>(rd, rs, rt);
        }
    }
    
    return nullptr;
}

int Assembler::getRegisterNumber(const std::string& regName) {
    auto it = m_registerMap.find(regName);
    if (it != m_registerMap.end()) {
        return it->second;
    }
    return -1; // Invalid register
}

std::string Assembler::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> Assembler::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    
    return tokens;
}

} // namespace mips
