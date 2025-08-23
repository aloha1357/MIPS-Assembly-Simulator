#include "Cpu.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "MIPS Assembly Simulator v1.0.0\n";
    
    mips::Cpu cpu;
    
    if (argc > 1) {
        // Load program if provided
        cpu.loadProgram(argv[1]);
        cpu.run(10); // Run for 10 cycles as default
        std::cout << "Executed " << cpu.getCycleCount() << " cycles\n";
    } else {
        std::cout << "Usage: " << argv[0] << " <assembly_file>\n";
        std::cout << "Running basic test...\n";
        cpu.run(5);
        std::cout << "Test completed: " << cpu.getCycleCount() << " cycles\n";
    }
    
    return 0;
}
