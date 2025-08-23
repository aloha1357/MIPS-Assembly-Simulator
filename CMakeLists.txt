cmake_minimum_required(VERSION 3.20)
project(MipsSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_QT "Build Qt GUI" OFF)  # 之後若有 GUI 再開

add_subdirectory(src)
add_subdirectory(tests)

if (ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets REQUIRED)
  add_executable(mips_gui src/gui/main_window.cpp)
  target_link_libraries(mips_gui PRIVATE Qt6::Widgets mips_core)
  target_compile_features(mips_gui PRIVATE cxx_std_20)
endif()
