# MIPS Assembly Simulator - 開發狀況報告

**報告日期**: 2025年8月15日  
**專案狀態**: ✅ 核心系統穩定 + 🔄 指令集持續擴展中  
**測試結果**: 111/111 通過 (100%)，執行時間 36ms

## 📊 目前進度總覽

### ✅ 已完成指令 (16/47 = 34%)

| 指令 | 類型 | 功能 | 狀態 | 備註 |
|------|------|------|------|------|
| `add` | R-type | 加法運算 | ✅ 完成 | 基礎算術 |
| `sub` | R-type | 減法運算 | ✅ 完成 | 基礎算術 |
| `and` | R-type | 位元AND | ✅ 完成 | 邏輯運算 |
| `or` | R-type | 位元OR | ✅ 完成 | 邏輯運算 |
| `xor` | R-type | 位元XOR | ✅ 完成 | 邏輯運算 |
| `nor` | R-type | 位元NOR | ✅ 完成 | 邏輯運算 |
| `slt` | R-type | 有符號比較 | ✅ 完成 | 比較指令 |
| `sltu` | R-type | 無符號比較 | ✅ 完成 | **🆕 剛完成** |
| `sll` | R-type | 左位移 | ✅ 完成 | 位移運算 |
| `syscall` | R-type | 系統呼叫 | ✅ 完成 | 系統功能 |
| `addi` | I-type | 立即值加法 | ✅ 完成 | 立即值運算 |
| `slti` | I-type | 立即值有符號比較 | ✅ 完成 | 比較指令 |
| `lw` | I-type | 載入字組 | ✅ 完成 | 記憶體存取 |
| `sw` | I-type | 儲存字組 | ✅ 完成 | 記憶體存取 |
| `beq` | I-type | 相等分支 | ✅ 完成 | 條件分支 |
| `j` | J-type | 無條件跳躍 | ✅ 完成 | 程式控制 |

### 🎯 最新完成: SLTU 指令 (2025-08-15)

**實作細節**:
- **功能**: `sltu $rd, $rs, $rt` - 設定 rd = (rs < rt) ? 1 : 0 (無符號比較)
- **關鍵差異**: 與 SLT 不同，SLTU 使用無符號比較
- **測試案例**: 6 個新測試，包含關鍵邊界測試 (0xFFFFFFFF < 5 = false)
- **開發方式**: 嚴格遵循 BDD 紅燈→綠燈→重構循環

```cpp
void SltuInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);  // 無符號
    uint32_t rtValue = cpu.getRegisterFile().read(m_rt);  // 無符號
    uint32_t result = (rsValue < rtValue) ? 1 : 0;        // 無符號比較
    cpu.getRegisterFile().write(m_rd, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

## 🚀 下一個開發目標: SLTIU 指令

### 目標規格
- **指令**: `sltiu $rt, $rs, immediate`
- **功能**: 設定 rt = (rs < immediate) ? 1 : 0 (無符號比較立即值)
- **類型**: I-type 指令
- **功能碼**: 0x0B
- **優先級**: 🔥 **最高** (完成比較指令家族)
- **預估時間**: 1.5 小時

### 為什麼選擇 SLTIU？
1. **自然延續**: 基於剛完成的 SLTU + 現有的 SLTI 經驗
2. **低風險**: 結合兩個已知模式，技術難度低
3. **高價值**: 完成比較指令家族 (SLT → SLTU → SLTI → SLTIU)
4. **架構一致**: 遵循現有 I-type 指令模式

## 📋 開發實作指南

### 第一步: 紅燈階段 (30分鐘)
1. **建立測試檔案**: `tests/test_sltiu_instruction.cpp`
```cpp
TEST_F(SltiuInstructionTest, SltiuInstruction_BasicOperation) {
    cpu->getRegisterFile().write(8, 5);        // $t0 = 5
    SltiuInstruction sltiu(9, 8, 10);          // sltiu $t1, $t0, 10
    sltiu.execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(9), 1);  // 5 < 10 = true
}
```

2. **確認編譯失敗**: SltiuInstruction 類別未定義 ✅

### 第二步: 綠燈階段 (60分鐘)
1. **宣告類別** (`src/Instruction.h`):
```cpp
class SltiuInstruction : public ITypeInstruction {
public:
    SltiuInstruction(uint8_t rt, uint8_t rs, int16_t immediate);
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};
```

2. **實作執行邏輯** (`src/Instruction.cpp`):
```cpp
void SltiuInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t immediateValue = static_cast<uint32_t>(static_cast<uint16_t>(m_immediate));
    uint32_t result = (rsValue < immediateValue) ? 1 : 0;
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

### 第三步: 重構階段 (30分鐘)
1. **代碼清理**: 確保命名和風格一致
2. **測試驗證**: 目標達到 117 個測試 (111 + 6)
3. **回歸測試**: 確認無破壞現有功能

## 🧪 建議測試案例

```cpp
// 1. 基本操作: 5 < 10 → 1
// 2. 邊界條件: 0 < 1 → 1
// 3. 相等測試: 42 < 42 → 0  
// 4. 大於測試: 100 < 50 → 0
// 5. 立即值邊界: 測試 0xFFFF
// 6. PC 更新驗證
```

## 📁 關鍵檔案位置

```
MIPS-Assembly-Simulator/
├── src/
│   ├── Instruction.h          # 🔥 添加 SltiuInstruction 類別宣告
│   ├── Instruction.cpp        # 🔥 實作 SltiuInstruction::execute()
│   └── InstructionDecoder.cpp # 🚧 後續整合解碼邏輯
├── tests/
│   ├── test_slt_instruction.cpp    # 📖 參考 SLTU 實作範例
│   └── test_sltiu_instruction.cpp  # 🆕 新建測試檔案
└── build/
    └── tests/unit_tests.exe   # 🧪 測試執行檔
```

## ⚡ 快速驗證命令

```powershell
# 編譯專案
cmake --build build --config Debug

# 執行所有測試
.\build\tests\unit_tests.exe --gtest_brief=1
# 期望: [  PASSED  ] 117 tests (111 + 6 新測試)

# 只測試 SLTIU
.\build\tests\unit_tests.exe --gtest_filter="*Sltiu*"
```

## 🎯 後續發展路線圖

### 近期目標 (優先級排序)
1. **SLTIU** (P1) - 立即值無符號比較 (下一個目標)
2. **BNE** (P1) - Branch Not Equal 分支指令  
3. **ORI** (P2) - OR Immediate 立即值邏輯運算
4. **ANDI** (P2) - AND Immediate 立即值邏輯運算

### 中期目標
- 達成 20+ 指令實作
- 完整分支指令組 (BEQ, BNE, BLT, BGE 等)
- 完整立即值指令組

### 架構目標
- 指令解碼器整合
- 組合語言編譯器整合  
- 性能優化

## 🏆 專案品質指標

- **測試覆蓋率**: 100% (111/111 測試通過)
- **執行效能**: <40ms 測試時間
- **代碼品質**: 零編譯警告
- **架構一致性**: 統一的指令實作模式

## 📞 開發建議

1. **嚴格遵循 BDD**: 不要跳過紅燈階段
2. **小步前進**: 一次只實作一個指令  
3. **保持一致**: 參考現有指令實作模式
4. **即時驗證**: 每次變更後運行完整測試
5. **文檔更新**: 完成後更新此報告

---

**下一步**: 開始 SLTIU 指令實作，從建立 `tests/test_sltiu_instruction.cpp` 開始！ 🚀
