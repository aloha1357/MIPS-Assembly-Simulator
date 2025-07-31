# MIPS Assembly Simulator - 開發歷程記錄

## 📈 專案進展時間軸

### Sprint 1: 基礎架構建立 (2025年7月初)
- ✅ 建立 CMake 建置系統
- ✅ 配置 Google Test 測試框架
- ✅ 實作基本的 CPU, Memory, RegisterFile 類別
- ✅ 建立指令基礎架構

### Sprint 2: 核心指令實作 (2025年7月中)
- ✅ 實作 R-Type 指令 (ADD, SUB)
- ✅ 實作 I-Type 指令 (ADDI, LW, SW, BEQ)
- ✅ 實作 J-Type 指令 (J)
- ✅ 實作 SYSCALL 指令
- ✅ 完成組譯器基本功能

### Sprint 3: 管線架構建立 (2025年7月下旬)
- ✅ 實作 5 階段管線架構
- ✅ 建立 IF, ID, EX, MEM, WB 各階段
- ✅ 實作管線暫存器
- ✅ 支援管線/單週期模式切換

### Sprint 4: 管線執行整合與修復 (2025年7月31日)
- 🔧 **發現問題**: 管線執行測試失敗 (4/5 tests failed)
- 🔍 **根本原因分析**: PC 更新時序衝突
- 💡 **關鍵突破**: WBStage PC 保存/恢復機制
- ✅ **問題解決**: 所有 64 測試通過

---

## 🐛 解決的關鍵問題詳細記錄

### 問題 1: Pipeline PC 更新競態條件

**發現時間**: 2025年7月31日  
**問題現象**: 
```
PipelineExecutionTest.PipelineVsSingleCycleComparison - FAILED
PipelineExecutionTest.PipelineModeWithMemoryOperations - FAILED  
PipelineExecutionTest.PipelineModeToggling - FAILED
```

**分析過程**:
1. 發現程式在管線模式下無法正確終止
2. 程式超過最大循環次數限制 (MAX_CYCLES)
3. 追蹤發現是 PC 更新時序問題

**根本原因**:
```cpp
// 衝突流程:
// 1. IFStage::execute() 更新 PC (管線控制)
// 2. WBStage::execute() 呼叫 instruction->execute() 
// 3. instruction->execute() 又更新 PC (單週期邏輯)
// 4. 兩個 PC 更新機制衝突，造成 PC 值錯亂
```

**解決方案**:
```cpp
// WBStage.cpp - 新增 PC 保護機制
void WBStage::execute() {
    if (hasValidInstruction()) {
        // 保存管線控制的 PC 值
        uint32_t savedPC = m_cpu->getRegisterFile().getPC();
        
        // 執行指令 (可能修改 PC)
        getInstruction()->execute(*m_cpu);
        
        // 在管線模式下恢復 PC，讓管線控制 PC 更新
        if (m_cpu->isPipelineMode()) {
            m_cpu->getRegisterFile().setPC(savedPC);
        }
    }
}
```

**測試驗證**:
```
Before: 1/5 PipelineExecutionTest passed
After:  5/5 PipelineExecutionTest passed ✅
```

### 問題 2: Syscall 設定缺失

**發現時間**: 2025年7月31日  
**問題現象**: 測試程式無法正確終止

**問題程式碼**:
```assembly
# 錯誤 - 缺少 $v0 設定
addi $t0, $zero, 5
syscall  # $v0 = 0，無法識別的 syscall
```

**修正後**:
```assembly
# 正確 - 設定 exit syscall
addi $t0, $zero, 5
addi $v0, $zero, 10  # syscall 10 = exit
syscall
```

**影響範圍**:
- `PipelineVsSingleCycleComparison`
- `PipelineModeWithMemoryOperations` 
- `PipelineModeToggling`

---

## 💡 關鍵開發洞察

### 1. 管線設計的複雜性
**學習**: 管線處理器不是單純的單週期處理器加上階段分割，需要重新設計指令執行流程
**應用**: 在 WB 階段區分管線模式和單週期模式的 PC 更新行為

### 2. 測試驅動開發的重要性
**學習**: 詳細的測試幫助快速定位問題
**應用**: 透過 Google Test 的篩選功能快速聚焦問題區域

### 3. 架構決策的影響
**學習**: 早期的指令設計決策影響後續管線實作
**應用**: 在 WB 階段添加模式檢查，而不是重新設計整個指令架構

---

## 🛠️ 開發工具與技術棧

### 核心技術
- **語言**: C++17
- **建置系統**: CMake 3.20+
- **測試框架**: Google Test (gtest)
- **編譯器**: MSVC (Visual Studio 2022)
- **架構**: x64

### 開發工具
- **IDE**: Visual Studio Code / Visual Studio 2022
- **版本控制**: Git
- **除錯**: Visual Studio Debugger
- **文檔**: Markdown

### 依賴管理
```cmake
# CMakeLists.txt 關鍵配置
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
```

---

## 📊 程式碼質量指標

### 測試覆蓋率
- **單元測試**: 64 個測試案例
- **功能測試**: 100% 核心功能覆蓋
- **管線測試**: 完整的管線執行驗證
- **BDD 測試**: 商業邏輯驗證

### 程式碼組織
```
程式碼行數分布:
├── src/           ~2,000 行 (核心邏輯)
├── tests/         ~1,500 行 (測試程式碼)
├── headers/       ~800 行 (介面定義)
└── docs/          ~1,000 行 (文檔)
```

### 設計模式應用
- **Strategy Pattern**: 不同類型指令的執行策略
- **Template Method**: 管線階段的通用執行流程
- **Factory Pattern**: 指令創建和解碼
- **Observer Pattern**: 管線階段間的資料傳遞

---

## 🎯 效能考量與優化

### 記憶體使用
- 固定大小陣列優於動態分配
- 智慧指標管理物件生命週期
- 避免不必要的複製操作

### 執行效率
```cpp
// 優化範例: 使用 const 引用避免複製
void executeInstruction(const Instruction& instr) const;

// 優化範例: 預先分配固定大小陣列
std::array<uint32_t, 32> registers;  // 而非 std::vector
```

### 可擴展性設計
- 模組化的指令系統，易於添加新指令
- 抽象的管線階段，支援不同管線設計
- 可配置的記憶體大小和暫存器數量

---

## 🔮 技術債務與未來改進

### 當前限制
1. **資料危險**: 尚未實作 Data Forwarding
2. **控制危險**: 缺少 Branch Prediction
3. **指令集**: 僅支援 7 個基本指令
4. **快取**: 沒有 Cache 記憶體模擬

### 建議改進順序
1. **短期** (1-2週): 實作 Data Forwarding
2. **中期** (1-2月): 添加更多 MIPS 指令
3. **長期** (3-6月): 實作完整的 MIPS32 架構

### 架構重構考慮
- 考慮使用 Visitor Pattern 處理不同指令類型
- 評估引入 State Machine 管理管線狀態
- 考慮分離 ISA 定義和處理器實作

---

## 🏆 專案成就與學習成果

### 技術成就
- ✅ 成功實作 5 階段 MIPS 管線處理器
- ✅ 解決複雜的 PC 更新時序問題
- ✅ 建立完整的測試覆蓋體系
- ✅ 實現雙模式執行 (管線/單週期)

### 軟體工程實踐
- ✅ 測試驅動開發 (TDD)
- ✅ 持續整合 (CI) 建置
- ✅ 詳細的文檔記錄
- ✅ 模組化架構設計

### 個人技能提升
- 🎓 深入理解計算機架構
- 🎓 掌握 C++ 高級特性
- 🎓 學會複雜系統除錯
- 🎓 體驗完整軟體開發流程

---

**最終狀態**: 2025年7月31日 - 專案成功完成  
**測試結果**: 64/64 tests passed (100%)  
**下一步**: 準備擴展功能或移交給新的開發團隊

> 🎉 這個專案展現了從概念到實作的完整軟體開發歷程，成功克服了管線處理器設計的核心挑戰！
