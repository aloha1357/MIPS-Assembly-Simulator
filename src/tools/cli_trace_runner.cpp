#include "MipsSimulatorAPI.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: cli_trace_runner <asm-file> <maxSteps>\n";
        return 1;
    }
    std::ifstream f(argv[1]);
    if (!f.is_open()) {
        std::cerr << "file open failed\n";
        return 1;
    }
    std::string asmCode((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    int maxSteps = std::stoi(argv[2]);

    mips::MipsSimulatorAPI api;
    if (!api.loadProgram(asmCode)) {
        std::cerr << "load failed: " << api.getLastError() << "\n";
        return 1;
    }

    for (int i = 0; i < maxSteps && !api.isTerminated(); ++i) {
        uint32_t pc = api.getProgramCounter();
        uint32_t ra = api.readRegister(31);
        std::cerr << "step=" << i << " pc=" << pc << " $ra=" << ra << "\n";
        api.step();
    }

    std::cout << api.getConsoleOutput();
    return 0;
}
