# 🚀 XOR/NOR 指令實作快速入門指南

## 📋 現況概述

你現在接手的是一個已經建立完整 BDD/TDD 框架的專案：
- ✅ AND 指令：完整實作，所有測試通過
- ✅ OR 指令：基本實作，測試通過  
- ⏳ XOR 指令：測試框架就緒，等待實作
- ⏳ NOR 指令：測試框架就緒，等待實作

## 🎯 你的任務

**目標**: 完成 XOR 和 NOR 指令的實作，遵循相同的原子化開發流程

## ⚡ 5分鐘快速開始

### Step 1: 驗證環境
```bash
cd c:\Users\aloha\Documents\GitHub\MIPS-Assembly-Simulator\build
ninja unit_tests
.\tests\unit_tests.exe --gtest_filter="LogicalInstructionWalkingSkeleton.*"
```
**期望結果**: 6個測試通過，2個被禁用

### Step 2: 啟用 XOR 測試 (創造紅燈)
編輯 `tests/test_logical_walking_skeleton.cpp`：

```cpp
// 找到這行並修改：
// TEST_F(LogicalInstructionWalkingSkeleton, DISABLED_XOR_BasicFunctionality) {
// 改為：
TEST_F(LogicalInstructionWalkingSkeleton, XOR_BasicFunctionality_ShouldFail) {
    // Arrange: 設定初始狀態
    setRegister("$t0", 0xFF00FF00);  // 來源暫存器1
    setRegister("$t1", 0x00FF00FF);  // 來源暫存器2
    
    // Act: 執行XOR指令 - 期望失敗（紅燈）
    std::string instruction = "xor $t2, $t0, $t1";
    executeInstruction(instruction);
    
    // Assert: 檢查預期結果
    uint32_t expectedResult = 0xFFFF0000;  // 0xFF00FF00 ^ 0x00FF00FF
    expectRegister("$t2", expectedResult);
}
```

### Step 3: 確認紅燈
```bash
ninja unit_tests
.\tests\unit_tests.exe --gtest_filter="*XOR*"
```
**期望結果**: 測試失敗（紅燈） - 這是正確的！

### Step 4: 實作 XOR 指令 (變成綠燈)

#### 4.1 在 `src/Instruction.h` 添加類別宣告：
```cpp
/**
 * @brief R-type XOR instruction (logical XOR)
 */
class XorInstruction : public RTypeInstruction {
public:
    XorInstruction(int rd, int rs, int rt);
    
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};
```

#### 4.2 在 `src/Instruction.cpp` 添加實作：
```cpp
XorInstruction::XorInstruction(int rd, int rs, int rt) 
    : RTypeInstruction(rd, rs, rt) {
}

void XorInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);
    uint32_t result = rsValue ^ rtValue;  // Bitwise XOR operation
    
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}

std::string XorInstruction::getName() const {
    return "xor";
}
```

#### 4.3 在 `src/InstructionDecoder.cpp` 添加解碼支援：
找到 `decodeRType()` 函式，在 switch 中添加：
```cpp
case 0x26:  // XOR instruction
    return std::make_unique<XorInstruction>(rd, rs, rt);
```

#### 4.4 在測試中添加 XOR 支援：
在 `tests/test_logical_walking_skeleton.cpp` 的 `executeInstruction()` 函式中添加：
```cpp
// Phase E: XOR指令支援
else if (instruction == "xor $t2, $t0, $t1") {
    auto xorInstr = std::make_unique<mips::XorInstruction>(10, 8, 9);
    xorInstr->execute(*cpu);
}
```

### Step 5: 驗證綠燈
```bash
ninja unit_tests
.\tests\unit_tests.exe --gtest_filter="*XOR*"
```
**期望結果**: 測試通過（綠燈）！

### Step 6: 重複 NOR 指令
使用相同的流程實作 NOR 指令：
- 功能碼：0x27
- 操作：`result = ~(rsValue | rtValue)`
- 測試數據：0xFF00FF00 NOR 0x00FF00FF = 0x0000FF00

## 🔍 除錯指南

### 常見問題

**Q: 編譯錯誤 "XorInstruction not declared"**
A: 確認 `#include "../src/Instruction.h"` 在測試檔案中，且 XorInstruction 已在標頭檔中宣告

**Q: 測試通過但結果不對**
A: 檢查 XOR 運算邏輯，確認使用 `^` 運算子而非 `&` 或 `|`

**Q: 解碼器不認識指令**
A: 確認在 `InstructionDecoder.cpp` 中添加了正確的 case 0x26

### 測試驗證計算

**XOR 測試**:
```
0xFF00FF00 (4278255360)
0x00FF00FF (16711935)
XOR 結果 = 0xFFFF0000 (4294901760)
```

**NOR 測試**:
```
0xFF00FF00 | 0x00FF00FF = 0xFFFFFFFF
~0xFFFFFFFF = 0x00000000
```

## 📚 參考資料

### 相關檔案
- **實作參考**: `src/Instruction.cpp` (查看 AndInstruction 和 OrInstruction)
- **測試參考**: `tests/test_logical_walking_skeleton.cpp` 
- **解碼參考**: `src/InstructionDecoder.cpp`

### MIPS 指令格式
```
R-type: | op(6) | rs(5) | rt(5) | rd(5) | shamt(5) | funct(6) |
XOR:    |   0   |  rs   |  rt   |  rd   |    0     |  0x26   |
NOR:    |   0   |  rs   |  rt   |  rd   |    0     |  0x27   |
```

### BDD 循環提醒
1. 🔴 **紅燈**: 先寫測試，確認失敗
2. 🟢 **綠燈**: 寫最小代碼讓測試通過  
3. ♻️ **重構**: 改善代碼品質
4. 🔄 **重複**: 繼續下一個指令

## 🎯 完成檢查清單

### XOR 指令完成檢查
- [ ] 測試從紅燈變綠燈
- [ ] 所有 XOR 測試通過
- [ ] 解碼器支援 XOR
- [ ] 指令執行邏輯正確

### NOR 指令完成檢查  
- [ ] 測試從紅燈變綠燈
- [ ] 所有 NOR 測試通過
- [ ] 解碼器支援 NOR
- [ ] 指令執行邏輯正確

### 整合測試
- [ ] 所有邏輯指令測試通過
- [ ] 沒有迴歸問題
- [ ] 代碼品質良好

## 🚀 完成後的下一步

當你完成 XOR 和 NOR 指令後：

1. **更新開發報告** - 在 `docs/LOGICAL_INSTRUCTIONS_DEVELOPMENT_REPORT.md` 中標記完成狀態
2. **添加綜合測試** - 為 XOR/NOR 添加類似 AND 的詳細測試
3. **性能測試** - 確認新指令的執行效率
4. **整合測試** - 測試多個邏輯指令的組合使用

## 💡 額外挑戰（可選）

如果你完成得很快，可以考慮：

1. **改善解析器** - 實作更靈活的指令字串解析
2. **錯誤處理** - 添加無效指令的錯誤檢測  
3. **GUI 整合** - 將邏輯指令加入到圖形界面
4. **文檔完善** - 更新用戶手冊和API文檔

---

**好運！記住：小步前進，測試驅動，保持代碼簡潔！** 🎯
