# MIPS Simulator Core Library
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
    MipsSimulatorAPI.cpp             # New unified API
    gui/MipsSimulatorGUI.cpp         # Base class implementation
)

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

# Simple GUI executable (Qt6 only, no ImGui)
add_executable(mips-sim-gui-qt
    gui_main.cpp
)

target_link_libraries(mips-sim-gui-qt
    PRIVATE 
    mips-simulator
    Qt6::Core
    Qt6::Widgets
)
