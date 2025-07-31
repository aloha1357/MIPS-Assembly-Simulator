# Source library
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
    # gui/MipsSimulatorGUI.cpp  # Temporarily commented out due to encoding issues
)

target_include_directories(mips-simulator
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Main executable
add_executable(mips-sim
    main.cpp
)

target_link_libraries(mips-sim
    PRIVATE mips-simulator
)

# GUI executable (temporarily commented out due to encoding issues)
# add_executable(mips-sim-gui
#     gui_main.cpp
# )

# target_link_libraries(mips-sim-gui
#     PRIVATE mips-simulator
# )
