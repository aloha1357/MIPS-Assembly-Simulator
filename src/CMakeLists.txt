# Source library
add_library(mips-simulator
    Cpu.cpp
    RegisterFile.cpp
    Memory.cpp
    Stage.cpp
    Instruction.cpp
    Assembler.cpp
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
