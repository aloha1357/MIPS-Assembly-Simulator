#include "Assembler.h"
#include "Instruction.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace mips
{

Assembler::Assembler()
{
    // Initialize register name mappings
    m_registerMap["$zero"] = 0;
    m_registerMap["$at"]   = 1;
    m_registerMap["$v0"]   = 2;
    m_registerMap["$v1"]   = 3;
    m_registerMap["$a0"]   = 4;
    m_registerMap["$a1"]   = 5;
    m_registerMap["$a2"]   = 6;
    m_registerMap["$a3"]   = 7;
    m_registerMap["$t0"]   = 8;
    m_registerMap["$t1"]   = 9;
    m_registerMap["$t2"]   = 10;
    m_registerMap["$t3"]   = 11;
    m_registerMap["$t4"]   = 12;
    m_registerMap["$t5"]   = 13;
    m_registerMap["$t6"]   = 14;
    m_registerMap["$t7"]   = 15;
    m_registerMap["$s0"]   = 16;
    m_registerMap["$s1"]   = 17;
    m_registerMap["$s2"]   = 18;
    m_registerMap["$s3"]   = 19;
    m_registerMap["$s4"]   = 20;
    m_registerMap["$s5"]   = 21;
    m_registerMap["$s6"]   = 22;
    m_registerMap["$s7"]   = 23;
    m_registerMap["$t8"]   = 24;
    m_registerMap["$t9"]   = 25;
    m_registerMap["$k0"]   = 26;
    m_registerMap["$k1"]   = 27;
    m_registerMap["$gp"]   = 28;
    m_registerMap["$sp"]   = 29;
    m_registerMap["$fp"]   = 30;
    m_registerMap["$ra"]   = 31;
}

std::vector<std::unique_ptr<Instruction>> Assembler::assemble(const std::string& assembly)
{
    std::vector<std::unique_ptr<Instruction>> instructions;
    std::istringstream                        stream(assembly);
    std::string                               line;

    while (std::getline(stream, line))
    {
        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        auto instruction = parseInstruction(line);
        if (instruction)
        {
            instructions.push_back(std::move(instruction));
        }
    }

    return instructions;
}

std::vector<std::unique_ptr<Instruction>>
Assembler::assembleWithLabels(const std::string&               assembly,
                              std::map<std::string, uint32_t>& labelMap)
{
    std::vector<std::unique_ptr<Instruction>> instructions;
    std::vector<std::string>                  lines;
    std::istringstream                        stream(assembly);
    std::string                               line;

    // First pass: collect all lines and identify labels
    uint32_t instructionAddress = 0;
    while (std::getline(stream, line))
    {
        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // Check if this line is a label (ends with ':')
        if (line.back() == ':')
        {
            std::string labelName = line.substr(0, line.length() - 1);
            labelMap[labelName]   = instructionAddress;
            continue;
        }

        // This is an instruction line
        lines.push_back(line);
        instructionAddress++;
    }

    // Second pass: parse instructions
    for (const auto& instrLine : lines)
    {
        auto instruction = parseInstruction(instrLine);
        if (instruction)
        {
            instructions.push_back(std::move(instruction));
        }
    }

    return instructions;
}

std::unique_ptr<Instruction> Assembler::parseInstruction(const std::string& line)
{
    // Remove comments
    std::string cleanLine  = line;
    size_t      commentPos = cleanLine.find('#');
    if (commentPos != std::string::npos)
    {
        cleanLine = cleanLine.substr(0, commentPos);
    }
    cleanLine = trim(cleanLine);

    if (cleanLine.empty())
    {
        return nullptr;
    }

    // Split by spaces
    std::vector<std::string> tokens;
    std::istringstream       tokenStream(cleanLine);
    std::string              token;
    while (tokenStream >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.empty())
    {
        return nullptr;
    }

    std::string opcode = tokens[0];

    if (opcode == "add" && tokens.size() >= 4)
    {
        // Parse: add $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<AddInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "addu" && tokens.size() >= 4)
    {
        // Parse: addu $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<ADDUInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "and" && tokens.size() >= 4)
    {
        // Parse: and $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<AndInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "or" && tokens.size() >= 4)
    {
        // Parse: or $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<OrInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "xor" && tokens.size() >= 4)
    {
        // Parse: xor $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<XorInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "nor" && tokens.size() >= 4)
    {
        // Parse: nor $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<NorInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "sub" && tokens.size() >= 4)
    {
        // Parse: sub $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<SubInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "subu" && tokens.size() >= 4)
    {
        // Parse: subu $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<SUBUInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "sltu" && tokens.size() >= 4)
    {
        // Parse: sltu $rd, $rs, $rt
        std::string rdStr = tokens[1];
        std::string rsStr = tokens[2];
        std::string rtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rs >= 0 && rt >= 0)
        {
            return std::make_unique<SltuInstruction>(rd, rs, rt);
        }
    }
    else if (opcode == "addi" && tokens.size() >= 4)
    {
        // Parse: addi $rt, $rs, imm
        std::string rtStr  = tokens[1];
        std::string rsStr  = tokens[2];
        std::string immStr = tokens[3];

        // Remove commas
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (immStr.back() == ',')
            immStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rt >= 0 && rs >= 0)
        {
            // Parse immediate value (support decimal and hex)
            int16_t imm = 0;
            try
            {
                if (immStr.substr(0, 2) == "0x" || immStr.substr(0, 2) == "0X")
                {
                    imm = static_cast<int16_t>(std::stoi(immStr, nullptr, 16));
                }
                else
                {
                    imm = static_cast<int16_t>(std::stoi(immStr));
                }
                return std::make_unique<AddiInstruction>(rt, rs, imm);
            }
            catch (const std::exception&)
            {
                // Invalid immediate value
                return nullptr;
            }
        }
    }
    else if (opcode == "addiu" && tokens.size() >= 4)
    {
        // Parse: addiu $rt, $rs, imm
        std::string rtStr  = tokens[1];
        std::string rsStr  = tokens[2];
        std::string immStr = tokens[3];

        // Remove commas
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (immStr.back() == ',')
            immStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rt >= 0 && rs >= 0)
        {
            // Parse immediate value (support decimal and hex)
            int16_t imm = 0;
            try
            {
                if (immStr.substr(0, 2) == "0x" || immStr.substr(0, 2) == "0X")
                {
                    imm = static_cast<int16_t>(std::stoi(immStr, nullptr, 16));
                }
                else
                {
                    imm = static_cast<int16_t>(std::stoi(immStr));
                }
                return std::make_unique<ADDIUInstruction>(rt, rs, imm);
            }
            catch (const std::exception&)
            {
                // Invalid immediate value
                return nullptr;
            }
        }
    }
    else if (opcode == "andi" && tokens.size() >= 4)
    {
        // Parse: andi $rt, $rs, imm
        std::string rtStr  = tokens[1];
        std::string rsStr  = tokens[2];
        std::string immStr = tokens[3];

        // Remove commas
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (immStr.back() == ',')
            immStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rt >= 0 && rs >= 0)
        {
            // Parse immediate value (support decimal and hex)
            int16_t imm = 0;
            try
            {
                if (immStr.substr(0, 2) == "0x" || immStr.substr(0, 2) == "0X")
                {
                    imm = static_cast<int16_t>(std::stoi(immStr, nullptr, 16));
                }
                else
                {
                    imm = static_cast<int16_t>(std::stoi(immStr));
                }
                return std::make_unique<AndiInstruction>(rt, rs, imm);
            }
            catch (const std::exception&)
            {
                // Invalid immediate value
                return nullptr;
            }
        }
    }
    else if (opcode == "ori" && tokens.size() >= 4)
    {
        // Parse: ori $rt, $rs, imm
        std::string rtStr  = tokens[1];
        std::string rsStr  = tokens[2];
        std::string immStr = tokens[3];

        // Remove commas
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (immStr.back() == ',')
            immStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rt >= 0 && rs >= 0)
        {
            // Parse immediate value (support decimal and hex)
            int16_t imm = 0;
            try
            {
                if (immStr.substr(0, 2) == "0x" || immStr.substr(0, 2) == "0X")
                {
                    imm = static_cast<int16_t>(std::stoi(immStr, nullptr, 16));
                }
                else
                {
                    imm = static_cast<int16_t>(std::stoi(immStr));
                }
                return std::make_unique<OriInstruction>(rt, rs, imm);
            }
            catch (const std::exception&)
            {
                // Invalid immediate value
                return nullptr;
            }
        }
    }
    else if (opcode == "xori" && tokens.size() >= 4)
    {
        // Parse: xori $rt, $rs, imm
        std::string rtStr  = tokens[1];
        std::string rsStr  = tokens[2];
        std::string immStr = tokens[3];

        // Remove commas
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (immStr.back() == ',')
            immStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rt >= 0 && rs >= 0)
        {
            // Parse immediate value (support decimal and hex)
            int16_t imm = 0;
            try
            {
                if (immStr.substr(0, 2) == "0x" || immStr.substr(0, 2) == "0X")
                {
                    imm = static_cast<int16_t>(std::stoi(immStr, nullptr, 16));
                }
                else
                {
                    imm = static_cast<int16_t>(std::stoi(immStr));
                }
                return std::make_unique<XoriInstruction>(rt, rs, imm);
            }
            catch (const std::exception&)
            {
                // Invalid immediate value
                return nullptr;
            }
        }
    }
    else if (opcode == "sltiu" && tokens.size() >= 4)
    {
        // Parse: sltiu $rt, $rs, imm
        std::string rtStr  = tokens[1];
        std::string rsStr  = tokens[2];
        std::string immStr = tokens[3];

        // Remove commas
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (immStr.back() == ',')
            immStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rt >= 0 && rs >= 0)
        {
            // Parse immediate value (support decimal and hex)
            int16_t imm = 0;
            try
            {
                if (immStr.substr(0, 2) == "0x" || immStr.substr(0, 2) == "0X")
                {
                    imm = static_cast<int16_t>(std::stoi(immStr, nullptr, 16));
                }
                else
                {
                    imm = static_cast<int16_t>(std::stoi(immStr));
                }
                return std::make_unique<SltiuInstruction>(rt, rs, imm);
            }
            catch (const std::exception&)
            {
                // Invalid immediate value
                return nullptr;
            }
        }
    }
    else if (opcode == "lb" && tokens.size() >= 3)
    {
        // Parse: lb $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.front() == '+')
                            {
                                offsetStr = offsetStr.substr(1);
                            }
                            if (!offsetStr.empty())
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<LBInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "sb" && tokens.size() >= 3)
    {
        // Parse: sb $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.front() == '+')
                            {
                                offsetStr = offsetStr.substr(1);
                            }
                            if (!offsetStr.empty())
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<SBInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "lbu" && tokens.size() >= 3)
    {
        // Parse: lbu $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.front() == '+')
                            {
                                offsetStr = offsetStr.substr(1);
                            }
                            offset = static_cast<int16_t>(std::stoi(offsetStr));
                        }
                        return std::make_unique<LBUInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "sh" && tokens.size() >= 3)
    {
        // Parse: sh $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.front() == '+')
                            {
                                offsetStr = offsetStr.substr(1);
                            }
                            if (!offsetStr.empty())
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<SHInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "lhu" && tokens.size() >= 3)
    {
        // Parse: lhu $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.front() == '+')
                            {
                                offsetStr = offsetStr.substr(1);
                            }
                            if (!offsetStr.empty())
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<LHUInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "lh" && tokens.size() >= 3)
    {
        // Parse: lh $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.front() == '+')
                            {
                                offsetStr = offsetStr.substr(1);
                            }
                            offset = static_cast<int16_t>(std::stoi(offsetStr));
                        }
                        return std::make_unique<LHInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "lw" && tokens.size() >= 3)
    {
        // Parse: lw $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.substr(0, 2) == "0x" || offsetStr.substr(0, 2) == "0X")
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr, nullptr, 16));
                            }
                            else
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<LwInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "sw" && tokens.size() >= 3)
    {
        // Parse: sw $rt, offset($rs)
        std::string rtStr        = tokens[1];
        std::string offsetRegStr = tokens[2];

        // Remove comma from rt
        if (rtStr.back() == ',')
            rtStr.pop_back();

        int rt = getRegisterNumber(rtStr);
        if (rt >= 0)
        {
            // Parse offset($rs) format
            size_t parenPos = offsetRegStr.find('(');
            if (parenPos != std::string::npos)
            {
                std::string offsetStr = offsetRegStr.substr(0, parenPos);
                std::string rsStr     = offsetRegStr.substr(parenPos + 1);
                if (rsStr.back() == ')')
                    rsStr.pop_back();

                int rs = getRegisterNumber(rsStr);
                if (rs >= 0)
                {
                    try
                    {
                        int16_t offset = 0;
                        if (!offsetStr.empty())
                        {
                            if (offsetStr.substr(0, 2) == "0x" || offsetStr.substr(0, 2) == "0X")
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr, nullptr, 16));
                            }
                            else
                            {
                                offset = static_cast<int16_t>(std::stoi(offsetStr));
                            }
                        }
                        return std::make_unique<SwInstruction>(rt, rs, offset);
                    }
                    catch (const std::exception&)
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    else if (opcode == "beq" && tokens.size() >= 4)
    {
        // Parse: beq $rs, $rt, label
        std::string rsStr    = tokens[1];
        std::string rtStr    = tokens[2];
        std::string labelStr = tokens[3];

        // Remove commas
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (labelStr.back() == ',')
            labelStr.pop_back();

        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rs >= 0 && rt >= 0)
        {
            return std::make_unique<BeqInstruction>(rs, rt, labelStr);
        }
    }
    else if (opcode == "bne" && tokens.size() >= 4)
    {
        // Parse: bne $rs, $rt, label
        std::string rsStr    = tokens[1];
        std::string rtStr    = tokens[2];
        std::string labelStr = tokens[3];

        // Remove commas
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (labelStr.back() == ',')
            labelStr.pop_back();

        int rs = getRegisterNumber(rsStr);
        int rt = getRegisterNumber(rtStr);

        if (rs >= 0 && rt >= 0)
        {
            // For now, use a fixed offset of 4 for label resolution
            // TODO: Implement proper label-to-offset calculation
            int16_t offset = 4;
            return std::make_unique<BneInstruction>(rs, rt, offset);
        }
    }
    else if (opcode == "blez" && tokens.size() >= 3)
    {
        // Parse: blez $rs, offset
        std::string rsStr     = tokens[1];
        std::string offsetStr = tokens[2];

        // Remove commas
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (offsetStr.back() == ',')
            offsetStr.pop_back();

        int rs = getRegisterNumber(rsStr);

        if (rs >= 0)
        {
            // Parse offset value (support decimal and hex)
            int16_t offset = 0;
            try
            {
                if (offsetStr.substr(0, 2) == "0x" || offsetStr.substr(0, 2) == "0X")
                {
                    offset = static_cast<int16_t>(std::stoi(offsetStr, nullptr, 16));
                }
                else
                {
                    offset = static_cast<int16_t>(std::stoi(offsetStr));
                }
                return std::make_unique<BLEZInstruction>(rs, offset);
            }
            catch (const std::exception&)
            {
                // If numeric parsing fails, treat as label with fixed offset
                // TODO: Implement proper label-to-offset calculation
                int16_t labelOffset = 4;
                return std::make_unique<BLEZInstruction>(rs, labelOffset);
            }
        }
    }
    else if (opcode == "bgtz" && tokens.size() >= 3)
    {
        // Parse: bgtz $rs, offset
        std::string rsStr     = tokens[1];
        std::string offsetStr = tokens[2];

        // Remove commas
        if (rsStr.back() == ',')
            rsStr.pop_back();
        if (offsetStr.back() == ',')
            offsetStr.pop_back();

        int rs = getRegisterNumber(rsStr);

        if (rs >= 0)
        {
            // Parse offset value (support decimal and hex)
            int16_t offset = 0;
            try
            {
                if (offsetStr.substr(0, 2) == "0x" || offsetStr.substr(0, 2) == "0X")
                {
                    offset = static_cast<int16_t>(std::stoi(offsetStr, nullptr, 16));
                }
                else
                {
                    offset = static_cast<int16_t>(std::stoi(offsetStr));
                }
                return std::make_unique<BGTZInstruction>(rs, offset);
            }
            catch (const std::exception&)
            {
                // If numeric parsing fails, treat as label with fixed offset
                // TODO: Implement proper label-to-offset calculation
                int16_t labelOffset = 4;
                return std::make_unique<BGTZInstruction>(rs, labelOffset);
            }
        }
    }
    else if (opcode == "j" && tokens.size() >= 2)
    {
        // Parse: j label
        std::string labelStr = tokens[1];

        // Remove comma if present
        if (labelStr.back() == ',')
            labelStr.pop_back();

        return std::make_unique<JInstruction>(labelStr);
    }
    else if (opcode == "sll" && tokens.size() >= 4)
    {
        // Parse: sll $rd, $rt, shamt
        std::string rdStr    = tokens[1];
        std::string rtStr    = tokens[2];
        std::string shamtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (shamtStr.back() == ',')
            shamtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rt >= 0)
        {
            // Parse shift amount (0-31)
            try
            {
                uint32_t shamt = 0;
                if (shamtStr.substr(0, 2) == "0x" || shamtStr.substr(0, 2) == "0X")
                {
                    shamt = static_cast<uint32_t>(std::stoi(shamtStr, nullptr, 16));
                }
                else
                {
                    shamt = static_cast<uint32_t>(std::stoi(shamtStr));
                }

                // Validate shift amount (0-31)
                if (shamt <= 31)
                {
                    return std::make_unique<SllInstruction>(rd, rt, shamt);
                }
            }
            catch (const std::exception&)
            {
                // Invalid shift amount
                return nullptr;
            }
        }
    }
    else if (opcode == "srl" && tokens.size() >= 4)
    {
        // Parse: srl $rd, $rt, shamt
        std::string rdStr    = tokens[1];
        std::string rtStr    = tokens[2];
        std::string shamtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (shamtStr.back() == ',')
            shamtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rt >= 0)
        {
            // Parse shift amount (0-31)
            try
            {
                uint32_t shamt = 0;
                if (shamtStr.substr(0, 2) == "0x" || shamtStr.substr(0, 2) == "0X")
                {
                    shamt = static_cast<uint32_t>(std::stoi(shamtStr, nullptr, 16));
                }
                else
                {
                    shamt = static_cast<uint32_t>(std::stoi(shamtStr));
                }

                // Validate shift amount (0-31)
                if (shamt <= 31)
                {
                    return std::make_unique<SrlInstruction>(rd, rt, shamt);
                }
            }
            catch (const std::exception&)
            {
                // Invalid shift amount
                return nullptr;
            }
        }
    }
    else if (opcode == "sra" && tokens.size() >= 4)
    {
        // Parse: sra $rd, $rt, shamt
        std::string rdStr    = tokens[1];
        std::string rtStr    = tokens[2];
        std::string shamtStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (shamtStr.back() == ',')
            shamtStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rt = getRegisterNumber(rtStr);

        if (rd >= 0 && rt >= 0)
        {
            // Parse shift amount (0-31)
            try
            {
                uint32_t shamt = 0;
                if (shamtStr.substr(0, 2) == "0x" || shamtStr.substr(0, 2) == "0X")
                {
                    shamt = static_cast<uint32_t>(std::stoi(shamtStr, nullptr, 16));
                }
                else
                {
                    shamt = static_cast<uint32_t>(std::stoi(shamtStr));
                }

                // Validate shift amount (0-31)
                if (shamt <= 31)
                {
                    return std::make_unique<SraInstruction>(rd, rt, shamt);
                }
            }
            catch (const std::exception&)
            {
                // Invalid shift amount
                return nullptr;
            }
        }
    }
    else if (opcode == "sllv" && tokens.size() >= 4)
    {
        // Parse: sllv $rd, $rt, $rs
        std::string rdStr = tokens[1];
        std::string rtStr = tokens[2];
        std::string rsStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rd >= 0 && rt >= 0 && rs >= 0)
        {
            return std::make_unique<SLLVInstruction>(rd, rt, rs);
        }
    }
    else if (opcode == "srlv" && tokens.size() >= 4)
    {
        // Parse: srlv $rd, $rt, $rs
        std::string rdStr = tokens[1];
        std::string rtStr = tokens[2];
        std::string rsStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rd >= 0 && rt >= 0 && rs >= 0)
        {
            return std::make_unique<SRLVInstruction>(rd, rt, rs);
        }
    }
    else if (opcode == "srav" && tokens.size() >= 4)
    {
        // Parse: srav $rd, $rt, $rs
        std::string rdStr = tokens[1];
        std::string rtStr = tokens[2];
        std::string rsStr = tokens[3];

        // Remove commas
        if (rdStr.back() == ',')
            rdStr.pop_back();
        if (rtStr.back() == ',')
            rtStr.pop_back();
        if (rsStr.back() == ',')
            rsStr.pop_back();

        int rd = getRegisterNumber(rdStr);
        int rt = getRegisterNumber(rtStr);
        int rs = getRegisterNumber(rsStr);

        if (rd >= 0 && rt >= 0 && rs >= 0)
        {
            return std::make_unique<SRAVInstruction>(rd, rt, rs);
        }
    }
    else if (opcode == "jr" && tokens.size() == 2)
    {  // Exactly 2 tokens: "jr" and register
        // Parse: jr $rs
        std::string rsStr = tokens[1];

        // Remove commas if present
        if (rsStr.back() == ',')
            rsStr.pop_back();

        int rs = getRegisterNumber(rsStr);

        if (rs >= 0)
        {
            return std::make_unique<JRInstruction>(rs);
        }
    }
    else if (opcode == "jal" && tokens.size() == 2)
    {  // Exactly 2 tokens: "jal" and target
        // Parse: jal target
        std::string targetStr = tokens[1];

        // Remove commas if present
        if (targetStr.back() == ',')
            targetStr.pop_back();

        // Try to parse as numeric value (hex or decimal)
        uint32_t target = 0;
        try
        {
            if (targetStr.size() > 2 && targetStr.substr(0, 2) == "0x")
            {
                // Hexadecimal
                target = static_cast<uint32_t>(std::stoul(targetStr, nullptr, 16));
            }
            else
            {
                // Decimal
                target = static_cast<uint32_t>(std::stoul(targetStr, nullptr, 10));
            }
            return std::make_unique<JALInstruction>(target);
        }
        catch (const std::exception&)
        {
            // Failed to parse as number, return nullptr
            return nullptr;
        }
    }
    else if (opcode == "jalr" && (tokens.size() == 2 || tokens.size() == 3))
    {
        // Parse: jalr $rs (implicit $rd = $ra) or jalr $rd, $rs
        try
        {
            if (tokens.size() == 2)
            {
                // jalr $rs (default destination is $ra = $31)
                std::string rsStr = tokens[1];
                int         rs    = getRegisterNumber(rsStr);
                if (rs == -1)
                {
                    return nullptr;  // Invalid register
                }
                return std::make_unique<JALRInstruction>(31, rs);  // $ra = 31
            }
            else
            {
                // jalr $rd, $rs
                std::string rdStr = tokens[1];
                if (rdStr.back() == ',')
                {
                    rdStr.pop_back();  // Remove comma
                }
                std::string rsStr = tokens[2];

                int rd = getRegisterNumber(rdStr);
                int rs = getRegisterNumber(rsStr);
                if (rd == -1 || rs == -1)
                {
                    return nullptr;  // Invalid register
                }
                return std::make_unique<JALRInstruction>(rd, rs);
            }
        }
        catch (const std::exception&)
        {
            return nullptr;
        }
    }
    else if (opcode == "mfhi" && tokens.size() >= 2)
    {
        // Parse: mfhi $rd
        std::string rdStr = tokens[1];

        int rd = getRegisterNumber(rdStr);
        if (rd >= 0)
        {
            return std::make_unique<MFHIInstruction>(rd);
        }
    }
    else if (opcode == "mthi" && tokens.size() >= 2)
    {
        // Parse: mthi $rs
        std::string rsStr = tokens[1];

        int rs = getRegisterNumber(rsStr);
        if (rs >= 0)
        {
            return std::make_unique<MTHIInstruction>(rs);
        }
    }
    else if (opcode == "mflo" && tokens.size() >= 2)
    {
        // Parse: mflo $rd
        std::string rdStr = tokens[1];

        int rd = getRegisterNumber(rdStr);
        if (rd >= 0)
        {
            return std::make_unique<MFLOInstruction>(rd);
        }
    }
    else if (opcode == "mtlo" && tokens.size() >= 2)
    {
        // Parse: mtlo $rs
        std::string rsStr = tokens[1];

        int rs = getRegisterNumber(rsStr);
        if (rs >= 0)
        {
            return std::make_unique<MTLOInstruction>(rs);
        }
    }
    else if (opcode == "syscall")
    {
        // Parse: syscall (no arguments)
        return std::make_unique<SyscallInstruction>();
    }

    return nullptr;
}

int Assembler::getRegisterNumber(const std::string& regName)
{
    auto it = m_registerMap.find(regName);
    if (it != m_registerMap.end())
    {
        return it->second;
    }
    return -1;  // Invalid register
}

std::string Assembler::trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
    {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> Assembler::split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream        ss(str);
    std::string              token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(trim(token));
    }

    return tokens;
}

}  // namespace mips
