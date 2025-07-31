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
    add_compile_options(/W4)  # Removed /WX temporarily for GUI development
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Enable testing
enable_testing()

# Include FetchContent for dependencies
include(FetchContent)

# Find OpenGL for ImGui rendering
find_package(OpenGL REQUIRED)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.4
)
FetchContent_MakeAvailable(imgui)

# SDL2 for windowing
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5
)

# Configure SDL2 to not treat warnings as errors and reduce strict compilation
set(SDL_WERROR OFF CACHE BOOL "Treat warnings as errors" FORCE)
set(SDL_SHARED ON CACHE BOOL "Build SDL2 shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL2 static library" FORCE)

# Disable SDL2 tests and examples to speed up build
set(SDL_TEST OFF CACHE BOOL "Build tests" FORCE)
set(SDL_INSTALL_TESTS OFF CACHE BOOL "Install tests" FORCE)

FetchContent_MakeAvailable(SDL2)

# Apply relaxed compilation settings to SDL2 targets
if(TARGET SDL2-static)
    target_compile_options(SDL2-static PRIVATE
        -Wno-error
        -Wno-sign-compare
        -Wno-unused-parameter
        -Wno-cast-function-type
        -Wno-pedantic
        -Wno-strict-aliasing
    )
endif()

if(TARGET SDL2)
    target_compile_options(SDL2 PRIVATE
        -Wno-error
        -Wno-sign-compare
        -Wno-unused-parameter
        -Wno-cast-function-type
        -Wno-pedantic
        -Wno-strict-aliasing
    )
endif()

if(TARGET SDL2main)
    target_compile_options(SDL2main PRIVATE
        -Wno-error
        -Wno-sign-compare
        -Wno-unused-parameter
        -Wno-cast-function-type
        -Wno-pedantic
        -Wno-strict-aliasing
    )
endif()

# Note: Cucumber-cpp setup will be added later
# For now focusing on Google Test walking skeleton

# Source directories
add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(features)  # Will be enabled later
