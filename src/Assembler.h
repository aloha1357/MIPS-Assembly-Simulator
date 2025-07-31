#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace mips {

class Instruction;

/**
 * @brief Simple assembler for MIPS instructions
 */
class Assembler {
public:
    Assembler();
    
    /**
     * @brief Parse assembly code and return list of instructions
     * @param assembly Assembly code as string
     * @return Vector of parsed instructions
     */
    std::vector<std::unique_ptr<Instruction>> assemble(const std::string& assembly);
    
    /**
     * @brief Parse assembly code with label support and return instructions + label map
     * @param assembly Assembly code as string
     * @param[out] labelMap Map of label names to instruction addresses
     * @return Vector of parsed instructions
     */
    std::vector<std::unique_ptr<Instruction>> assembleWithLabels(const std::string& assembly, 
                                                                std::map<std::string, uint32_t>& labelMap);
    
private:
    std::map<std::string, int> m_registerMap;
    
    /**
     * @brief Parse a single instruction line
     */
    std::unique_ptr<Instruction> parseInstruction(const std::string& line);
    
    /**
     * @brief Get register number from name
     */
    int getRegisterNumber(const std::string& regName);
    
    /**
     * @brief Trim whitespace from string
     */
    std::string trim(const std::string& str);
    
    /**
     * @brief Split string by delimiter
     */
    std::vector<std::string> split(const std::string& str, char delimiter);
};

} // namespace mips
