# CLI module CMakeLists.txt

# Create CLI library
add_library(mips_cli_lib 
    cli.cpp
    cli.hpp
    run_executor.cpp
    run_executor.hpp
    assemble_executor.cpp
    assemble_executor.hpp
)

target_include_directories(mips_cli_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(mips_cli_lib 
    PUBLIC 
        mips_core
)

target_compile_features(mips_cli_lib PUBLIC cxx_std_20)

# Create CLI executable
add_executable(mips_cli_exe ../cli.cpp)
target_link_libraries(mips_cli_exe PRIVATE mips_cli_lib)
target_compile_features(mips_cli_exe PRIVATE cxx_std_20)

# Set output name
set_target_properties(mips_cli_exe PROPERTIES OUTPUT_NAME "mipsim")
