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
    # Use stricter warnings in Debug mode, more lenient in Release
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

# Enable testing
enable_testing()

# Include FetchContent for dependencies
include(FetchContent)

# Find OpenGL for ImGui rendering (optional for CI builds)
find_package(OpenGL)

# Check if we're in a headless environment (CI)
option(BUILD_HEADLESS "Build without GUI support for CI/CD" OFF)

if(NOT OpenGL_FOUND)
    message(WARNING "OpenGL not found, building in headless mode")
    set(BUILD_HEADLESS ON)
endif()

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Dear ImGui (only if not headless)
if(NOT BUILD_HEADLESS)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.90.4
    )
    FetchContent_MakeAvailable(imgui)
endif()

# SDL2 for windowing (only if not headless)
if(NOT BUILD_HEADLESS)
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
            -Wno-unused-variable
        )
    endif()
    
    message(STATUS "SDL2 configured for GUI builds")
else()
    message(STATUS "Skipping SDL2 configuration (headless mode)")
endif()

# Note: Cucumber-cpp setup will be added later
# For now focusing on Google Test walking skeleton

# Source directories
add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(features)  # Will be enabled later
