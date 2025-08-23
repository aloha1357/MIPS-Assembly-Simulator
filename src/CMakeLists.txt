# 把 src/ 下面所有 .cpp 都抓進來
file(GLOB_RECURSE SRC_ALL CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# 排除 main.cpp 和 GUI 相關檔案，只留核心檔變成 library
set(MAIN_CANDIDATE "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
list(FILTER SRC_ALL EXCLUDE REGEX ".*/main\\.cpp$")
list(FILTER SRC_ALL EXCLUDE REGEX ".*/imgui_main\\.cpp$")
list(FILTER SRC_ALL EXCLUDE REGEX ".*/gui_main\\.cpp$")
list(FILTER SRC_ALL EXCLUDE REGEX ".*/gui/.*\\.cpp$")

add_library(mips_core ${SRC_ALL})
# 如果你的 public header 在 src/include/，請改成該路徑
target_include_directories(mips_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(mips_core PUBLIC cxx_std_20)

# CLI：只編 main.cpp，連上 core
if (EXISTS "${MAIN_CANDIDATE}")
  add_executable(mips_cli "${MAIN_CANDIDATE}")
  target_link_libraries(mips_cli PRIVATE mips_core)
  target_compile_features(mips_cli PRIVATE cxx_std_20)
endif()
