#include "ImGuiMipsSimulatorGUI.h"
#include "../Assembler.h"
#include "../Cpu.h"
#include "../Memory.h"
#include "../RegisterFile.h"
#include <GL/gl.h>
#include <SDL.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <cstring>
#include <imgui.h>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace mips
{

ImGuiMipsSimulatorGUI::ImGuiMipsSimulatorGUI(bool headless)
    : MipsSimulatorGUI(headless),
      m_window(nullptr),
      m_glContext(nullptr),
      m_shouldQuit(false),
      m_showCodeEditor(true),
      m_showRegisterViewer(true),
      m_showMemoryViewer(true),
      m_showPipelineViewer(true),
      m_showConsoleOutput(true),
      m_showAbout(false),
      m_selectedLine(-1),
      m_memoryViewStart(0),
      m_memoryBytesPerRow(16)
{
    // Initialize code buffer
    memset(m_codeBuffer, 0, sizeof(m_codeBuffer));

    // Initialize components
    m_cpu       = std::make_unique<Cpu>();
    m_memory    = std::make_unique<Memory>();
    m_assembler = std::make_unique<Assembler>();

    // Connect CPU and Memory
    if (m_cpu && m_memory)
    {
        // Set memory reference for CPU if method exists
        // m_cpu->setMemory(m_memory.get());
    }
}

ImGuiMipsSimulatorGUI::~ImGuiMipsSimulatorGUI()
{
    shutdown();
}

bool ImGuiMipsSimulatorGUI::initialize()
{
    if (isHeadless())
    {
        std::cout << "Initializing GUI in headless mode" << std::endl;
        return true;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_window = SDL_CreateWindow("MIPS Assembly Simulator", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (m_window == nullptr)
    {
        std::cerr << "Error: SDL_CreateWindow(): " << SDL_GetError() << std::endl;
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::cout << "MIPS Assembly Simulator GUI initialized successfully" << std::endl;
    return true;
}

int ImGuiMipsSimulatorGUI::run()
{
    if (isHeadless())
    {
        std::cout << "Running in headless mode" << std::endl;
        return 0;
    }

    m_shouldQuit = false;

    // Main loop
    while (!m_shouldQuit)
    {
        handleEvents();
        renderFrame();
    }

    return 0;
}

void ImGuiMipsSimulatorGUI::shutdown()
{
    if (isHeadless())
    {
        return;
    }

    // Cleanup
    if (m_glContext)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
    }

    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}

void ImGuiMipsSimulatorGUI::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            m_shouldQuit = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(m_window))
            m_shouldQuit = true;
    }
}

void ImGuiMipsSimulatorGUI::renderFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Render UI components
    renderMainMenuBar();
    renderToolbar();
    renderCodeEditor();
    renderRegisterViewer();
    renderMemoryViewer();
    renderPipelineViewer();
    renderConsoleOutput();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(m_window);
}

void ImGuiMipsSimulatorGUI::renderMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                // New file action
                memset(m_codeBuffer, 0, sizeof(m_codeBuffer));
                appendConsoleOutput("New file created.\n");
            }
            if (ImGui::MenuItem("Load Demo Program"))
            {
                loadDemoProgram();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                // Open file action - would implement file dialog
                appendConsoleOutput("File open dialog not yet implemented.\n");
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                // Save file action
                appendConsoleOutput("File save dialog not yet implemented.\n");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
                m_shouldQuit = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Code Editor", nullptr, &m_showCodeEditor);
            ImGui::MenuItem("Registers", nullptr, &m_showRegisterViewer);
            ImGui::MenuItem("Memory", nullptr, &m_showMemoryViewer);
            ImGui::MenuItem("Pipeline", nullptr, &m_showPipelineViewer);
            ImGui::MenuItem("Console", nullptr, &m_showConsoleOutput);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Run"))
        {
            if (ImGui::MenuItem("Execute", "F5"))
            {
                executeCode();
            }
            if (ImGui::MenuItem("Step", "F10"))
            {
                stepExecution();
            }
            if (ImGui::MenuItem("Reset", "Ctrl+R"))
            {
                resetSimulator();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                m_showAbout = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ImGuiMipsSimulatorGUI::renderToolbar()
{
    ImGui::Begin("Toolbar", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize);

    if (ImGui::Button("Execute"))
    {
        executeCode();
    }
    ImGui::SameLine();
    if (ImGui::Button("Step"))
    {
        stepExecution();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        resetSimulator();
    }

    ImGui::End();
}

void ImGuiMipsSimulatorGUI::renderCodeEditor()
{
    if (!m_showCodeEditor)
        return;

    ImGui::Begin("Code Editor", &m_showCodeEditor);

    // Simple multiline text input for now
    ImGui::InputTextMultiline("##source", m_codeBuffer, sizeof(m_codeBuffer),
                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20),
                              ImGuiInputTextFlags_AllowTabInput);

    ImGui::End();
}

void ImGuiMipsSimulatorGUI::renderRegisterViewer()
{
    if (!m_showRegisterViewer)
        return;

    ImGui::Begin("Registers", &m_showRegisterViewer);

    if (ImGui::BeginTable("RegisterTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Register");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        if (m_cpu)
        {
            auto& regFile = m_cpu->getRegisterFile();
            for (int i = 0; i < 32; ++i)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("$%d", i);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", getRegisterName(i).c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("0x%08X", regFile.read(i));
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void ImGuiMipsSimulatorGUI::renderMemoryViewer()
{
    if (!m_showMemoryViewer)
        return;

    ImGui::Begin("Memory Viewer", &m_showMemoryViewer);

    static int startAddress = 0;
    ImGui::InputInt("Start Address", &startAddress, 16, 256, ImGuiInputTextFlags_CharsHexadecimal);

    if (ImGui::BeginTable("MemoryTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Address");
        ImGui::TableSetupColumn("+0");
        ImGui::TableSetupColumn("+4");
        ImGui::TableSetupColumn("+8");
        ImGui::TableSetupColumn("+C");
        ImGui::TableHeadersRow();

        if (m_memory)
        {
            for (int row = 0; row < 16; ++row)
            {
                ImGui::TableNextRow();
                uint32_t addr = startAddress + (row * 16);

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("0x%08X", addr);

                for (int col = 0; col < 4; ++col)
                {
                    ImGui::TableSetColumnIndex(col + 1);
                    uint32_t value = m_memory->readWord(addr + (col * 4));
                    ImGui::Text("0x%08X", value);
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void ImGuiMipsSimulatorGUI::renderPipelineViewer()
{
    if (!m_showPipelineViewer)
        return;

    ImGui::Begin("Pipeline Viewer", &m_showPipelineViewer);

    if (ImGui::BeginTable("PipelineTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Stage");
        ImGui::TableSetupColumn("Instruction");
        ImGui::TableHeadersRow();

        const char* stages[] = {"IF", "ID", "EX", "MEM", "WB"};

        for (int i = 0; i < 5; ++i)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", stages[i]);
            ImGui::TableSetColumnIndex(1);

            // Get pipeline stage information from CPU if available
            if (m_cpu)
            {
                // This would need to be implemented in the CPU class
                ImGui::Text("NOP");  // Placeholder
            }
            else
            {
                ImGui::Text("--");
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void ImGuiMipsSimulatorGUI::renderConsoleOutput()
{
    if (!m_showConsoleOutput)
        return;

    ImGui::Begin("Console Output", &m_showConsoleOutput);

    // Display console output
    ImGui::TextUnformatted(m_consoleOutput.c_str());

    // Auto-scroll to bottom
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::End();
}

void ImGuiMipsSimulatorGUI::executeCode()
{
    if (!m_assembler || !m_cpu)
    {
        appendConsoleOutput("Error: Simulator not properly initialized\n");
        return;
    }

    // Get code from the editor buffer
    std::string code(m_codeBuffer);
    if (code.empty())
    {
        appendConsoleOutput("Error: No code to execute\n");
        return;
    }

    try
    {
        appendConsoleOutput("Assembling and executing code...\n");

        // Reset CPU state before execution
        m_cpu->reset();

        // Load and execute the program
        m_cpu->loadProgramFromString(code);

        // Run until program terminates or max cycles reached
        int maxCycles = 1000;  // Prevent infinite loops
        int cycles    = 0;

        while (!m_cpu->shouldTerminate() && cycles < maxCycles)
        {
            m_cpu->tick();
            cycles++;
        }

        // Get any console output from syscalls
        std::string cpuOutput = m_cpu->getConsoleOutput();
        if (!cpuOutput.empty())
        {
            appendConsoleOutput("Program output:\n" + cpuOutput);
        }

        if (cycles >= maxCycles)
        {
            appendConsoleOutput("Warning: Program terminated after reaching maximum cycles\n");
        }
        else
        {
            appendConsoleOutput("Program executed successfully in " + std::to_string(cycles) +
                                " cycles\n");
        }
    }
    catch (const std::exception& e)
    {
        appendConsoleOutput("Error: " + std::string(e.what()) + "\n");
    }
}

void ImGuiMipsSimulatorGUI::stepExecution()
{
    if (!m_cpu)
    {
        appendConsoleOutput("Error: CPU not initialized\n");
        return;
    }

    try
    {
        // Check if we need to load code first
        std::string code(m_codeBuffer);
        if (m_cpu->getCycleCount() == 0 && !code.empty())
        {
            // First step - load the program
            m_cpu->reset();
            m_cpu->loadProgramFromString(code);
            appendConsoleOutput("Program loaded. Ready to execute.\n");
        }

        if (m_cpu->shouldTerminate())
        {
            appendConsoleOutput("Program has already terminated.\n");
            return;
        }

        // Execute one cycle
        uint32_t pcBefore = m_cpu->getProgramCounter();
        m_cpu->tick();
        uint32_t pcAfter = m_cpu->getProgramCounter();

        // Show step information
        appendConsoleOutput("Step " + std::to_string(m_cpu->getCycleCount()) + ": PC " +
                            std::to_string(pcBefore) + " -> " + std::to_string(pcAfter) + "\n");

        // Check for console output from syscalls
        std::string        cpuOutput  = m_cpu->getConsoleOutput();
        static std::string lastOutput = "";
        if (cpuOutput != lastOutput)
        {
            std::string newOutput = cpuOutput.substr(lastOutput.length());
            if (!newOutput.empty())
            {
                appendConsoleOutput("Output: " + newOutput);
                lastOutput = cpuOutput;
            }
        }

        if (m_cpu->shouldTerminate())
        {
            appendConsoleOutput("Program terminated.\n");
        }
    }
    catch (const std::exception& e)
    {
        appendConsoleOutput("Error during execution: " + std::string(e.what()) + "\n");
    }
}

void ImGuiMipsSimulatorGUI::resetSimulator()
{
    if (m_cpu)
    {
        m_cpu->reset();
    }
    appendConsoleOutput("Simulator reset.\n");
}

void ImGuiMipsSimulatorGUI::loadDemoProgram()
{
    // Simple demo program that demonstrates basic MIPS instructions
    const char* demoCode = "# MIPS Assembly Demo Program\n"
                           "# This program demonstrates basic arithmetic and system calls\n"
                           "\n"
                           "# Load two numbers\n"
                           "addi $t0, $zero, 5      # Load 5 into $t0\n"
                           "addi $t1, $zero, 10     # Load 10 into $t1\n"
                           "\n"
                           "# Arithmetic operations\n"
                           "add $t2, $t0, $t1       # $t2 = $t0 + $t1 = 15\n"
                           "sub $t3, $t1, $t0       # $t3 = $t1 - $t0 = 5\n"
                           "\n"
                           "# Print the result of addition\n"
                           "addi $v0, $zero, 1      # syscall 1: print_int\n"
                           "add $a0, $zero, $t2     # Move result to $a0\n"
                           "syscall                 # Print 15\n"
                           "\n"
                           "# Exit program\n"
                           "addi $v0, $zero, 10     # syscall 10: exit\n"
                           "syscall\n";

    // Copy demo code to buffer
    strncpy(m_codeBuffer, demoCode, sizeof(m_codeBuffer) - 1);
    m_codeBuffer[sizeof(m_codeBuffer) - 1] = '\0';

    appendConsoleOutput("Demo program loaded into editor.\n");
    appendConsoleOutput("Click 'Execute' to run the program or 'Step' to execute line by line.\n");
}

void ImGuiMipsSimulatorGUI::appendConsoleOutput(const std::string& text)
{
    m_consoleOutput += text;

    // Limit console output size to prevent memory issues
    if (m_consoleOutput.size() > 10000)
    {
        m_consoleOutput = m_consoleOutput.substr(m_consoleOutput.size() - 8000);
    }
}

std::string ImGuiMipsSimulatorGUI::getRegisterName(int regNum)
{
    const char* names[] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2",
                           "t3",   "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5",
                           "s6",   "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};

    if (regNum >= 0 && regNum < 32)
    {
        return names[regNum];
    }
    return "unknown";
}

}  // namespace mips
