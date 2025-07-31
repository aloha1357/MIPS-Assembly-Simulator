# 🎉 MIPS Assembly Simulator - Sprint 3 開發完成報告

**完成日期**: 2025年7月31日  
**開發者**: GitHub Copilot  
**Sprint 目標**: 實現系統調用支援與32位元指令解碼器  

## 📊 開發成果總覽

### ✅ 完成的主要功能

#### 1. 系統調用機制 (SyscallInstruction)
- **syscall 1**: `print_int` - 列印整數到控制台
- **syscall 4**: `print_string` - 從記憶體讀取字串並列印
- **syscall 5**: `read_int` - 從控制台讀取整數輸入
- **syscall 10**: `exit` - 優雅終止程式執行

**技術特點**:
- 遵循標準 MIPS ABI 暫存器約定 ($v0, $a0)
- 完整的控制台I/O抽象層
- 程式終止狀態管理
- 記憶體字串讀取 (支援null結尾字串)

#### 2. 32位元指令解碼器 (InstructionDecoder)
- **R型指令**: ADD, SUB, SYSCALL
- **I型指令**: ADDI, LW, SW, BEQ  
- **J型指令**: J (跳躍)

**技術特點**:
- 精確的位元欄位提取 (opcode, rs, rt, rd, immediate, function)
- 16位元立即值符號擴展
- 模組化解碼架構 (按指令類型分類)
- 錯誤處理 (未知指令回傳 nullptr)

### 📈 測試覆蓋率統計

| 測試類別 | 測試數量 | 狀態 | 描述 |
|---------|---------|------|------|
| 基礎單元測試 | 21 | ✅ 全通過 | CPU, RegisterFile, Memory, Instructions |
| BDD 場景測試 | 11 | ✅ 全通過 | 核心指令行為驗證 |
| 管線架構測試 | 9 | ✅ 全通過 | 管線模式切換與整合 |
| **系統調用測試** | **7** | ✅ **新增** | print_int, print_string, read_int, exit |
| **指令解碼器測試** | **11** | ✅ **新增** | R/I/J型指令解碼與執行 |
| **總計** | **59** | ✅ **100%** | 相比 Sprint 2 增加 18個測試 (+43%) |

### 🏗️ 架構改進

#### 新增檔案 (4個)
```
src/InstructionDecoder.h        (30 行)  - 指令解碼器介面
src/InstructionDecoder.cpp      (120 行) - 32位元解碼實現  
tests/test_syscalls.cpp         (150 行) - 系統調用測試
tests/test_instruction_decoder.cpp (200 行) - 解碼器測試
```

#### 擴展現有檔案 (5個)
```
src/Instruction.h/.cpp         - 新增 SyscallInstruction
src/Cpu.h/.cpp                 - 新增控制台I/O和終止支援
src/Assembler.cpp              - 新增 syscall 指令解析
tests/CMakeLists.txt           - 整合新測試檔案
src/CMakeLists.txt             - 整合新源碼檔案
```

### 🔧 技術實現亮點

#### 1. 系統調用架構設計
```cpp
class SyscallInstruction : public Instruction {
    void execute(Cpu& cpu) override {
        uint32_t syscallNumber = cpu.getRegisterFile().read(2); // $v0
        switch (syscallNumber) {
            case 1: handlePrintInt(cpu); break;
            case 4: handlePrintString(cpu); break;
            case 5: handleReadInt(cpu); break;
            case 10: handleExit(cpu); break;
        }
    }
};
```

#### 2. 指令解碼器位元操作
```cpp
static uint32_t extractOpcode(uint32_t word) {
    return (word >> 26) & 0x3F; // bits 31-26
}

static uint32_t extractRs(uint32_t word) {
    return (word >> 21) & 0x1F; // bits 25-21
}
```

#### 3. 控制台I/O抽象
```cpp
class Cpu {
    void printInt(uint32_t value);        // 系統調用輸出
    void printString(const std::string&); // 字串輸出
    uint32_t readInt();                   // 整數輸入
    bool shouldTerminate() const;         // 終止狀態檢查
};
```

## 🎯 開發品質保證

### 測試驅動開發 (TDD)
1. **先寫測試**: 每個新功能都先編寫測試案例
2. **紅-綠-重構**: 測試失敗 → 實現功能 → 測試通過 → 程式碼重構
3. **回歸測試**: 確保新功能不破壞現有功能

### 程式碼品質標準
- ✅ 所有編譯警告解決
- ✅ 遵循現有程式碼風格
- ✅ 完整的錯誤處理
- ✅ 詳細的程式碼註解
- ✅ 向後相容性保證

### 效能考量
- 指令解碼使用位元操作，執行效率高
- 系統調用採用 switch-case，O(1) 時間複雜度
- 記憶體字串讀取有邊界檢查，防止無限迴圈

## 🚀 下一階段建議

### 優先級 P1: 管線執行整合
當前狀態：管線架構已完成，需要整合到CPU執行流程
預計工作量：2-3週
關鍵挑戰：管線與單週期模式的動態切換

### 優先級 P2: 危險處理機制
目標：實現 Load-Use 危險偵測和基本停頓機制
預計工作量：1-2週
基礎：現有管線架構為危險處理提供了良好基礎

### 優先級 P3: 指令集擴展
建議順序：邏輯指令 → 位移指令 → 比較指令 → 分支指令
現狀：解碼器和指令架構已支援輕鬆擴展

## 💡 開發心得與經驗

### 成功因素
1. **模組化設計**: 新功能獨立封裝，不影響現有系統
2. **測試先行**: TDD 方法確保功能正確性和穩定性
3. **文檔完整**: 詳細的開發報告為後續開發提供指導

### 技術挑戰與解決
1. **指標類型轉換**: unique_ptr 與原始指標的正確使用
2. **記憶體存取**: 字組對齊和邊界檢查的平衡
3. **標籤處理**: 分支指令的標籤解析簡化設計

### 對後續開發者的建議
1. **保持測試覆蓋率**: 目標是超過 65 個測試
2. **維護向後相容**: 單週期模式必須繼續正常工作
3. **關注效能**: 管線模式的效能優化是關鍵

## 🎊 結語

Sprint 3 成功實現了兩個重要里程碑：**系統調用機制**和**32位元指令解碼器**。這兩個功能大大增強了 MIPS 模擬器的實用性和完整性。測試覆蓋率從 41 個增加到 59 個 (+43%)，程式碼品質和穩定性得到顯著提升。

專案現在具備了：
- ✅ 完整的核心指令集 (8個指令)
- ✅ 系統調用支援 (4個核心系統調用)
- ✅ 32位元機器碼解碼能力
- ✅ 管線架構框架
- ✅ 豐富的測試套件 (59個測試)

為下一階段的管線整合和危險處理實現奠定了堅實的基礎。

---
**開發完成**: 2025年7月31日  
**版本**: Sprint 3 - 系統調用與指令解碼器實現  
**測試狀態**: 59/59 通過 (100%) ✅  
**下一步**: 管線執行整合與危險處理機制
