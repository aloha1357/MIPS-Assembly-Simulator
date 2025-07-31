#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace mips {

class Cpu;

/**
 * @brief Register information for GUI display
 */
struct RegisterInfo {
    std::string name;
    uint32_t value;
    bool hasDecimalValue;
    bool hasHexValue;
    bool isHighlighted;
};

/**
 * @brief Pipeline stage information for visualization
 */
struct PipelineStageInfo {
    std::string name;
    std::string instruction;
    bool hasValidInstruction;
    bool hasVisualRepresentation;
    bool isHighlighted;
};

/**
 * @brief Main GUI class for MIPS Assembly Simulator
 * 
 * This class provides a graphical interface for the MIPS simulator,
 * including code editing, register/memory viewing, pipeline visualization,
 * and execution controls.
 * 
 * Designed to support both real GUI (Qt/Dear ImGui) and headless testing modes.
 */
class MipsSimulatorGUI {
public:
    /**
     * @brief Constructor
     * @param headless If true, runs in headless mode for testing
     */
    explicit MipsSimulatorGUI(bool headless = false);
    
    /**
     * @brief Destructor
     */
    virtual ~MipsSimulatorGUI();

    // ────────────────────────────────────────────────
    // Core Application Management
    // ────────────────────────────────────────────────

    /**
     * @brief Initialize the GUI application
     * @return true if initialization successful
     */
    bool initialize();

    /**
     * @brief Run the main application loop
     * @return Application exit code
     */
    int run();

    /**
     * @brief Shutdown the application
     */
    void shutdown();

    // ────────────────────────────────────────────────
    // Main Window Properties
    // ────────────────────────────────────────────────

    /**
     * @brief Check if main window is visible
     */
    bool isMainWindowVisible() const;

    /**
     * @brief Get window title
     */
    std::string getWindowTitle() const;

    /**
     * @brief Set window title
     */
    void setWindowTitle(const std::string& title);

    /**
     * @brief Check if menu bar is present
     */
    bool hasMenuBar() const;

    /**
     * @brief Check if toolbar is present
     */
    bool hasToolbar() const;

    // ────────────────────────────────────────────────
    // Panel Visibility
    // ────────────────────────────────────────────────

    bool isCodeEditorVisible() const;
    bool isRegisterViewerVisible() const;
    bool isMemoryViewerVisible() const;
    bool isPipelineViewerVisible() const;
    bool isConsoleOutputVisible() const;

    void setCodeEditorVisible(bool visible);
    void setRegisterViewerVisible(bool visible);
    void setMemoryViewerVisible(bool visible);
    void setPipelineViewerVisible(bool visible);
    void setConsoleOutputVisible(bool visible);

    // ────────────────────────────────────────────────
    // Code Editor Interface
    // ────────────────────────────────────────────────

    /**
     * @brief Focus the code editor
     */
    void focusCodeEditor();

    /**
     * @brief Set text in code editor
     */
    void setCodeEditorText(const std::string& text);

    /**
     * @brief Get text from code editor
     */
    std::string getCodeEditorText() const;

    /**
     * @brief Check if syntax highlighting is enabled
     */
    bool isSyntaxHighlightingEnabled() const;

    /**
     * @brief Check if line numbers are visible
     */
    bool areLineNumbersVisible() const;

    /**
     * @brief Load file into code editor
     * @param filename Name of the file (for display)
     * @param content File content
     * @return true if successful
     */
    bool loadFile(const std::string& filename, const std::string& content);

    /**
     * @brief Save current code to file
     * @param filename Target filename
     * @return true if successful
     */
    bool saveFile(const std::string& filename);

    // ────────────────────────────────────────────────
    // Register Viewer Interface
    // ────────────────────────────────────────────────

    /**
     * @brief Get number of registers displayed
     */
    size_t getRegisterCount() const;

    /**
     * @brief Get register information for display
     * @param regNumber Register number (0-31)
     */
    RegisterInfo getRegisterInfo(int regNumber) const;

    /**
     * @brief Get register value
     * @param regNumber Register number (0-31)
     */
    uint32_t getRegisterValue(int regNumber) const;

    /**
     * @brief Check if register is highlighted
     * @param regNumber Register number (0-31)
     */
    bool isRegisterHighlighted(int regNumber) const;

    /**
     * @brief Check if PC register is visible
     */
    bool isPCRegisterVisible() const;

    /**
     * @brief Get PC value
     */
    uint32_t getPCValue() const;

    /**
     * @brief Update highlight animations
     */
    void updateHighlights();

    // ────────────────────────────────────────────────
    // Memory Viewer Interface
    // ────────────────────────────────────────────────

    bool memoryViewerShowsHexAddresses() const;
    bool memoryViewerShowsHexValues() const;
    bool memoryViewerShowsAsciiValues() const;
    bool memoryViewerSupportsScrolling() const;
    bool hasAddressJumpFeature() const;

    /**
     * @brief Get memory value at address
     * @param address Memory address
     */
    uint32_t getMemoryValue(uint32_t address) const;

    /**
     * @brief Check if memory address is highlighted
     * @param address Memory address
     */
    bool isMemoryAddressHighlighted(uint32_t address) const;

    /**
     * @brief Jump to specific memory address in viewer
     * @param address Target address
     */
    void jumpToMemoryAddress(uint32_t address);

    // ────────────────────────────────────────────────
    // Pipeline Visualization Interface
    // ────────────────────────────────────────────────

    /**
     * @brief Get all pipeline stage information
     */
    std::vector<PipelineStageInfo> getPipelineStages() const;

    /**
     * @brief Get specific pipeline stage information
     * @param stageName Stage name (IF, ID, EX, MEM, WB)
     */
    PipelineStageInfo getPipelineStage(const std::string& stageName) const;

    // ────────────────────────────────────────────────
    // Execution Control Interface
    // ────────────────────────────────────────────────

    bool isRunButtonEnabled() const;
    bool isStepButtonEnabled() const;
    bool isResetButtonEnabled() const;
    bool isStopButtonEnabled() const;

    /**
     * @brief Execute one instruction step
     */
    void stepExecution();

    /**
     * @brief Run program until completion or breakpoint
     */
    void runProgram();

    /**
     * @brief Stop program execution
     */
    void stopExecution();

    /**
     * @brief Reset simulator state
     */
    void reset();

    /**
     * @brief Load program from string
     * @param program Assembly code
     * @return true if successful
     */
    bool loadProgram(const std::string& program);

    // ────────────────────────────────────────────────
    // Console Interface
    // ────────────────────────────────────────────────

    /**
     * @brief Get console output text
     */
    std::string getConsoleOutput() const;

    /**
     * @brief Clear console output
     */
    void clearConsole();

    /**
     * @brief Add text to console
     * @param text Text to add
     * @param color Optional color (red, green, etc.)
     */
    void addConsoleText(const std::string& text, const std::string& color = "");

    // ────────────────────────────────────────────────
    // Mode Management
    // ────────────────────────────────────────────────

    /**
     * @brief Check if in pipeline mode
     */
    bool isPipelineMode() const;

    /**
     * @brief Set execution mode
     * @param pipelineMode true for pipeline, false for single-cycle
     */
    void setPipelineMode(bool pipelineMode);

    /**
     * @brief Get current mode indicator text
     */
    std::string getModeIndicator() const;

    // ────────────────────────────────────────────────
    // Error Handling
    // ────────────────────────────────────────────────

    /**
     * @brief Check if there are error messages
     */
    bool hasErrorMessages() const;

    /**
     * @brief Get error messages
     */
    std::vector<std::string> getErrorMessages() const;

    /**
     * @brief Clear error messages
     */
    void clearErrorMessages();

    /**
     * @brief Check if there are highlighted error lines
     */
    bool hasHighlightedErrorLines() const;

    /**
     * @brief Highlight error line in code editor
     * @param lineNumber Line number (1-based)
     * @param message Error message
     */
    void highlightErrorLine(int lineNumber, const std::string& message);

    // ────────────────────────────────────────────────
    // Breakpoint Management
    // ────────────────────────────────────────────────

    /**
     * @brief Set breakpoint at line
     * @param lineNumber Line number (1-based)
     */
    void setBreakpoint(int lineNumber);

    /**
     * @brief Remove breakpoint at line
     * @param lineNumber Line number (1-based)
     */
    void removeBreakpoint(int lineNumber);

    /**
     * @brief Check if breakpoint exists at line
     * @param lineNumber Line number (1-based)
     */
    bool hasBreakpoint(int lineNumber) const;

    /**
     * @brief Check if breakpoint is visible at line
     * @param lineNumber Line number (1-based)
     */
    bool isBreakpointVisible(int lineNumber) const;

    /**
     * @brief Get current execution line
     */
    int getCurrentExecutionLine() const;

    /**
     * @brief Check if execution is paused
     */
    bool isExecutionPaused() const;

    /**
     * @brief Get pipeline stage information for visualization
     */
    std::vector<PipelineStageInfo> getPipelineStageInfo() const;

    /**
     * @brief Get single byte from memory
     * @param address Memory address
     * @return Byte value
     */
    uint8_t getMemoryByte(uint32_t address) const;

    /**
     * @brief Get console text content
     * @return Console text
     */
    std::string getConsoleText() const;

    // ────────────────────────────────────────────────
    // Event Callbacks
    // ────────────────────────────────────────────────

    /**
     * @brief Set callback for when a register value changes
     */
    void setOnRegisterChangedCallback(std::function<void(int, uint32_t)> callback);

    /**
     * @brief Set callback for when memory changes
     */
    void setOnMemoryChangedCallback(std::function<void(uint32_t, uint32_t)> callback);

    /**
     * @brief Set callback for when execution state changes
     */
    void setOnExecutionStateChangedCallback(std::function<void(bool)> callback);

protected:
    // Protected access for derived classes
    bool isHeadless() const { return m_headless; }

private:
    // Implementation details hidden in private section
    class Impl;
    std::unique_ptr<Impl> m_impl;
    
    bool m_headless;
    std::unique_ptr<Cpu> m_cpu;
};

} // namespace mips
