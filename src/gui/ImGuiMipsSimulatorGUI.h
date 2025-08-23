#pragma once

#include "MipsSimulatorGUI.h"
#include <memory>
#include <string>

// Forward declarations for ImGui/SDL
struct SDL_Window;
typedef void* SDL_GLContext;

// Forward declarations for Dear ImGui
struct ImGuiIO;

namespace mips
{

// Forward declarations
class Cpu;
class Memory;
class Assembler;

/**
 * @brief Dear ImGui implementation of MIPS Simulator GUI
 *
 * This class provides a real graphical interface using Dear ImGui
 * for educational MIPS assembly programming and pipeline visualization.
 */
class ImGuiMipsSimulatorGUI : public MipsSimulatorGUI
{
  public:
    explicit ImGuiMipsSimulatorGUI(bool headless = false);
    ~ImGuiMipsSimulatorGUI();

    // Core application management
    bool initialize();
    int  run();
    void shutdown();

    // ImGui-specific methods
    void renderFrame();
    void handleEvents();

  private:
    // SDL/OpenGL setup
    bool initializeSDL();
    bool initializeImGui();
    void cleanupSDL();
    void cleanupImGui();

    // UI rendering methods
    void renderMainMenuBar();
    void renderToolbar();
    void renderCodeEditor();
    void renderRegisterViewer();
    void renderMemoryViewer();
    void renderPipelineViewer();
    void renderConsoleOutput();
    void renderStatusBar();

    // Event handling
    bool handleSDLEvent();

    // Window management
    void updateWindowTitle();
    void setDefaultLayout();

    // ImGui state
    SDL_Window*   m_window;
    SDL_GLContext m_glContext;
    bool          m_shouldQuit;

    // UI state
    bool m_showCodeEditor;
    bool m_showRegisterViewer;
    bool m_showMemoryViewer;
    bool m_showPipelineViewer;
    bool m_showConsoleOutput;
    bool m_showAbout;

    // Editor state
    char m_codeBuffer[8192];  // Code editor buffer
    int  m_selectedLine;

    // Memory viewer state
    uint32_t m_memoryViewStart;
    int      m_memoryBytesPerRow;

    // Console output
    std::string m_consoleOutput;

    // Components
    std::unique_ptr<Cpu>       m_cpu;
    std::unique_ptr<Memory>    m_memory;
    std::unique_ptr<Assembler> m_assembler;

    // Style and appearance
    void setupImGuiStyle();
    void pushMipsColors();
    void popMipsColors();

  public:
    // Helper methods (made public for testing)
    void        executeCode();
    void        stepExecution();
    void        resetSimulator();
    void        loadDemoProgram();
    void        appendConsoleOutput(const std::string& text);
    std::string getRegisterName(int regNum);

  private:
};

}  // namespace mips
