# Cucumber-cpp test executable
add_executable(cucumber_tests
    step_definitions.cpp
    step_definitions_logical.cpp
)

target_link_libraries(cucumber_tests
    PRIVATE
    mips-simulator
    cucumber-cpp
    gtest
)

# Add custom target to run cucumber tests
add_custom_target(run_cucumber_tests
    COMMAND cucumber_tests
    DEPENDS cucumber_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
