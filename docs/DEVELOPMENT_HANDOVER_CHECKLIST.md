# 📋 邏輯指令開發交接檢查清單

## ✅ 交接前驗證（已完成）

### 環境和構建
- [x] CMake 配置正確
- [x] Ninja 構建系統工作正常  
- [x] GoogleTest 框架整合
- [x] 所有依賴項可用

### 程式碼品質
- [x] 編譯無警告或錯誤
- [x] 代碼遵循現有命名規範
- [x] 適當的註釋和文檔
- [x] 錯誤處理基礎框架

### 測試框架
- [x] Walking Skeleton 測試架構完整
- [x] BDD/TDD 工作流程建立
- [x] 測試隔離性良好
- [x] 測試數據和期望值正確

### 功能驗證
- [x] AND 指令完整實作和測試
- [x] OR 指令基本實作和測試
- [x] 指令解碼器正確整合
- [x] CPU 暫存器操作正確

## 📊 當前專案狀態

### 已實作指令統計
```
邏輯指令進度: 2/4 (50%)
├── AND  ✅ 完整實作 (基本+綜合+邊界測試)
├── OR   ✅ 基本實作 (基本測試通過)
├── XOR  ⏳ 框架就緒 (測試被禁用)
└── NOR  ⏳ 框架就緒 (測試被禁用)

總指令進度: 11/47 (23.4%)
```

### 測試執行狀態
```bash
# 最後測試執行結果：
[  PASSED  ] 6 tests.
[  DISABLED ] 2 tests.

# 具體測試：
✅ AND_BasicFunctionality_ShouldFail 
✅ CPU_BasicFunctionality_ShouldPass
✅ InstructionParsing_Framework_ShouldPass
✅ AND_ComprehensiveTests
✅ AND_EdgeCases  
✅ OR_BasicFunctionality_ShouldFail
🔲 DISABLED_XOR_BasicFunctionality
🔲 DISABLED_NOR_BasicFunctionality
```

## 🔄 接手者需要了解的關鍵概念

### 1. 原子化開發流程
```
紅燈 → 綠燈 → 重構 → 重複
  ↓      ↓       ↓      ↓
測試失敗→最小實作→代碼改善→下一功能
```

### 2. Walking Skeleton 方法
- **目的**: 建立端到端的最小工作系統
- **策略**: 一次實作一個指令
- **驗證**: 每個指令都要通過完整的執行路徑

### 3. 測試驅動開發
- **規則**: 先寫測試，再寫實作
- **原則**: 測試應該快速、獨立、可重複
- **實踐**: 使用描述性的測試名稱

## 🎯 下一個開發者的立即任務

### Priority 1: XOR 指令實作
- [ ] 啟用 XOR 測試（移除 DISABLED_ 前綴）
- [ ] 確認紅燈（測試失敗）
- [ ] 添加 XorInstruction 類別到 Instruction.h
- [ ] 實作 XOR 執行邏輯到 Instruction.cpp  
- [ ] 添加解碼器支援到 InstructionDecoder.cpp
- [ ] 更新測試解析器支援 XOR
- [ ] 確認綠燈（測試通過）

### Priority 2: NOR 指令實作
重複相同流程實作 NOR 指令

### Priority 3: 品質改善
- [ ] 為 OR/XOR/NOR 添加綜合測試
- [ ] 改善指令字串解析器
- [ ] 添加錯誤處理和驗證

## 🔧 技術細節參考

### MIPS R-type 指令格式
```
31-26  25-21  20-16  15-11  10-6   5-0
op     rs     rt     rd     shamt  funct
000000 rs     rt     rd     00000  funct

AND: funct = 0x24 (36)
OR:  funct = 0x25 (37) 
XOR: funct = 0x26 (38)
NOR: funct = 0x27 (39)
```

### 關鍵類別和函式
```cpp
// 核心類別
class RTypeInstruction;        // 基礎R-type指令類別
class LogicalInstructionWalkingSkeleton; // 測試類別

// 關鍵函式
void execute(Cpu& cpu);        // 指令執行
std::unique_ptr<Instruction> decode(uint32_t); // 指令解碼
void executeInstruction(const std::string&);   // 測試輔助
```

### 測試輔助函式
```cpp
void setRegister(const std::string& regName, uint32_t value);
void expectRegister(const std::string& regName, uint32_t expected);
int getRegisterNumber(const std::string& regName);
```

## 📁 重要檔案和位置

### 需要修改的檔案
```
src/Instruction.h              # 添加新指令類別宣告
src/Instruction.cpp            # 實作新指令執行邏輯  
src/InstructionDecoder.cpp     # 添加解碼器支援
tests/test_logical_walking_skeleton.cpp # 啟用和更新測試
```

### 參考檔案
```
docs/LOGICAL_INSTRUCTIONS_DEVELOPMENT_REPORT.md # 詳細開發報告
docs/XOR_NOR_QUICKSTART_GUIDE.md               # 快速入門指南
```

### 構建和測試命令
```bash
# 構建
cd build
ninja unit_tests

# 測試所有邏輯指令
.\tests\unit_tests.exe --gtest_filter="LogicalInstructionWalkingSkeleton.*"

# 測試特定指令
.\tests\unit_tests.exe --gtest_filter="*XOR*"
.\tests\unit_tests.exe --gtest_filter="*NOR*"
```

## ⚠️ 注意事項和陷阱

### 常見錯誤
1. **忘記添加包含檔案** - 記得在測試中包含新的指令標頭
2. **解碼器功能碼錯誤** - XOR=0x26, NOR=0x27
3. **位元運算符混淆** - XOR用^，NOR用~(a|b)
4. **測試數據計算錯誤** - 使用計算器驗證期望值

### 偵錯提示
1. **編譯錯誤** - 檢查標頭檔包含和類別宣告
2. **連結錯誤** - 確認在 CMakeLists.txt 中包含所有原始檔
3. **測試失敗** - 逐步檢查：解析→解碼→執行→寫入→讀取
4. **結果不正確** - 用二進制計算驗證位元運算

## 📞 支援和資源

### 程式碼風格參考
- 查看現有的 AddInstruction 和 SubInstruction 實作
- 遵循相同的註釋和命名模式
- 保持函式簡潔和單一責任

### 測試模式參考  
- 參考 AND_ComprehensiveTests 的測試結構
- 使用描述性的測試案例名稱
- 包含正常情況、邊界值和錯誤條件

### 故障排除
如果遇到問題：
1. 檢查最近的工作提交
2. 確認環境配置沒有改變
3. 重新運行已知工作的測試
4. 查閱開發報告中的技術細節

## ✅ 交接完成確認

### 交接者確認
- [x] 程式碼庫狀態良好
- [x] 所有測試通過
- [x] 文檔完整準備
- [x] 下一步驟清晰明確

### 接收者確認（請接收者填寫）
- [ ] 已閱讀開發報告
- [ ] 已閱讀快速入門指南
- [ ] 環境設置成功
- [ ] 現有測試執行成功
- [ ] 理解開發流程
- [ ] 準備開始實作

---

**交接日期**: 2025年8月15日  
**交接者**: GitHub Copilot  
**接收者**: _______________  
**預計完成時間**: _______________

**祝開發順利！** 🚀
