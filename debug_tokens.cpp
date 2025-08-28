#include <iostream>
#include <sstream>
#include <vector>
#include <string>

int main() {
    std::string line = "lh $a0, 0 ($a1)";
    std::cout << "Original line: '" << line << "'" << std::endl;

    // Split by spaces
    std::vector<std::string> tokens;
    std::istringstream tokenStream(line);
    std::string token;
    while (tokenStream >> token) {
        tokens.push_back(token);
    }

    std::cout << "Tokens:" << std::endl;
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::cout << "  tokens[" << i << "] = '" << tokens[i] << "'" << std::endl;
    }

    if (tokens.size() >= 3) {
        std::string opcode = tokens[0];
        std::string rtStr = tokens[1];
        std::string offsetRegStr = tokens[2];

        std::cout << "opcode = '" << opcode << "'" << std::endl;
        std::cout << "rtStr = '" << rtStr << "'" << std::endl;
        std::cout << "offsetRegStr = '" << offsetRegStr << "'" << std::endl;

        // Check if rtStr has comma
        if (!rtStr.empty() && rtStr.back() == ',') {
            std::cout << "rtStr has comma, removing it..." << std::endl;
            rtStr.pop_back();
            std::cout << "rtStr after removing comma = '" << rtStr << "'" << std::endl;
        }

        // Parse offset($rs) format
        size_t parenPos = offsetRegStr.find('(');
        if (parenPos != std::string::npos) {
            std::string offsetStr = offsetRegStr.substr(0, parenPos);
            std::string rsStr = offsetRegStr.substr(parenPos + 1);
            if (!rsStr.empty() && rsStr.back() == ')') {
                rsStr.pop_back();
            }

            std::cout << "offsetStr = '" << offsetStr << "'" << std::endl;
            std::cout << "rsStr = '" << rsStr << "'" << std::endl;
        }
    }

    return 0;
}
