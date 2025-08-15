/**
 * @file step_definitions_logical_minimal.cpp
 * @brief BDD Step Definitions for Minimal AND Instruction Implementation
 * 
 * 按照BDD最小增量原則：
 * Phase A: 撰寫步驟 - 先只宣告接口，所有函式回傳/行為留空
 */

#include <cucumber-cpp/autodetect.hpp>
#include <gtest/gtest.h>
#include "../src/Cpu.h"
#include "../src/Assembler.h"
#include "../src/Instruction.h"
#include <memory>
#include <map>
#include <string>
#include <chrono>

using cucumber::ScenarioScope;

namespace {
    // Register name to number mapping
    std::map<std::string, int> registerMap = {
        {"$zero", 0}, {"$at", 1},
        {"$v0", 2}, {"$v1", 3},
        {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
        {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11},
        {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
        {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19},
        {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
        {"$t8", 24}, {"$t9", 25},
        {"$k0", 26}, {"$k1", 27},
        {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31}
    };

    uint32_t parseHexValue(const std::string& hexStr) {
        return static_cast<uint32_t>(std::stoul(hexStr, nullptr, 16));
    }

    int getRegisterNumber(const std::string& regName) {
        auto it = registerMap.find(regName);
        return (it != registerMap.end()) ? it->second : -1;
    }
}

/**
 * @brief AND指令最小測試的上下文結構
 */
struct AndInstructionMinimalContext {
    std::unique_ptr<mips::Cpu> cpu;
    std::unique_ptr<mips::Assembler> assembler;
    std::map<std::string, uint32_t> initialRegisterValues;
    std::string lastExecutedInstruction;
    bool cpuInitialized = false;
    int instructionCycles = 0;
    
    // 用於驗證暫存器是否保持不變
    void storeInitialValue(const std::string& regName, uint32_t value) {
        initialRegisterValues[regName] = value;
    }
    
    uint32_t getInitialValue(const std::string& regName) const {
        auto it = initialRegisterValues.find(regName);
        return (it != initialRegisterValues.end()) ? it->second : 0;
    }
};

// ============================================================================
// Phase A: 撰寫步驟 - 先只宣告接口，所有函式回傳/行為留空
// ============================================================================

/**
 * @brief Step 1: 初始化MIPS CPU模擬器
 * Phase A: 先留空接口 - 這裡應該導致測試失敗
 */
GIVEN("^I have a MIPS CPU simulator$") {
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 先留空接口，讓測試失敗
    // TODO: 在Phase B中實作CPU初始化
    
    // 暫時註解掉實作，確保測試失敗
    // context->cpu = std::make_unique<mips::Cpu>();
    // context->assembler = std::make_unique<mips::Assembler>();
    // context->cpuInitialized = true;
}

/**
 * @brief Step 2: 將CPU設為乾淨狀態
 * Phase A: 先留空接口
 */
GIVEN("^the CPU is in a clean state with all registers set to zero$") {
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作暫存器重置
    
    // 暫時註解掉實作
    // if (context->cpu) {
    //     for (int i = 0; i < 32; ++i) {
    //         context->cpu->getRegisterFile().write(i, 0);
    //     }
    // }
}

/**
 * @brief Step 3: 設定程式計數器
 * Phase A: 先留空接口
 */
GIVEN("^the program counter is at address 0$") {
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作PC設定
    
    // 暫時註解掉實作
    // if (context->cpu) {
    //     context->cpu->setProgramCounter(0);
    // }
}

/**
 * @brief Step 4: 設定暫存器值
 * Phase A: 先留空接口 - 這是關鍵的測試點
 */
GIVEN("^register (.+) contains (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, hexValue);
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作暫存器設定
    
    // 暫時註解掉實作
    // int regNum = getRegisterNumber(regName);
    // if (regNum != -1 && context->cpu) {
    //     uint32_t value = parseHexValue(hexValue);
    //     context->storeInitialValue(regName, value);
    //     context->cpu->getRegisterFile().write(regNum, value);
    // }
}

/**
 * @brief Step 5: 執行指令 - 這是核心測試點
 * Phase A: 這裡故意留空，讓測試失敗
 */
WHEN("^I execute the instruction \"(.+)\"$") {
    REGEX_PARAM(std::string, instruction);
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 這裡故意留空，讓測試失敗
    // 這是我們期望的紅燈狀態
    context->lastExecutedInstruction = instruction;
    
    // Phase A: 拋出"尚未實作"的異常，確保測試失敗
    FAIL() << "AND instruction execution not yet implemented - this is expected in Phase A";
}

/**
 * @brief Step 6: 驗證暫存器結果
 * Phase A: 先留空接口
 */
THEN("^register (.+) should contain (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, expectedHex);
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作結果驗證
    
    // 暫時註解掉實作
    // int regNum = getRegisterNumber(regName);
    // if (regNum != -1 && context->cpu) {
    //     uint32_t expected = parseHexValue(expectedHex);
    //     uint32_t actual = context->cpu->getRegisterFile().read(regNum);
    //     
    //     EXPECT_EQ(actual, expected) 
    //         << "Register " << regName << " should contain " << expectedHex 
    //         << " but contains 0x" << std::hex << actual;
    // }
}

/**
 * @brief Step 7: 驗證暫存器保持不變
 * Phase A: 先留空接口
 */
THEN("^register (.+) should still contain (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, expectedHex);
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作不變性驗證
}

/**
 * @brief Step 8: 驗證CPU週期計數
 * Phase A: 先留空接口
 */
THEN("^the instruction should complete in exactly 1 CPU cycle$") {
    ScenarioScope<AndInstructionMinimalContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作週期計數驗證
}
