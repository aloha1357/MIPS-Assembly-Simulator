enable_testing()

# 使用 FetchContent 自動下載 GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 把 tests/ 底下全部 .cpp 視為測試檔（排除備份檔）
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# 排除有編譯問題的檔案
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*\\.backup$")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*test_gui_interface\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*test_enhanced_gui_console\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*test_gui_console_output\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*test_mips_core_console\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*test_jalr_instruction_bdd_minimal_fixed\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*test_jalr_instruction_bdd_minimal_v2\\.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*test_blez_instruction_bdd_minimal_clean\\.cpp$")

# 顯示找到的測試檔案數量（除錯用）
list(LENGTH TEST_SOURCES TEST_COUNT)
message(STATUS "Found ${TEST_COUNT} test files after filtering")

add_executable(mips_tests ${TEST_SOURCES})
target_link_libraries(mips_tests PRIVATE gtest_main gtest mips_core)
target_compile_features(mips_tests PRIVATE cxx_std_20)

# 測試分群配置

# 1) 一鍵全跑
add_test(NAME all_tests COMMAND mips_tests)

# 2) 核心功能測試（CPU、Memory、Pipeline 等基礎功能）
add_test(NAME core_tests COMMAND mips_tests --gtest_filter=CpuTest.*:MemoryTest.*:RegisterFileTest.*:PipelineTest.*:InstructionDecoderTest.*:InstructionTest.*)
set_tests_properties(core_tests PROPERTIES LABELS "core")

# 3) 邏輯指令測試群組
add_test(NAME logical_tests COMMAND mips_tests --gtest_filter=*Logical*:*logical*:And*:Or*:Xor*:Nor*:*Andi*:*Ori*:*Xori*)
set_tests_properties(logical_tests PROPERTIES LABELS "logical")

# 4) 位移指令測試群組
add_test(NAME shift_tests COMMAND mips_tests --gtest_filter=*Sll*:*Srl*:*Sra*:*SLL*:*SRL*:*SRA*)
set_tests_properties(shift_tests PROPERTIES LABELS "shift")

# 5) 分支/跳躍指令測試群組
add_test(NAME branch_tests COMMAND mips_tests --gtest_filter=*BEQ*:*BNE*:*BLEZ*:*BGTZ*:*JR*:*JAL*:*JALR*)
set_tests_properties(branch_tests PROPERTIES LABELS "branch")

# 6) 記憶體指令測試群組
add_test(NAME memory_tests COMMAND mips_tests --gtest_filter=*LW*:*SW*:*LB*:*SB*:*LH*:*SH*:*LBU*:*LHU*)
set_tests_properties(memory_tests PROPERTIES LABELS "memory")

# 7) 算術指令測試群組
add_test(NAME arithmetic_tests COMMAND mips_tests --gtest_filter=*ADD*:*SUB*:*MULT*:*DIV*:*SLT*:*Addi*:*Add*:*Sub*)
set_tests_properties(arithmetic_tests PROPERTIES LABELS "arithmetic")

# 8) BDD 測試群組（行為驅動開發測試）
add_test(NAME bdd_tests COMMAND mips_tests --gtest_filter=*BDD*:*Bdd*)
set_tests_properties(bdd_tests PROPERTIES LABELS "bdd")

# 9) 整合測試群組
add_test(NAME integration_tests COMMAND mips_tests --gtest_filter=*Integration*:*EndToEnd*:*AssemblerDebugTest*:*SyscallTest*)
set_tests_properties(integration_tests PROPERTIES LABELS "integration")

# 10) 快速基本功能驗證（只跑核心測試，適合快速檢查）
add_test(NAME smoke_tests COMMAND mips_tests --gtest_filter=CpuTest.InitialState:MemoryTest.WriteAndRead:RegisterFileTest.WriteAndRead:InstructionTest.AddInstructionBasic)
set_tests_properties(smoke_tests PROPERTIES LABELS "smoke")

# Register test with CTest
include(GoogleTest)
gtest_discover_tests(mips_tests)
