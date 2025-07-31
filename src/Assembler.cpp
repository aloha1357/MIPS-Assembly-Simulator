#include "Assembler.h"
#include "Instruction.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

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

std::vector<std::unique_ptr<Instruction>> Assembler::assembleWithLabels(const std::string& assembly, 
                                                                       std::map<std::string, uint32_t>& labelMap) {
    std::vector<std::unique_ptr<Instruction>> instructions;
    std::vector<std::string> lines;
    std::istringstream stream(assembly);
    std::string line;
    
    // First pass: collect all lines and identify labels
    uint32_t instructionAddress = 0;
    while (std::getline(stream, line)) {
        line = trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Check if this line is a label (ends with ':')
        if (line.back() == ':') {
            std::string labelName = line.substr(0, line.length() - 1);
            labelMap[labelName] = instructionAddress;
            continue;
        }
        
        // This is an instruction line
        lines.push_back(line);
        instructionAddress++;
    }
    
    // Second pass: parse instructions
    for (const auto& instrLine : lines) {
        auto instruction = parseInstruction(instrLine);
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
    else if (opcode == "addi" && tokens.size() >= 4) {
        // Parse: addi $rt, $rs, imm
        std::string rtStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string immStr = tokens[3];
        
        // Remove commas
        if (rtStr.back() == ',') rtStr.pop_back();
        if (rsStr.back() == ',') rsStr.pop_back();
        if (immStr.back() == ',') immStr.pop_back();
        
        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);
        
        if (rt >= 0 && rs >= 0) {
            // Parse immediate value (support decimal and hex)
            int16_t imm = 0;
            try {
                if (immStr.substr(0, 2) == "0x" || immStr.substr(0, 2) == "0X") {
                    imm = static_cast<int16_t>(std::stoi(immStr, nullptr, 16));
                } else {
                    imm = static_cast<int16_t>(std::stoi(immStr));
                }
                return std::make_unique<AddiInstruction>(rt, rs, imm);
            } catch (const std::exception&) {
                // Invalid immediate value
                return nullptr;
            }
        }
    }
    else if (opcode == "lw" && tokens.size() >= 3) {
        // Parse: lw $rt, offset($rs)
        std::string rtStr = tokens[1];
        std::string offsetRegStr = tokens[2];
        
        // Remove comma from rt
        if (rtStr.back() == ',') rtStr.pop_back();
        
        int rt = getRegisterNumber(rtStr);
        if (rt >= 0) {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos) {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')') rsStr.pop_back();
                
                int rs = getRegisterNumber(rsStr);
                if (rs >= 0) {
                    try {
                        int16_t offset = 0;
                        if (!offsetStr.empty()) {
                            if (offsetStr.substr(0, 2) == "0x" || offsetStr.substr(0, 2) == "0X") {
                                offset = static_cast<int16_t>(std::stoi(offsetStr, nullptr, 16));
                            } else {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<LwInstruction>(rt, rs, offset);
                    } catch (const std::exception&) {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "sw" && tokens.size() >= 3) {
        // Parse: sw $rt, offset($rs)
        std::string rtStr = tokens[1];
        std::string offsetRegStr = tokens[2];
        
        // Remove comma from rt
        if (rtStr.back() == ',') rtStr.pop_back();
        
        int rt = getRegisterNumber(rtStr);
        if (rt >= 0) {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos) {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')') rsStr.pop_back();
                
                int rs = getRegisterNumber(rsStr);
                if (rs >= 0) {
                    try {
                        int16_t offset = 0;
                        if (!offsetStr.empty()) {
                            if (offsetStr.substr(0, 2) == "0x" || offsetStr.substr(0, 2) == "0X") {
                                offset = static_cast<int16_t>(std::stoi(offsetStr, nullptr, 16));
                            } else {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<SwInstruction>(rt, rs, offset);
                    } catch (const std::exception&) {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "beq" && tokens.size() >= 4) {
        // Parse: beq $rs, $rt, label
        std::string rsStr = tokens[1];
        std::string rtStr = tokens[2];
        std::string labelStr = tokens[3];
        
        // Remove commas
        if (rsStr.back() == ',') rsStr.pop_back();
        if (rtStr.back() == ',') rtStr.pop_back();
        if (labelStr.back() == ',') labelStr.pop_back();
        
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);
        
        if (rs >= 0 && rt >= 0) {
            return std::make_unique<BeqInstruction>(rs, rt, labelStr);
        }
    }
    else if (opcode == "j" && tokens.size() >= 2) {
        // Parse: j label
        std::string labelStr = tokens[1];
        
        // Remove comma if present
        if (labelStr.back() == ',') labelStr.pop_back();
        
        return std::make_unique<JInstruction>(labelStr);
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
