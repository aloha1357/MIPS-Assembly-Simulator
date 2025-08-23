#include "gui/ImGuiMipsSimulatorGUI.h"
#include <SDL.h>
#include <iostream>

/**
 * @brief Main entry point for MIPS Assembly Simulator with Dear ImGui
 *
 * This launches the full graphical interface using Dear ImGui for
 * educational MIPS assembly programming and pipeline visualization.
 */
int SDL_main(int argc, char* argv[])
{
    std::cout << "=== MIPS Assembly Simulator - Dear ImGui Version ===" << std::endl;

    // Check if running in headless mode (for testing)
    bool headless = false;
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--headless")
        {
            headless = true;
            break;
        }
    }

    // Create Dear ImGui GUI
    mips::ImGuiMipsSimulatorGUI gui(headless);

    if (!gui.initialize())
    {
        std::cerr << "Failed to initialize Dear ImGui GUI" << std::endl;
        return 1;
    }

    // Run the application
    int result = gui.run();

    // Clean shutdown
    gui.shutdown();

    std::cout << "MIPS Simulator exited with code: " << result << std::endl;
    return result;
}
