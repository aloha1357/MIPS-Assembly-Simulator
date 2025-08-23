cmake_minimum_required(VERSION 3.21)
project(MipsSimulator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Find required packages
find_package(Catch2 REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Optional packages for GUI
find_package(PkgConfig QUIET)
find_package(OpenGL QUIET)

# Option to build headless (for CI/CD)
option(BUILD_HEADLESS "Build headless version without GUI dependencies" OFF)

# Check for SDL2
if(NOT BUILD_HEADLESS)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL2 QUIET sdl2)
    endif()
    
    if(NOT SDL2_FOUND)
        find_package(SDL2 QUIET)
    endif()
endif()

# Configure imgui as external dependency
include(FetchContent)
if(NOT BUILD_HEADLESS AND SDL2_FOUND AND OpenGL_FOUND)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.5
    )
    FetchContent_MakeAvailable(imgui)
endif()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Create main library target
add_library(my_library 
    lib.cpp
)

# CLI tool
add_executable(my_cli_tool cli.cpp)
target_link_libraries(my_cli_tool my_library)

# GUI tool (Qt-based)
add_executable(my_gui_tool gui.cpp)
target_link_libraries(my_gui_tool my_library Qt6::Core Qt6::Widgets)

# Test executables
add_executable(my_catch2_tests tests.cpp)
target_link_libraries(my_catch2_tests my_library Catch2::Catch2WithMain)

add_executable(my_gtest_tests tests2.cpp)
target_link_libraries(my_gtest_tests my_library GTest::gtest_main)

# Add tests to CTest
add_test(NAME Catch2Tests COMMAND my_catch2_tests)
add_test(NAME GTestTests COMMAND my_gtest_tests)

# Formatting target using clang-format
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/*.cpp 
        ${CMAKE_SOURCE_DIR}/src/*.h 
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/*.h
    )
    
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatting source code with clang-format"
    )
endif()

# Installation rules for Artemis
install(TARGETS my_cli_tool my_gui_tool DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/assignment/report.md DESTINATION .)

# Print configuration summary
message(STATUS "=== MIPS Simulator Build Configuration ===")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Headless Build: ${BUILD_HEADLESS}")
if(NOT BUILD_HEADLESS)
    message(STATUS "SDL2 Found: ${SDL2_FOUND}")
    message(STATUS "OpenGL Found: ${OpenGL_FOUND}")
endif()
message(STATUS "Qt6 Found: ${Qt6_FOUND}")
message(STATUS "Catch2 Found: ${Catch2_FOUND}")
message(STATUS "GTest Found: ${GTest_FOUND}")
message(STATUS "==========================================")
