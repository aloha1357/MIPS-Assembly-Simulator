# 🚀 MIPS Simulator - 下一位開發者快速上手指南

## 🎯 當前狀況 (2025-08-15)
- **已完成**: 16 個 MIPS 指令 | 111 個測試 (100% 通過)
- **最新**: SLTU 無符號比較指令剛完成
- **建議下一步**: SLTIU 指令 (預估 1.5 小時)

## ⚡ 5分鐘快速驗證

```powershell
# 1. 編譯專案
cmake --build build --config Debug

# 2. 運行全部測試
.\build\tests\unit_tests.exe --gtest_brief=1
# 期望看到: [  PASSED  ] 111 tests.

# 3. 驗證 SLTU 指令
.\build\tests\unit_tests.exe --gtest_filter="*Sltu*"
# 期望看到: [  PASSED  ] 6 tests from SltInstructionTest (關於 SltuInstruction)
```

## 🔄 BDD 開發循環 (已驗證有效)

### A. 紅燈階段 (30-45分鐘)
```cpp
// 1. 在 tests/test_sltiu_instruction.cpp 寫失敗測試
TEST_F(SltiuInstructionTest, SltiuInstruction_BasicOperation) {
    SltiuInstruction sltiu(10, 8, 100);  // sltiu $t2, $t0, 100
    sltiu.execute(*cpu);
    // 預期會編譯失敗：SltiuInstruction 類別未定義
}
```

### B. 綠燈階段 (60-90分鐘)
```cpp
// 2. 在 src/Instruction.h 添加類別宣告
class SltiuInstruction : public ITypeInstruction {
public:
    SltiuInstruction(uint8_t rt, uint8_t rs, int16_t immediate);
    void execute(Cpu& cpu) override;
    std::string getName() const override;
};

// 3. 在 src/Instruction.cpp 實作最小可行版本
void SltiuInstruction::execute(Cpu& cpu) {
    uint32_t rsValue = cpu.getRegisterFile().read(m_rs);
    uint32_t immediateValue = static_cast<uint32_t>(m_immediate);
    uint32_t result = (rsValue < immediateValue) ? 1 : 0;
    cpu.getRegisterFile().write(m_rt, result);
    cpu.setProgramCounter(cpu.getProgramCounter() + 1);
}
```

### C. 重構階段 (30分鐘)
- 檢查代碼風格一致性
- 確保 111+6 = 117 個測試全通過
- 更新開發報告

## 📁 關鍵檔案路徑

```
src/
├── Instruction.h          # 🔥 在這裡添加新指令類別
├── Instruction.cpp        # 🔥 在這裡實作執行邏輯
└── InstructionDecoder.cpp # 🚧 之後整合解碼邏輯

tests/
├── test_slt_instruction.cpp    # ✅ SLTU 測試範例參考
└── test_sltiu_instruction.cpp  # 🆕 新檔案 (建議名稱)

docs/
├── DEVELOPMENT_HANDOVER_REPORT.md  # 📊 完整開發報告
└── development-reports/
    └── FINAL_COMPLETION_REPORT.md  # 📈 專案完成報告
```

## 🎯 SLTIU 指令規格

- **指令格式**: `sltiu $rt, $rs, immediate`
- **功能**: 設定 `$rt = ($rs < immediate) ? 1 : 0` (無符號比較)
- **類型**: I-type 指令
- **功能碼**: 0x0B
- **關鍵特性**: 立即值無符號擴展 + 無符號比較

## 🧪 建議測試案例

```cpp
// 1. 基本操作: 5 < 100 → 1
// 2. 邊界條件: 0 < 1 → 1  
// 3. 相等測試: 42 < 42 → 0
// 4. 大值測試: 200 < 100 → 0
// 5. 立即值邊界: 測試 16-bit 立即值範圍
// 6. PC 更新: 確認程式計數器正確遞增
```

## ⚠️ 注意事項

1. **無符號比較**: 使用 `uint32_t` 而非 `int32_t`
2. **立即值處理**: I-type 使用 16-bit 立即值
3. **暫存器順序**: SLTIU 是 rt = rs < imm (與 R-type 不同)
4. **回歸測試**: 每次變更後確保 111 個測試仍通過

## 🏃‍♂️ 開始開發

```powershell
# 開始新的 BDD 循環
code tests/test_sltiu_instruction.cpp  # 先寫測試
code src/Instruction.h                # 再宣告類別  
code src/Instruction.cpp              # 最後實作邏輯
```

**記住**: 小步前進，測試驅動，保持簡潔！ 🎯
