# Create ImGui library (only if not in headless mode)
if(imgui_SOURCE_DIR AND NOT BUILD_HEADLESS AND OpenGL_FOUND)
    add_library(imgui_backend
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    
    target_include_directories(imgui_backend PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    
    target_link_libraries(imgui_backend PUBLIC
        OpenGL::GL
        SDL2::SDL2
        SDL2::SDL2main
    )
    
    set(IMGUI_AVAILABLE TRUE)
    message(STATUS "ImGui backend available")
else()
    set(IMGUI_AVAILABLE FALSE)
    message(STATUS "ImGui backend not available (headless mode or missing dependencies)")
endif()

# Source library (without GUI dependencies)
add_library(mips-simulator
    Cpu.cpp
    RegisterFile.cpp
    Memory.cpp
    Stage.cpp
    Instruction.cpp
    Assembler.cpp
    InstructionDecoder.cpp
    IFStage.cpp
    IDStage.cpp
    EXStage.cpp
    MEMStage.cpp
    WBStage.cpp
    gui/MipsSimulatorGUI.cpp          # Base class implementation
)

# Create separate GUI library if ImGui is available
if(IMGUI_AVAILABLE AND TARGET SDL2)
    add_library(mips-simulator-gui
        gui/ImGuiMipsSimulatorGUI.cpp    # ImGui implementation
    )
    
    target_link_libraries(mips-simulator-gui PUBLIC
        mips-simulator
        imgui_backend
        OpenGL::GL
    )
    
    target_include_directories(mips-simulator-gui
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

target_include_directories(mips-simulator
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Main CLI executable
add_executable(mips-sim
    main.cpp
)

target_link_libraries(mips-sim
    PRIVATE mips-simulator
)

# GUI executable with Dear ImGui
if(IMGUI_AVAILABLE AND TARGET SDL2)
    add_executable(mips-sim-gui
        imgui_main.cpp
    )

    target_link_libraries(mips-sim-gui
        PRIVATE 
        mips-simulator-gui
        SDL2main
        SDL2
    )
    
    # Set console subsystem for Windows
    if(WIN32)
        set_target_properties(mips-sim-gui PROPERTIES
            WIN32_EXECUTABLE FALSE  # Keep console for debug output
        )
    endif()
    
    message(STATUS "ImGui GUI build enabled")
else()
    message(STATUS "ImGui or SDL2 not found, skipping GUI build")
endif()

# Legacy GUI executable (headless/mock version)
add_executable(mips-sim-gui-legacy
    gui_main.cpp
)

target_link_libraries(mips-sim-gui-legacy
    PRIVATE mips-simulator
)
