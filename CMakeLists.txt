cmake_minimum_required(VERSION 3.21)
project(Project)
set(CMAKE_CXX_STANDARD 14)
find_package(Catch2 REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_library(my_library lib.cpp)

add_executable(my_cli_tool cli.cpp)
target_link_libraries(my_cli_tool my_library)

add_executable(my_gui_tool gui.cpp)
target_link_libraries(my_gui_tool my_library Qt6::Core Qt6::Widgets)

add_executable(my_catch2_tests tests.cpp)
target_link_libraries(my_catch2_tests my_library Catch2::Catch2WithMain)

add_executable(my_gtest_tests tests2.cpp)
target_link_libraries(my_gtest_tests my_library GTest::gtest_main)
