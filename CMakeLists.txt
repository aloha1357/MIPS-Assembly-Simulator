cmake_minimum_required(VERSION 3.20)
project(MipsSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile only core functionality to speed up compilation
add_subdirectory(src)

# Add CLI module
add_subdirectory(cli)

# Build tests only when needed
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Qt GUI temporarily disabled to reduce compilation time
option(ENABLE_QT "Build Qt GUI" OFF)
if (ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets REQUIRED)
  add_executable(mips_gui src/gui/main_window.cpp)
  target_link_libraries(mips_gui PRIVATE Qt6::Widgets mips_core)
  target_compile_features(mips_gui PRIVATE cxx_std_20)
endif()

# ===== Code Formatting Support =====
# Find clang-format
find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format clang-format-14 clang-format-13 clang-format-12
    DOC "Path to clang-format executable"
)

if(CLANG_FORMAT_EXECUTABLE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXECUTABLE}")
    
    # Collect all C++ source files
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/*.hpp
    )
    
    # Exclude third-party code and generated files
    list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*build.*")
    list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*_deps.*")
    list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*CMakeFiles.*")
    
    # Add formatting target
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i -style=file ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting code..."
        VERBATIM
    )
    
    # Add format check target
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror -style=file ${ALL_CXX_SOURCE_FILES}
        COMMENT "Checking code format..."
        VERBATIM
    )
    
    message(STATUS "Usage:")
    message(STATUS "   cmake --build . --target format      # Format all code")
    message(STATUS "   cmake --build . --target format-check # Check code format")
    
else()
    message(STATUS "Warning: clang-format not found, code formatting unavailable")
    message(STATUS "Installation methods:")
    message(STATUS "   choco install llvm (Windows)")
    message(STATUS "   sudo apt install clang-format (Linux)")
    message(STATUS "   brew install clang-format (macOS)")
endif()
