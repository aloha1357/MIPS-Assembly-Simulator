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
    test_ori_instruction.cpp               # ORI instruction TDD tests (Phase A: Red Light)
    test_andi_instruction.cpp              # ANDI instruction TDD tests (Phase A: Red Light)
    test_xori_instruction.cpp              # XORI instruction TDD tests (Phase A: Red Light)
    test_logical_and_bdd_minimal.cpp       # AND instruction BDD tests (Phase A: Red Light)
    test_logical_or_bdd_minimal.cpp        # OR instruction BDD tests (Phase A: Red Light)
    test_logical_xor_bdd_minimal.cpp       # XOR instruction BDD tests (Phase A: Red Light)
    test_logical_nor_bdd_minimal.cpp       # NOR instruction BDD tests (Phase A: Red Light)
    test_logical_sll_bdd_minimal.cpp       # SLL instruction BDD tests (Phase A: Red Light)
    test_and_instruction.cpp               # AND instruction Integration tests
    test_or_instruction.cpp                # OR instruction Integration tests
    test_xor_instruction.cpp               # XOR instruction Integration tests
    test_nor_instruction.cpp               # NOR instruction Integration tests
    test_sll_instruction.cpp               # SLL instruction Integration tests
    test_logical_srl_bdd_minimal.cpp       # SRL instruction BDD tests
    test_srl_instruction.cpp               # SRL instruction Integration tests
    test_logical_sra_bdd_minimal.cpp       # SRA instruction BDD tests
    test_sra_instruction.cpp               # SRA instruction Integration tests
    test_logical_ori_bdd_minimal.cpp       # ORI instruction BDD tests (Phase A: Red Light)
    test_ori_instruction_integration.cpp   # ORI instruction Integration tests
    test_logical_andi_bdd_minimal.cpp      # ANDI instruction BDD tests (Phase A: Red Light)
    test_andi_instruction_integration.cpp  # ANDI instruction Integration tests
    test_logical_xori_bdd_minimal.cpp      # XORI instruction BDD tests (Phase A: Red Light)
    test_xori_instruction_integration.cpp  # XORI instruction Integration tests
    test_blez_instruction_bdd_minimal.cpp  # BLEZ instruction BDD tests (Phase 3.1: Red Light)
    test_bgtz_instruction_bdd_minimal.cpp  # BGTZ instruction BDD tests (Phase 3.4: Red Light)
    test_bgtz_instruction_integration.cpp  # BGTZ instruction Integration tests (Phase 3.6)
    test_addiu_instruction_bdd_minimal.cpp # ADDIU instruction BDD tests (Phase 4.1: Red Light)
    test_subu_instruction_bdd_minimal.cpp  # SUBU instruction BDD tests (Phase 4.2: Red Light)
    test_addu_instruction_bdd_minimal.cpp  # ADDU instruction BDD tests (Phase 4.3: Red Light)
    test_sllv_instruction_bdd_minimal.cpp  # SLLV instruction BDD tests (Phase 5.1: Red Light)
    test_srlv_instruction_bdd_minimal.cpp  # SRLV instruction BDD tests (Phase 5.2: Red Light)
    test_srav_instruction_bdd_minimal.cpp  # SRAV instruction BDD tests (Phase 5.3: Red Light)
    test_jr_instruction_bdd_minimal.cpp    # JR instruction BDD tests (Phase 6.1: Red Light)
    test_jr_instruction_integration.cpp    # JR instruction Integration tests (Phase 6.1: Integration)
    test_jal_instruction_bdd_minimal.cpp   # JAL instruction BDD tests (Phase 6.2: Red Light)
    test_jalr_instruction_bdd_minimal.cpp  # JALR instruction BDD tests (Phase 6.3: Red Light)
    test_jalr_instruction_integration.cpp  # JALR instruction Integration tests (Phase 6.3: Integration)
    test_lb_instruction_bdd_minimal.cpp    # LB instruction BDD tests (Phase 7.1: Red Light)
    test_sb_instruction_bdd_minimal.cpp    # SB instruction BDD tests (Phase 7.2: Red Light)
    test_lbu_instruction_bdd_minimal.cpp   # LBU instruction BDD tests (Phase 7.3: Red Light)
    test_lh_instruction_bdd_minimal.cpp    # LH instruction BDD tests (Phase 7.4: Red Light)
    test_sh_instruction_bdd_minimal.cpp    # SH instruction BDD tests (Phase 7.5: Red Light)
    test_lhu_instruction_bdd_minimal.cpp   # LHU instruction BDD tests (Phase 7.6: Red Light)
    test_register_file_hilo_bdd.cpp     # RegisterFile HI/LO BDD tests (Phase 8.1: Green Light)
    test_mult_instruction_bdd_minimal.cpp   # MULT instruction BDD tests (Phase 8.2: Green Light)
    test_multu_instruction_bdd_minimal.cpp  # MULTU instruction BDD tests (Phase 8.4: Green Light)
    test_div_instruction_bdd_minimal.cpp    # DIV instruction BDD tests (Phase 8.5: Green Light)
    test_divu_instruction_bdd_minimal.cpp   # DIVU instruction BDD tests (Phase 8.6: Red Light)
    test_mfhi_instruction_bdd_minimal.cpp   # MFHI instruction BDD tests (Phase 9.1: Green Light)
    test_mthi_instruction_bdd_minimal.cpp   # MTHI instruction BDD tests (Phase 9.2: Green Light)
    test_mflo_instruction_bdd_minimal.cpp   # MFLO instruction BDD tests (Phase 9.3: Green Light)
    test_mtlo_instruction_bdd_minimal.cpp   # MTLO instruction BDD tests (Phase 9.4: Red Light)
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
