cmake_minimum_required(VERSION 3.21)
project(MipsSimulator)
set(CMAKE_CXX_STANDARD 17)

# Required packages for Artemis environment
find_package(Catch2 REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Add subdirectory for main source
add_subdirectory(src)

# Legacy library and tools for backward compatibility
add_library(my_library lib.cpp)

add_executable(my_cli_tool cli.cpp)
target_link_libraries(my_cli_tool my_library)

add_executable(my_gui_tool gui.cpp)
target_link_libraries(my_gui_tool my_library Qt6::Core Qt6::Widgets)

# Test executables using the actual MIPS simulator library
add_executable(my_catch2_tests tests.cpp)
target_link_libraries(my_catch2_tests mips-simulator Catch2::Catch2WithMain)

add_executable(my_gtest_tests tests2.cpp)
target_link_libraries(my_gtest_tests mips-simulator GTest::gtest_main)

# Primary Artemis-compatible executable
add_executable(mips-simulator-artemis src/artemis_main.cpp)
target_link_libraries(mips-simulator-artemis PRIVATE mips-simulator)

# Format target for clang-format
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
         src/*.cpp src/*.h src/*.hpp
         *.cpp *.h *.hpp)
    
    add_custom_target(format-code
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatting code with clang-format"
    )
endif()

# Enable testing
enable_testing()
add_test(NAME CatchTests COMMAND my_catch2_tests)
add_test(NAME GTestTests COMMAND my_gtest_tests)
