# Google Test executable
add_executable(unit_tests
    test_cpu.cpp
    test_bdd_core_instructions.cpp
    test_pipeline.cpp
    test_pipeline_integration.cpp
    test_pipeline_execution.cpp
    test_syscalls.cpp
    test_instruction_decoder.cpp
    test_gui_console_output.cpp
    test_enhanced_gui_console.cpp  # Enhanced console testing
    test_mips_core_console.cpp     # MIPS core requirements console testing
    test_syscall_debug.cpp         # Debug syscall issues
    test_assembler_debug.cpp       # Debug assembler issues
    test_hello_mips.cpp            # Hello MIPS test files
    test_logical_walking_skeleton.cpp  # Walking Skeleton for logical instructions
    test_extended_instructions_atomic.cpp  # Extended instructions (SLT, SLTI, etc.)
    test_slt_instruction.cpp               # SLT instruction TDD tests (Phase A: Red Light)
    test_sltiu_instruction.cpp             # SLTIU instruction TDD tests (Phase A: Red Light)
    test_bne_instruction.cpp               # BNE instruction TDD tests (Phase A: Red Light)
    # test_gui_interface.cpp  # Temporarily commented out due to encoding issues
)

target_link_libraries(unit_tests
    PRIVATE
    mips-simulator
    gtest_main
    gtest
)

# Note: GUI console test uses base GUI class, not ImGui-specific class
# This avoids SDL2 linking issues in unit tests

# Register test with CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)
