#include "MipsSimulatorAPI.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: clean_runner <asm-file>\n";
        return 1;
    }
    std::ifstream f(argv[1]);
    if (!f.is_open()) {
        std::cerr << "file open failed\n";
        return 1;
    }
    std::string asmCode((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    mips::MipsSimulatorAPI api;
    if (!api.loadProgram(asmCode)) {
        std::cerr << "load failed: " << api.getLastError() << "\n";
        return 1;
    }

    // Run to completion (or until internal limit)
    api.run(0);
    // Write program console output to a UTF-8 file to avoid PowerShell
    // automatic UTF-16 redirection which corrupts byte-wise diffs.
    const std::string outPath = "tmp_clean_utf8.out";
    std::ofstream out(outPath, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "failed to open output file\n";
        return 1;
    }
    out.write(api.getConsoleOutput().data(), static_cast<std::streamsize>(api.getConsoleOutput().size()));
    out.close();

    // Also write to stdout for interactive inspection (may be captured by shell differently)
    std::cout << api.getConsoleOutput();
    return 0;
}
