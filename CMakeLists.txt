cmake_minimum_required(VERSION 3.20)
project(MipsSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 為了加速編譯，先只編譯核心功能
add_subdirectory(src)

# 添加 CLI 模塊
add_subdirectory(cli)

# 只在需要時編譯測試
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Qt GUI 暫時關閉以減少編譯時間
option(ENABLE_QT "Build Qt GUI" OFF)
if (ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets REQUIRED)
  add_executable(mips_gui src/gui/main_window.cpp)
  target_link_libraries(mips_gui PRIVATE Qt6::Widgets mips_core)
  target_compile_features(mips_gui PRIVATE cxx_std_20)
endif()

# ===== 代碼格式化支持 =====
# 尋找 clang-format
find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format clang-format-14 clang-format-13 clang-format-12
    DOC "Path to clang-format executable"
)

if(CLANG_FORMAT_EXECUTABLE)
    message(STATUS "🎨 找到 clang-format: ${CLANG_FORMAT_EXECUTABLE}")
    
    # 收集所有 C++ 源文件
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/*.hpp
    )
    
    # 排除第三方代碼和生成的檔案
    list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*build.*")
    list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*_deps.*")
    list(FILTER ALL_CXX_SOURCE_FILES EXCLUDE REGEX ".*CMakeFiles.*")
    
    # 添加格式化目標
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i -style=file ${ALL_CXX_SOURCE_FILES}
        COMMENT "🎨 正在格式化代碼..."
        VERBATIM
    )
    
    # 添加格式檢查目標
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror -style=file ${ALL_CXX_SOURCE_FILES}
        COMMENT "🔍 檢查代碼格式..."
        VERBATIM
    )
    
    message(STATUS "💡 使用方式:")
    message(STATUS "   cmake --build . --target format      # 格式化所有代碼")
    message(STATUS "   cmake --build . --target format-check # 檢查代碼格式")
    
else()
    message(STATUS "⚠️  未找到 clang-format，代碼格式化功能不可用")
    message(STATUS "   安裝方式: choco install llvm (Windows)")
    message(STATUS "           sudo apt install clang-format (Linux)")
    message(STATUS "           brew install clang-format (macOS)")
endif()
