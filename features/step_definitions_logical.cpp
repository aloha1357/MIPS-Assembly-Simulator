/**
 * @file step_definitions_logical.cpp
 * @brief BDD Step Definitions for Logical Instructions Walking Skeleton
 * 
 * 這個文件專門用於邏輯指令的BDD步驟定義
 * 按照BDD最小增量原則，先定義接口，實作留空
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
 * @brief 邏輯指令測試的上下文結構
 */
struct LogicalInstructionContext {
    std::unique_ptr<mips::Cpu> cpu;
    std::map<std::string, uint32_t> initialRegisterValues;
    std::string lastExecutedInstruction;
    bool cpuInitialized = false;
    
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
 * @brief Walking Skeleton Step 1: 初始化MIPS CPU模擬器
 */
GIVEN("^I have a MIPS CPU simulator$") {
    ScenarioScope<LogicalInstructionContext> context;
    
    // Phase A: 先留空接口 - 這裡應該導致測試失敗
    // TODO: 在Phase B中實作CPU初始化
    
    // 臨時實作讓Walking Skeleton能運行
    context->cpu = std::make_unique<mips::Cpu>();
    context->cpuInitialized = true;
}

/**
 * @brief Walking Skeleton Step 2: 將CPU設為乾淨狀態
 */
GIVEN("^the CPU is in a clean state with all registers set to zero$") {
    ScenarioScope<LogicalInstructionContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作暫存器重置
    
    // 臨時實作
    if (context->cpu) {
        for (int i = 0; i < 32; ++i) {
            context->cpu->getRegisterFile().write(i, 0);
        }
    }
}

/**
 * @brief Walking Skeleton Step 3: 設定程式計數器
 */
GIVEN("^the program counter is at address 0$") {
    ScenarioScope<LogicalInstructionContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作PC設定
    
    // 臨時實作
    if (context->cpu) {
        context->cpu->setProgramCounter(0);
    }
}

/**
 * @brief Walking Skeleton Step 4: 設定暫存器值
 */
GIVEN("^register (.+) contains (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, hexValue);
    ScenarioScope<LogicalInstructionContext> context;
    
    // Phase A: 先留空接口 - 這是關鍵的Walking Skeleton測試點
    // TODO: 在Phase B中實作暫存器設定
    
    // 臨時實作讓Walking Skeleton能運行到指令執行
    int regNum = getRegisterNumber(regName);
    if (regNum != -1 && context->cpu) {
        uint32_t value = parseHexValue(hexValue);
        context->storeInitialValue(regName, value);
        context->cpu->getRegisterFile().write(regNum, value);
    }
}

/**
 * @brief Walking Skeleton Step 5: 執行指令 - 這是核心測試點
 */
WHEN("^I execute the instruction \"(.+)\"$") {
    REGEX_PARAM(std::string, instruction);
    ScenarioScope<LogicalInstructionContext> context;
    
    // Phase A: 這裡故意留空，讓Walking Skeleton失敗
    // 這是我們期望的紅燈狀態
    context->lastExecutedInstruction = instruction;
    
    // Phase B: 在這裡實作指令解析和執行
    // TODO: 實作AND指令解析和執行
    
    // 檢查是否為AND指令
    if (instruction.find("and ") == 0) {
        // Phase A: 暫時留空，讓測試失敗
        // 這裡應該拋出"尚未實作"的異常或讓斷言失敗
        FAIL() << "AND instruction not yet implemented - this is expected in Phase A";
    }
}

/**
 * @brief Walking Skeleton Step 6: 驗證暫存器結果
 */
THEN("^register (.+) should contain (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, expectedHex);
    ScenarioScope<LogicalInstructionContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作結果驗證
    
    // 臨時實作
    int regNum = getRegisterNumber(regName);
    if (regNum != -1 && context->cpu) {
        uint32_t expected = parseHexValue(expectedHex);
        uint32_t actual = context->cpu->getRegisterFile().read(regNum);
        
        EXPECT_EQ(actual, expected) 
            << "Register " << regName << " should contain " << expectedHex 
            << " but contains 0x" << std::hex << actual;
    }
}

/**
 * @brief Walking Skeleton Step 7: 驗證暫存器保持不變
 */
THEN("^register (.+) should remain unchanged at (0x[0-9A-Fa-f]+)$") {
    REGEX_PARAM(std::string, regName);
    REGEX_PARAM(std::string, expectedHex);
    ScenarioScope<LogicalInstructionContext> context;
    
    // Phase A: 先留空接口
    // TODO: 在Phase B中實作不變性驗證
    
    // 臨時實作
    int regNum = getRegisterNumber(regName);
    if (regNum != -1 && context->cpu) {
        uint32_t expected = parseHexValue(expectedHex);
        uint32_t actual = context->cpu->getRegisterFile().read(regNum);
        uint32_t initial = context->getInitialValue(regName);
        
        EXPECT_EQ(actual, expected) 
            << "Register " << regName << " should remain at " << expectedHex 
            << " but is now 0x" << std::hex << actual;
        EXPECT_EQ(actual, initial)
            << "Register " << regName << " was modified during instruction execution";
    }
}

// ============================================================================
// 其他邏輯指令的步驟（目前被@ignore標記）
// ============================================================================

// 這些步驟將在後續的BDD循環中逐一啟用和實作

/**
 * @brief OR指令的步驟定義（目前未實作）
 */
// WHEN OR instruction steps (currently @ignore tagged)

/**
 * @brief XOR指令的步驟定義（目前未實作）
 */
// WHEN XOR instruction steps (currently @ignore tagged)

/**
 * @brief NOR指令的步驟定義（目前未實作）
 */
// WHEN NOR instruction steps (currently @ignore tagged)
