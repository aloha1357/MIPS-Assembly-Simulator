#include "MipsSimulatorGUI.h"
#include "../Cpu.h"
#include "../Memory.h"
#include "../RegisterFile.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>

namespace mips
{

/**
 * @brief Implementation class for MipsSimulatorGUI
 *
 * This implementation provides a headless/mock version for testing.
 * In a real implementation, this would integrate with Qt, Dear ImGui, or another GUI framework.
 */
class MipsSimulatorGUI::Impl
{
  public:
    explicit Impl(bool headless)
        : m_headless(headless),
          m_initialized(false),
          m_windowTitle("MIPS Assembly Simulator"),
          m_codeEditorText(""),
          m_consoleOutput(""),
          m_pipelineMode(false),
          m_executionPaused(false),
          m_currentExecutionLine(0)
    {
        // Initialize panel visibility
        m_panelVisibility["code_editor"]     = true;
        m_panelVisibility["register_viewer"] = true;
        m_panelVisibility["memory_viewer"]   = true;
        m_panelVisibility["pipeline_viewer"] = true;
        m_panelVisibility["console_output"]  = true;

        // Initialize register highlights
        m_registerHighlights.assign(32, false);

        // Initialize pipeline stages
        m_pipelineStages.resize(5);
        m_pipelineStages[0] = {"IF", "", false, true, false};
        m_pipelineStages[1] = {"ID", "", false, true, false};
        m_pipelineStages[2] = {"EX", "", false, true, false};
        m_pipelineStages[3] = {"MEM", "", false, true, false};
        m_pipelineStages[4] = {"WB", "", false, true, false};
    }

    // State variables
    bool        m_headless;
    bool        m_initialized;
    std::string m_windowTitle;
    std::string m_codeEditorText;
    std::string m_consoleOutput;
    bool        m_pipelineMode;
    bool        m_executionPaused;
    int         m_currentExecutionLine;

    // Panel visibility
    std::map<std::string, bool> m_panelVisibility;

    // Register and memory state
    std::vector<char>  m_registerHighlights;
    std::set<uint32_t> m_highlightedMemoryAddresses;

    // Pipeline visualization
    std::vector<PipelineStageInfo> m_pipelineStages;

    // Error handling
    std::vector<std::string> m_errorMessages;
    std::set<int>            m_highlightedErrorLines;

    // Breakpoints
    std::set<int> m_breakpoints;

    // Callbacks
    std::function<void(int, uint32_t)>      m_onRegisterChanged;
    std::function<void(uint32_t, uint32_t)> m_onMemoryChanged;
    std::function<void(bool)>               m_onExecutionStateChanged;
};

MipsSimulatorGUI::MipsSimulatorGUI(bool headless)
    : m_impl(std::make_unique<Impl>(headless)), m_headless(headless), m_cpu(std::make_unique<Cpu>())
{
}

MipsSimulatorGUI::~MipsSimulatorGUI() = default;

// ────────────────────────────────────────────────
// Core Application Management
// ────────────────────────────────────────────────

bool MipsSimulatorGUI::initialize()
{
    if (m_impl->m_initialized)
    {
        return true;
    }

    if (m_impl->m_headless)
    {
        // Headless mode - just mark as initialized
        m_impl->m_initialized = true;
        return true;
    }

    // TODO: In real implementation, initialize GUI framework (Qt/Dear ImGui)
    // For now, simulate successful initialization
    m_impl->m_initialized = true;

    addConsoleText("MIPS Assembly Simulator initialized", "green");
    return true;
}

int MipsSimulatorGUI::run()
{
    if (!m_impl->m_initialized)
    {
        return -1;
    }

    if (m_impl->m_headless)
    {
        // Headless mode - return immediately
        return 0;
    }

    // TODO: In real implementation, run main event loop
    // For now, simulate successful run
    return 0;
}

void MipsSimulatorGUI::shutdown()
{
    m_impl->m_initialized = false;
    // TODO: In real implementation, cleanup GUI resources
}

// ────────────────────────────────────────────────
// Main Window Properties
// ────────────────────────────────────────────────

bool MipsSimulatorGUI::isMainWindowVisible() const
{
    return m_impl->m_initialized;
}

std::string MipsSimulatorGUI::getWindowTitle() const
{
    return m_impl->m_windowTitle;
}

void MipsSimulatorGUI::setWindowTitle(const std::string& title)
{
    m_impl->m_windowTitle = title;
}

bool MipsSimulatorGUI::hasMenuBar() const
{
    return m_impl->m_initialized;
}

bool MipsSimulatorGUI::hasToolbar() const
{
    return m_impl->m_initialized;
}

// ────────────────────────────────────────────────
// Panel Visibility
// ────────────────────────────────────────────────

bool MipsSimulatorGUI::isCodeEditorVisible() const
{
    return m_impl->m_panelVisibility.at("code_editor");
}

bool MipsSimulatorGUI::isRegisterViewerVisible() const
{
    return m_impl->m_panelVisibility.at("register_viewer");
}

bool MipsSimulatorGUI::isMemoryViewerVisible() const
{
    return m_impl->m_panelVisibility.at("memory_viewer");
}

bool MipsSimulatorGUI::isPipelineViewerVisible() const
{
    return m_impl->m_panelVisibility.at("pipeline_viewer") && m_impl->m_pipelineMode;
}

bool MipsSimulatorGUI::isConsoleOutputVisible() const
{
    return m_impl->m_panelVisibility.at("console_output");
}

void MipsSimulatorGUI::setCodeEditorVisible(bool visible)
{
    m_impl->m_panelVisibility["code_editor"] = visible;
}

void MipsSimulatorGUI::setRegisterViewerVisible(bool visible)
{
    m_impl->m_panelVisibility["register_viewer"] = visible;
}

void MipsSimulatorGUI::setMemoryViewerVisible(bool visible)
{
    m_impl->m_panelVisibility["memory_viewer"] = visible;
}

void MipsSimulatorGUI::setPipelineViewerVisible(bool visible)
{
    m_impl->m_panelVisibility["pipeline_viewer"] = visible;
}

void MipsSimulatorGUI::setConsoleOutputVisible(bool visible)
{
    m_impl->m_panelVisibility["console_output"] = visible;
}

// ────────────────────────────────────────────────
// Code Editor Interface
// ────────────────────────────────────────────────

void MipsSimulatorGUI::focusCodeEditor()
{
    // TODO: In real implementation, focus the code editor widget
}

void MipsSimulatorGUI::setCodeEditorText(const std::string& text)
{
    m_impl->m_codeEditorText = text;
}

std::string MipsSimulatorGUI::getCodeEditorText() const
{
    return m_impl->m_codeEditorText;
}

bool MipsSimulatorGUI::isSyntaxHighlightingEnabled() const
{
    return true;  // Assume syntax highlighting is always enabled
}

bool MipsSimulatorGUI::areLineNumbersVisible() const
{
    return true;  // Assume line numbers are always visible
}

bool MipsSimulatorGUI::loadFile(const std::string& filename, const std::string& content)
{
    setCodeEditorText(content);
    setWindowTitle("MIPS Assembly Simulator - " + filename);
    return true;
}

bool MipsSimulatorGUI::saveFile(const std::string& filename)
{
    // TODO: In real implementation, save to actual file
    setWindowTitle("MIPS Assembly Simulator - " + filename);
    return true;
}

// ────────────────────────────────────────────────
// Register Viewer Interface
// ────────────────────────────────────────────────

size_t MipsSimulatorGUI::getRegisterCount() const
{
    return 32;
}

RegisterInfo MipsSimulatorGUI::getRegisterInfo(int regNumber) const
{
    if (regNumber < 0 || regNumber >= 32)
    {
        return {"", 0, false, false, false};
    }

    RegisterInfo info;

    // Generate register names (simplified mapping)
    if (regNumber == 0)
    {
        info.name = "$zero";
    }
    else if (regNumber >= 8 && regNumber <= 15)
    {
        info.name = "$t" + std::to_string(regNumber - 8);
    }
    else if (regNumber >= 16 && regNumber <= 23)
    {
        info.name = "$s" + std::to_string(regNumber - 16);
    }
    else if (regNumber >= 4 && regNumber <= 7)
    {
        info.name = "$a" + std::to_string(regNumber - 4);
    }
    else if (regNumber >= 2 && regNumber <= 3)
    {
        info.name = "$v" + std::to_string(regNumber - 2);
    }
    else
    {
        info.name = "$" + std::to_string(regNumber);
    }

    info.value           = m_cpu->getRegisterFile().read(regNumber);
    info.hasDecimalValue = true;
    info.hasHexValue     = true;
    info.isHighlighted   = m_impl->m_registerHighlights[regNumber];

    return info;
}

uint32_t MipsSimulatorGUI::getRegisterValue(int regNumber) const
{
    if (regNumber < 0 || regNumber >= 32)
    {
        return 0;
    }
    return m_cpu->getRegisterFile().read(regNumber);
}

bool MipsSimulatorGUI::isRegisterHighlighted(int regNumber) const
{
    if (regNumber < 0 || regNumber >= 32)
    {
        return false;
    }
    return m_impl->m_registerHighlights[regNumber];
}

bool MipsSimulatorGUI::isPCRegisterVisible() const
{
    return true;
}

void MipsSimulatorGUI::updateHighlights()
{
    // Simulate highlight fade-out
    for (size_t i = 0; i < m_impl->m_registerHighlights.size(); ++i)
    {
        m_impl->m_registerHighlights[i] = false;  // In real implementation, this would be gradual
    }
    m_impl->m_highlightedMemoryAddresses.clear();
}

// ────────────────────────────────────────────────
// Memory Viewer Interface
// ────────────────────────────────────────────────

bool MipsSimulatorGUI::memoryViewerShowsHexAddresses() const
{
    return true;
}

bool MipsSimulatorGUI::memoryViewerShowsHexValues() const
{
    return true;
}

bool MipsSimulatorGUI::memoryViewerShowsAsciiValues() const
{
    return true;
}

bool MipsSimulatorGUI::memoryViewerSupportsScrolling() const
{
    return true;
}

bool MipsSimulatorGUI::hasAddressJumpFeature() const
{
    return true;
}

uint32_t MipsSimulatorGUI::getMemoryValue(uint32_t address) const
{
    return m_cpu->getMemory().readWord(address);
}

bool MipsSimulatorGUI::isMemoryAddressHighlighted(uint32_t address) const
{
    return m_impl->m_highlightedMemoryAddresses.count(address) > 0;
}

void MipsSimulatorGUI::jumpToMemoryAddress(uint32_t /*address*/)
{
    // TODO: In real implementation, scroll memory viewer to address
}

// ────────────────────────────────────────────────
// Pipeline Visualization Interface
// ────────────────────────────────────────────────

std::vector<PipelineStageInfo> MipsSimulatorGUI::getPipelineStages() const
{
    return m_impl->m_pipelineStages;
}

PipelineStageInfo MipsSimulatorGUI::getPipelineStage(const std::string& stageName) const
{
    for (const auto& stage : m_impl->m_pipelineStages)
    {
        if (stage.name == stageName)
        {
            return stage;
        }
    }
    return {"", "", false, false, false};
}

// ────────────────────────────────────────────────
// Execution Control Interface
// ────────────────────────────────────────────────

bool MipsSimulatorGUI::isRunButtonEnabled() const
{
    return !m_impl->m_codeEditorText.empty() && !m_cpu->shouldTerminate();
}

bool MipsSimulatorGUI::isStepButtonEnabled() const
{
    return !m_impl->m_codeEditorText.empty() && !m_cpu->shouldTerminate();
}

bool MipsSimulatorGUI::isResetButtonEnabled() const
{
    return true;
}

bool MipsSimulatorGUI::isStopButtonEnabled() const
{
    return false;  // TODO: Implement execution state tracking
}

void MipsSimulatorGUI::stepExecution()
{
    if (m_cpu->shouldTerminate())
    {
        return;
    }

    // Track register changes for highlighting
    std::vector<uint32_t> oldRegValues(32);
    for (int i = 0; i < 32; ++i)
    {
        oldRegValues[i] = m_cpu->getRegisterFile().read(i);
    }

    // Store previous console output to detect new output from syscalls
    std::string previousConsoleOutput = m_cpu->getConsoleOutput();

    // Execute one step
    m_cpu->tick();

    // Check for new syscall output and add it to GUI console
    std::string currentConsoleOutput = m_cpu->getConsoleOutput();
    if (currentConsoleOutput.length() > previousConsoleOutput.length())
    {
        std::string newOutput = currentConsoleOutput.substr(previousConsoleOutput.length());
        addConsoleText(newOutput, "blue");  // Add syscall output in blue color
    }

    // Update highlights for changed registers
    for (int i = 0; i < 32; ++i)
    {
        uint32_t newValue = m_cpu->getRegisterFile().read(i);
        if (newValue != oldRegValues[i])
        {
            m_impl->m_registerHighlights[i] = true;
            if (m_impl->m_onRegisterChanged)
            {
                m_impl->m_onRegisterChanged(i, newValue);
            }
        }
    }

    // Update pipeline visualization if in pipeline mode
    if (m_impl->m_pipelineMode)
    {
        // TODO: Update pipeline stage information from CPU
        // This would require extending the CPU interface to expose pipeline state
    }

    addConsoleText("Step executed", "");
}

void MipsSimulatorGUI::runProgram()
{
    if (m_cpu->shouldTerminate())
    {
        return;
    }

    m_impl->m_executionPaused = false;

    // Run until termination or breakpoint
    while (!m_cpu->shouldTerminate() && !m_impl->m_executionPaused)
    {
        stepExecution();

        // Check for breakpoints
        uint32_t currentPC = m_cpu->getProgramCounter();
        if (m_impl->m_breakpoints.count(currentPC + 1) > 0)
        {  // +1 for 1-based line numbers
            m_impl->m_executionPaused      = true;
            m_impl->m_currentExecutionLine = currentPC + 1;
            addConsoleText("Execution paused at breakpoint", "yellow");
            break;
        }
    }

    if (m_cpu->shouldTerminate())
    {
        addConsoleText("Program execution completed", "green");
    }
}

void MipsSimulatorGUI::stopExecution()
{
    m_impl->m_executionPaused = true;
    addConsoleText("Execution stopped", "yellow");
}

void MipsSimulatorGUI::reset()
{
    m_cpu.reset();
    m_cpu = std::make_unique<Cpu>();

    // Clear all highlights
    std::fill(m_impl->m_registerHighlights.begin(), m_impl->m_registerHighlights.end(), false);
    m_impl->m_highlightedMemoryAddresses.clear();

    // Reset pipeline stages
    for (auto& stage : m_impl->m_pipelineStages)
    {
        stage.instruction         = "";
        stage.hasValidInstruction = false;
        stage.isHighlighted       = false;
    }

    m_impl->m_executionPaused      = false;
    m_impl->m_currentExecutionLine = 0;

    addConsoleText("Simulator reset", "blue");
}

bool MipsSimulatorGUI::loadProgram(const std::string& program)
{
    try
    {
        m_cpu->loadProgramFromString(program);
        setCodeEditorText(program);
        clearErrorMessages();
        addConsoleText("Program loaded successfully", "green");
        return true;
    }
    catch (const std::exception& e)
    {
        m_impl->m_errorMessages.push_back(e.what());
        addConsoleText("Error loading program: " + std::string(e.what()), "red");
        return false;
    }
}

// ────────────────────────────────────────────────
// Console Interface
// ────────────────────────────────────────────────

std::string MipsSimulatorGUI::getConsoleOutput() const
{
    return m_impl->m_consoleOutput;
}

void MipsSimulatorGUI::clearConsole()
{
    m_impl->m_consoleOutput.clear();
}

void MipsSimulatorGUI::addConsoleText(const std::string& text, const std::string& color)
{
    if (!m_impl->m_consoleOutput.empty())
    {
        m_impl->m_consoleOutput += "\n";
    }

    // In a real implementation, color would be applied via GUI formatting
    if (!color.empty())
    {
        m_impl->m_consoleOutput += "[" + color + "] ";
    }

    m_impl->m_consoleOutput += text;
}

// ────────────────────────────────────────────────
// Mode Management
// ────────────────────────────────────────────────

bool MipsSimulatorGUI::isPipelineMode() const
{
    return m_impl->m_pipelineMode;
}

void MipsSimulatorGUI::setPipelineMode(bool pipelineMode)
{
    if (m_impl->m_pipelineMode != pipelineMode)
    {
        m_impl->m_pipelineMode = pipelineMode;
        m_cpu->setPipelineMode(pipelineMode);

        if (pipelineMode)
        {
            addConsoleText("Switched to Pipeline Mode", "blue");
        }
        else
        {
            addConsoleText("Switched to Single-Cycle Mode", "blue");
        }
    }
}

std::string MipsSimulatorGUI::getModeIndicator() const
{
    return m_impl->m_pipelineMode ? "Pipeline Mode" : "Single-Cycle Mode";
}

// ────────────────────────────────────────────────
// Error Handling
// ────────────────────────────────────────────────

bool MipsSimulatorGUI::hasErrorMessages() const
{
    return !m_impl->m_errorMessages.empty();
}

std::vector<std::string> MipsSimulatorGUI::getErrorMessages() const
{
    return m_impl->m_errorMessages;
}

void MipsSimulatorGUI::clearErrorMessages()
{
    m_impl->m_errorMessages.clear();
    m_impl->m_highlightedErrorLines.clear();
}

bool MipsSimulatorGUI::hasHighlightedErrorLines() const
{
    return !m_impl->m_highlightedErrorLines.empty();
}

void MipsSimulatorGUI::highlightErrorLine(int lineNumber, const std::string& message)
{
    m_impl->m_highlightedErrorLines.insert(lineNumber);
    m_impl->m_errorMessages.push_back("Line " + std::to_string(lineNumber) + ": " + message);
}

// ────────────────────────────────────────────────
// Breakpoint Management
// ────────────────────────────────────────────────

void MipsSimulatorGUI::setBreakpoint(int lineNumber)
{
    m_impl->m_breakpoints.insert(lineNumber);
    addConsoleText("Breakpoint set at line " + std::to_string(lineNumber), "blue");
}

void MipsSimulatorGUI::removeBreakpoint(int lineNumber)
{
    m_impl->m_breakpoints.erase(lineNumber);
    addConsoleText("Breakpoint removed from line " + std::to_string(lineNumber), "blue");
}

bool MipsSimulatorGUI::hasBreakpoint(int lineNumber) const
{
    return m_impl->m_breakpoints.count(lineNumber) > 0;
}

bool MipsSimulatorGUI::isBreakpointVisible(int lineNumber) const
{
    return hasBreakpoint(lineNumber);
}

int MipsSimulatorGUI::getCurrentExecutionLine() const
{
    return m_impl->m_currentExecutionLine;
}

bool MipsSimulatorGUI::isExecutionPaused() const
{
    return m_impl->m_executionPaused;
}

std::vector<PipelineStageInfo> MipsSimulatorGUI::getPipelineStageInfo() const
{
    return m_impl->m_pipelineStages;
}

uint8_t MipsSimulatorGUI::getMemoryByte(uint32_t address) const
{
    if (m_cpu && &m_cpu->getMemory())
    {
        // Get word and extract byte
        uint32_t wordAddr   = address & ~0x3;  // Align to word boundary
        uint32_t wordData   = m_cpu->getMemory().readWord(wordAddr);
        int      byteOffset = address & 0x3;
        return (wordData >> (byteOffset * 8)) & 0xFF;
    }
    return 0;
}

uint32_t MipsSimulatorGUI::getPCValue() const
{
    return m_cpu ? m_cpu->getProgramCounter() : 0;
}

std::string MipsSimulatorGUI::getConsoleText() const
{
    return m_impl->m_consoleOutput;
}

// ────────────────────────────────────────────────
// Event Callbacks
// ────────────────────────────────────────────────

void MipsSimulatorGUI::setOnRegisterChangedCallback(std::function<void(int, uint32_t)> callback)
{
    m_impl->m_onRegisterChanged = callback;
}

void MipsSimulatorGUI::setOnMemoryChangedCallback(std::function<void(uint32_t, uint32_t)> callback)
{
    m_impl->m_onMemoryChanged = callback;
}

void MipsSimulatorGUI::setOnExecutionStateChangedCallback(std::function<void(bool)> callback)
{
    m_impl->m_onExecutionStateChanged = callback;
}

}  // namespace mips
