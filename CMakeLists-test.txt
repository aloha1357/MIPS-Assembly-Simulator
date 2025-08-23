cmake_minimum_required(VERSION 3.21)
project(MipsSimulatorTest)
set(CMAKE_CXX_STANDARD 17)

# Only test core library compilation without external dependencies
add_subdirectory(src)

# Simple test executable to verify core compilation
add_executable(test-core-only
    src/artemis_main.cpp
)

target_link_libraries(test-core-only
    PRIVATE mips-simulator
)
