#include "gui/MipsSimulatorGUI.h"
#include <iostream>

/**
 * @brief Main entry point for MIPS Assembly Simulator GUI
 * 
 * This demonstrates the GUI interface for the MIPS simulator.
 * In a full implementation, this would launch a Qt or Dear ImGui application.
 */
int main(int argc, char* argv[]) {
    std::cout << "=== MIPS Assembly Simulator - GUI Demo ===" << std::endl;
    
    // Check if running in headless mode (for testing)
    bool headless = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--headless") {
            headless = true;
            break;
        }
    }
    
    // Create and initialize GUI
    mips::MipsSimulatorGUI gui(headless);
    
    if (!gui.initialize()) {
        std::cerr << "Failed to initialize GUI" << std::endl;
        return 1;
    }
    
    if (headless) {
        // Headless demo mode - demonstrate functionality via console
        std::cout << "\n=== Headless Demo Mode ===" << std::endl;
        
        // Demo: Load a simple program
        std::string demoProgram = R"(addi $t0, $zero, 5
addi $t1, $zero, 10
add $t2, $t0, $t1
addi $v0, $zero, 1
add $a0, $zero, $t2
syscall
addi $v0, $zero, 10
syscall)";
        
        std::cout << "\nLoading demo program..." << std::endl;
        if (gui.loadProgram(demoProgram)) {
            std::cout << "Program loaded successfully!" << std::endl;
            
            // Demo: Show initial state
            std::cout << "\nInitial state:" << std::endl;
            std::cout << "PC: " << gui.getPCValue() << std::endl;
            std::cout << "Mode: " << gui.getModeIndicator() << std::endl;
            
            // Demo: Execute step by step
            std::cout << "\nExecuting step by step..." << std::endl;
            for (int step = 0; step < 5; ++step) {
                std::cout << "\n--- Step " << (step + 1) << " ---" << std::endl;
                gui.stepExecution();
                
                std::cout << "PC: " << gui.getPCValue() << std::endl;
                std::cout << "$t0: " << gui.getRegisterValue(8) << std::endl;
                std::cout << "$t1: " << gui.getRegisterValue(9) << std::endl;
                std::cout << "$t2: " << gui.getRegisterValue(10) << std::endl;
            }
            
            // Demo: Switch to pipeline mode
            std::cout << "\nSwitching to pipeline mode..." << std::endl;
            gui.setPipelineMode(true);
            std::cout << "Mode: " << gui.getModeIndicator() << std::endl;
            
            // Demo: Show pipeline stages
            auto stages = gui.getPipelineStages();
            std::cout << "Pipeline stages:" << std::endl;
            for (const auto& stage : stages) {
                std::cout << "  " << stage.name << ": " 
                          << (stage.hasValidInstruction ? stage.instruction : "empty") 
                          << std::endl;
            }
            
            // Demo: Reset
            std::cout << "\nResetting simulator..." << std::endl;
            gui.reset();
            std::cout << "PC after reset: " << gui.getPCValue() << std::endl;
            std::cout << "$t0 after reset: " << gui.getRegisterValue(8) << std::endl;
            
            // Demo: Console output
            std::cout << "\nConsole output:" << std::endl;
            std::cout << gui.getConsoleOutput() << std::endl;
        }
        
        gui.shutdown();
        return 0;
    }
    
    // Run GUI application
    std::cout << "Starting GUI application..." << std::endl;
    std::cout << "Note: This is a placeholder. In a full implementation," << std::endl;
    std::cout << "this would launch a Qt or Dear ImGui window." << std::endl;
    
    int result = gui.run();
    gui.shutdown();
    
    return result;
}
