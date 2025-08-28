enable_testing()

# Use FetchContent to automatically download GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test mode selection - can be enabled with cmake -DFULL_TESTS=ON for full test suite
option(FULL_TESTS "Enable full test suite (all test files)" OFF)

if(FULL_TESTS)
    message(STATUS "Full test mode enabled - compiling available test files")
    
    # Full test: include all test files but exclude problematic ones
    file(GLOB ALL_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp")
    
    # Exclude test files with compilation issues
    set(PROBLEMATIC_FILES
        # GUI related files (missing dependencies)
        "test_gui_interface.cpp"
        "test_gui_console_output.cpp" 
        "test_enhanced_gui_console.cpp"
        "test_mips_core_console.cpp"  # Depends on MipsSimulatorGUI
        # Files missing header files
        "test_jalr_instruction_bdd_minimal_fixed.cpp"  # Missing MemorySubsystem.h
        # Files with syntax errors
        "test_blez_instruction_bdd_minimal_clean.cpp"  # DISABLED_TEST_F syntax error
        "test_jalr_instruction_bdd_minimal_v2.cpp"     # DISABLED_TEST_F syntax error
        "test_jalr_instruction_bdd_minimal.cpp"        # May have similar issues
    )
    
    set(EXISTING_TEST_SOURCES)
    foreach(test_file ${ALL_TEST_FILES})
        get_filename_component(test_filename ${test_file} NAME)
        if(NOT ${test_filename} IN_LIST PROBLEMATIC_FILES)
            list(APPEND EXISTING_TEST_SOURCES ${test_file})
        endif()
    endforeach()
    
else()
    message(STATUS "Minimal test mode - compiling only core test files")
    
    # Minimal test: select only core test files to avoid compilation timeout
    set(CORE_TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/test_cpu.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_instruction_decoder.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_pipeline.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_syscalls.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_bdd_core_instructions.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_llo_instruction.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_lhi_instruction.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_trap_instruction.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_character_syscalls.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_missing_instructions_integration.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_cli_argument_parsing.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_cli_run_command.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_cli_assemble_command.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_jal_jr_integration.cpp"
    )

    # Add small targeted tests useful during iterative debugging
    list(APPEND CORE_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test_slt_and_lbu.cpp")
    list(APPEND CORE_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test_mismatch_cases.cpp")
    list(APPEND CORE_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test_failing_segments.cpp")

    # Check if files exist and filter
    set(EXISTING_TEST_SOURCES)
    foreach(test_file ${CORE_TEST_SOURCES})
        if(EXISTS ${test_file})
            list(APPEND EXISTING_TEST_SOURCES ${test_file})
        endif()
    endforeach()

    # If no core test files exist, select first 10 test files
    if(NOT EXISTING_TEST_SOURCES)
        file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp")
        list(LENGTH TEST_FILES TOTAL_COUNT)
        if(TOTAL_COUNT GREATER 10)
            list(SUBLIST TEST_FILES 0 10 EXISTING_TEST_SOURCES)
        else()
            set(EXISTING_TEST_SOURCES ${TEST_FILES})
        endif()
    endif()
endif()

list(LENGTH EXISTING_TEST_SOURCES TEST_COUNT)
message(STATUS "Building with ${TEST_COUNT} test files to avoid timeout")

add_executable(mips_tests ${EXISTING_TEST_SOURCES})
target_link_libraries(mips_tests PRIVATE gtest_main gtest mips_core mips_cli_lib)
target_compile_features(mips_tests PRIVATE cxx_std_20)

# Simplified test configuration
add_test(NAME all_tests COMMAND mips_tests)

# Register test with CTest
include(GoogleTest)
gtest_discover_tests(mips_tests)
