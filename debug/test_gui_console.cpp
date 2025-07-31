#include "src/gui/ImGuiMipsSimulatorGUI.h"
#include <iostream>

/**
 * @brief Test GUI console output functionality
 * 
 * This program tests if the GUI properly displays syscall output,
 * specifically string printing functionality.
 */
int main() {
    std::cout << "=== Testing GUI Console Output ===" << std::endl;
    
    // Create GUI in headless mode
    mips::ImGuiMipsSimulatorGUI gui(true);
    
    if (!gui.initialize()) {
        std::cerr << "Failed to initialize GUI" << std::endl;
        return 1;
    }
    
    // Test string printing program
    const char* testProgram = R"(
# Test string printing
# Store "Hello\0" at address 0x1000
addi $t0, $zero, 0x48       # 'H' 
addi $t1, $zero, 0x65       # 'e'
addi $t2, $zero, 0x6C       # 'l'
addi $t3, $zero, 0x6C       # 'l'
addi $t4, $zero, 0x6F       # 'o'

# Combine first word: "Hell"
sll $t1, $t1, 8            # shift 'e' to position 8-15
sll $t2, $t2, 16           # shift 'l' to position 16-23
sll $t3, $t3, 24           # shift 'l' to position 24-31
add $t0, $t0, $t1          # combine H + e
add $t0, $t0, $t2          # combine H + e + l
add $t0, $t0, $t3          # combine H + e + l + l
sw $t0, 0x1000($zero)      # store "Hell" at address 0x1000

# Store second word: "o\0\0\0"
sw $t4, 0x1004($zero)      # store "o\0\0\0" at address 0x1004

# Print the string
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # string address
syscall                    # Should print "Hello"

# Print integer 123
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 123       # value to print
syscall

# Exit program
addi $v0, $zero, 10        # syscall 10: exit
syscall
)";

    std::cout << "\nLoading test program..." << std::endl;
    if (!gui.loadProgram(testProgram)) {
        std::cerr << "Failed to load test program" << std::endl;
        return 1;
    }
    
    std::cout << "Program loaded successfully!" << std::endl;
    
    // Execute the program
    std::cout << "\nExecuting program..." << std::endl;
    gui.runProgram();
    
    // Get the console output
    std::string consoleOutput = gui.getConsoleOutput();
    std::cout << "\n=== Console Output ===" << std::endl;
    std::cout << consoleOutput << std::endl;
    std::cout << "======================" << std::endl;
    
    // Analyze output
    if (consoleOutput.find("Hello") != std::string::npos) {
        std::cout << "\n✓ String printing is working!" << std::endl;
    } else {
        std::cout << "\n✗ String printing failed or not visible in output" << std::endl;
    }
    
    if (consoleOutput.find("123") != std::string::npos) {
        std::cout << "✓ Integer printing is working!" << std::endl;
    } else {
        std::cout << "✗ Integer printing failed or not visible in output" << std::endl;
    }
    
    gui.shutdown();
    return 0;
}
