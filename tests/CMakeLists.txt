# Google Test executable
add_executable(unit_tests
    test_cpu.cpp
    test_bdd_core_instructions.cpp
)

target_link_libraries(unit_tests
    PRIVATE
    mips-simulator
    gtest_main
    gtest
)

# Register test with CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)
