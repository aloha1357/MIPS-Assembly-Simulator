#include "Instruction.h"
#include "Assembler.h"
#include <sstream>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: dump_labels <asm-file>\n";
        return 1;
    }
    std::ifstream f(argv[1]);
    if (!f.is_open()) {
        std::cerr << "file open failed\n";
        return 1;
    }
    std::string asmCode((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    mips::Assembler assembler;
    std::map<std::string, uint32_t> labelMap;
    std::vector<mips::DataDirective> dataDirs;
    auto instrs = assembler.assembleWithLabels(asmCode, labelMap, dataDirs);
    // Re-run the assembler's first-pass logic locally here to compute the
    // instructionAddress count that the assembler uses when placing data.
    std::istringstream sstream(asmCode);
    std::string line;
    std::vector<std::string> allLines;
    while (std::getline(sstream, line)) {
        auto t = line;
        // trim
        size_t start = t.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        size_t end = t.find_last_not_of(" \t\r\n");
        t = t.substr(start, end - start + 1);
        if (t.empty() || t[0] == '#') continue;
        allLines.push_back(t);
    }
    uint32_t instructionAddress = 0;
    bool inDataSection = false;
    for (size_t i = 0; i < allLines.size(); ++i) {
        std::string L = allLines[i];
        size_t colonPos = L.find(':');
        if (colonPos != std::string::npos) {
            // label line
            // lookahead
            bool nextLineIsData = false;
            if (i + 1 < allLines.size()) {
                std::string nextLine = allLines[i+1];
                if (nextLine.rfind(".word", 0) == 0 || nextLine.rfind(".byte", 0) == 0 || nextLine.rfind(".asciiz", 0) == 0) {
                    nextLineIsData = true;
                }
            }
            if (nextLineIsData || inDataSection) {
                // data label: do not increment
                continue;
            } else {
                // instruction label: do not increment
                continue;
            }
        }
        if (L.rfind(".word", 0) == 0 || L.rfind(".byte", 0) == 0 || L.rfind(".asciiz", 0) == 0) {
            inDataSection = true;
            continue;
        }
        if (!inDataSection) {
            instructionAddress++;
        }
    }
    std::cout << "assembler-first-pass-instructionAddress=" << instructionAddress << " parsedInstructions=" << instrs.size() << "\n";
    std::cout << "instructions=" << instrs.size() << " dataDirectives=" << dataDirs.size() << " labels=" << labelMap.size() << "\n";
    // Print instruction listing
    // Build list of original instruction source lines (exclude labels and data)
    std::vector<std::string> instrSourceLines;
    {
        bool inData = false;
        for (size_t i = 0; i < allLines.size(); ++i) {
            std::string L = allLines[i];
            size_t colonPos = L.find(':');
            if (colonPos != std::string::npos) {
                // label line - skip
                // But if label is followed by data, we will enter data section later
                continue;
            }
            if (L.rfind(".word", 0) == 0 || L.rfind(".byte", 0) == 0 || L.rfind(".asciiz", 0) == 0) {
                inData = true;
                continue;
            }
            if (!inData) {
                instrSourceLines.push_back(L);
            }
        }
    }

    for (size_t i = 0; i < instrs.size(); ++i) {
        std::string name = instrs[i] ? instrs[i]->getName() : "<null>";
        std::string src = (i < instrSourceLines.size()) ? instrSourceLines[i] : "<no-src>";
        std::cout << "idx=" << i << " \t" << name << " \t\t" << src << "\n";
    }
    for (const auto &p : labelMap) {
        std::cout << "label: '" << p.first << "' -> " << p.second << "\n";
    }
    for (const auto &d : dataDirs) {
        std::cout << "data at " << d.address << " type=" << d.type << " words=" << d.words.size() << " bytes=" << d.bytes.size() << "\n";
    }
    return 0;
}
