cmake_minimum_required(VERSION 3.24)
project(mips-assembly-simulator 
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "MIPS Assembly Simulator with 5-stage pipeline"
)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Enable testing
enable_testing()

# Include FetchContent for dependencies
include(FetchContent)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Note: Cucumber-cpp setup will be added later
# For now focusing on Google Test walking skeleton

# Source directories
add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(features)  # Will be enabled later
