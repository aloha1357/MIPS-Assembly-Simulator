# MIPS Assembly Simulator - 開發進度報告 (Sprint 5)

**報告日期**: 2025年7月31日  
**開發階段**: Sprint 5 - GUI 開發 (BDD方法)  
**測試狀態**: 64/64 測試通過 (100% 通過率)  
**專案狀態**: 核心功能完成，GUI 設計階段完成

---

## 📊 專案當前狀態

### ✅ **完全完成的模組 (100%)**
| 模組 | 狀態 | 測試覆蓋 | 備註 |
|------|------|----------|------|
| **核心指令執行** | ✅ 完成 | 12/12 | 支援 7 個 MIPS 指令 |
| **單週期模式** | ✅ 完成 | 5/5 | 完全穩定 |
| **管線架構** | ✅ 完成 | 14/14 | 5 階段框架 |
| **系統調用** | ✅ 完成 | 7/7 | 4 個 syscall |
| **指令解碼器** | ✅ 完成 | 11/11 | 32位元解碼 |
| **記憶體系統** | ✅ 完成 | 3/3 | 字組對齊 |
| **暫存器檔案** | ✅ 完成 | 3/3 | 32個MIPS暫存器 |

### 🚀 **新增模組 (進行中)**
| 模組 | 狀態 | 完成度 | 下一步 |
|------|------|--------|-------|
| **GUI BDD 規格** | ✅ 完成 | 100% | 37 scenarios 設計完成 |
| **GUI 測試框架** | ✅ 完成 | 100% | 測試架構就緒 |
| **GUI 介面設計** | ✅ 完成 | 100% | API 設計完成 |
| **GUI 基礎實作** | ⏳ 進行中 | 80% | Headless 版本完成 |
| **真實 GUI 框架** | ❓ 未開始 | 0% | 需選擇 Qt/ImGui |

---

## 🏗️ 專案架構詳解

### 目錄結構 & 責任分工
```
MIPS-Assembly-Simulator/
├── 📁 src/                          # 核心原始碼
│   ├── 🧠 Cpu.cpp/h                 # 主控制器 - 管線/單週期切換
│   ├── 📋 RegisterFile.cpp/h        # 32個MIPS暫存器管理
│   ├── 🗃️ Memory.cpp/h              # 4KB記憶體系統
│   ├── 🔧 Assembler.cpp/h           # MIPS組語解析器
│   ├── 🔍 InstructionDecoder.cpp/h  # 32位元指令解碼
│   ├── 📝 Instruction.cpp/h         # 指令基類與實作
│   ├── 🔄 *Stage.cpp/h              # 管線5階段實作
│   └── 📁 gui/                      # 🆕 GUI模組 (新增)
│       ├── MipsSimulatorGUI.h       # GUI介面定義
│       └── MipsSimulatorGUI.cpp     # GUI基礎實作
├── 📁 tests/                        # 測試套件
│   ├── test_*.cpp                   # 單元測試 (64個)
│   └── test_gui_interface.cpp       # 🆕 GUI BDD測試
├── 📁 features/                     # BDD功能規格
│   ├── core_instructions.feature   # 核心指令規格
│   ├── pipeline.feature            # 管線功能規格
│   └── gui_interface.feature       # 🆕 GUI功能規格 (37 scenarios)
├── 📁 build/                        # 建置輸出
│   ├── src/Debug/                   # 執行檔輸出
│   └── tests/Debug/                 # 測試執行檔
└── 📄 *.md                          # 技術文檔
```

### 核心類別關係圖
```
┌─────────────────┐
│      Cpu        │ ◄─── 主控制器，管理所有組件
│  (主控制器)      │
└─────────────────┘
         │
         ├── RegisterFile (暫存器檔案)
         ├── Memory (記憶體系統)
         ├── Assembler (組譯器)
         ├── Instructions (指令實作)
         ├── Pipeline Stages (管線階段)
         │   ├── IFStage → IDStage → EXStage → MEMStage → WBStage
         │   └── Pipeline Registers (管線暫存器)
         └── 🆕 MipsSimulatorGUI (圖形化介面)
```

---

## 🔧 開發系統使用指南

### 環境設定 & 建置命令

#### **基本環境需求**
```bash
# 作業系統: Windows 10/11
# 編譯器: Visual Studio 2022 (MSVC)
# CMake: 3.20+
# Shell: PowerShell (強烈建議)
```

#### **專案初始化**
```powershell
# 1. 克隆專案
git clone https://github.com/aloha1357/MIPS-Assembly-Simulator.git
cd MIPS-Assembly-Simulator

# 2. 建立建置目錄
cmake -B build -G "Visual Studio 17 2022"

# 3. 首次建置 (包含下載依賴)
cmake --build build --config Debug
```

#### **日常開發命令**
```powershell
# 快速編譯 (增量編譯)
cmake --build build --config Debug

# 清理重建
cmake --build build --target clean
cmake --build build --config Debug

# Release 版本編譯
cmake --build build --config Release

# 查看建置詳細資訊
cmake --build build --config Debug --verbose
```

### 測試執行命令

#### **運行所有測試**
```powershell
# 絕對路徑執行 (推薦)
C:\Users\aloha\Documents\GitHub\MIPS-Assembly-Simulator\build\tests\Debug\unit_tests.exe

# 相對路徑 (需在專案根目錄)
.\build\tests\Debug\unit_tests.exe
```

#### **特定測試套件**
```powershell
# 管線相關測試
.\build\tests\Debug\unit_tests.exe --gtest_filter="*Pipeline*"

# 指令執行測試
.\build\tests\Debug\unit_tests.exe --gtest_filter="*Instruction*"

# BDD 測試
.\build\tests\Debug\unit_tests.exe --gtest_filter="*BDD*"

# GUI 測試 (完成後可用)
.\build\tests\Debug\unit_tests.exe --gtest_filter="*GUI*"

# 單一測試
.\build\tests\Debug\unit_tests.exe --gtest_filter="PipelineExecutionTest.BasicPipelineExecution"
```

#### **測試輸出選項**
```powershell
# 顯示執行時間
.\build\tests\Debug\unit_tests.exe --gtest_print_time=1

# 列出所有測試
.\build\tests\Debug\unit_tests.exe --gtest_list_tests

# 詳細錯誤訊息
.\build\tests\Debug\unit_tests.exe --gtest_break_on_failure
```

### Visual Studio 整合

#### **開啟 Visual Studio**
```powershell
# 開啟 Solution 檔案
start .\build\mips-assembly-simulator.sln

# 或直接用 VS 開啟專案資料夾
code .  # 如果使用 VS Code
```

#### **除錯設定**
- **中斷點除錯**: 在 Visual Studio 中設定中斷點
- **測試除錯**: 右鍵單一測試 → Debug
- **記憶體檢查**: 使用 VS 的診斷工具

---

## 🚧 開發過程中遇到的問題 & 解決方案

### 1. **管線 PC 更新時序問題** ⚠️ **重大問題**

**問題描述**:
```
管線模式下，WBStage 執行指令時會修改 PC，
與 IFStage 的 PC 管理機制產生衝突，導致 PC 值錯亂。
```

**錯誤症狀**:
- 管線執行結果與單週期不一致
- PC 跳躍異常
- 測試 `PipelineExecutionTest` 失敗

**解決方案** (已修復):
```cpp
// 在 WBStage.cpp 中實作 PC 保存/恢復機制
void WBStage::execute() {
    if (hasValidInstruction()) {
        // 💡 關鍵：保存當前 PC 值
        uint32_t savedPC = m_cpu->getRegisterFile().getPC();
        
        // 執行指令 (可能會修改 PC)
        getInstruction()->execute(*m_cpu);
        
        // 💡 關鍵：在管線模式下恢復 PC
        if (m_cpu->isPipelineMode()) {
            m_cpu->getRegisterFile().setPC(savedPC);
        }
    }
}
```

**教訓**: 管線處理器中的 PC 管理需要特別小心，避免多個階段同時修改 PC。

### 2. **Syscall 終止問題** ⚠️ **測試失敗**

**問題描述**:
```
測試程式缺少 $v0 暫存器設定，導致 syscall 無法正確終止程式。
```

**錯誤症狀**:
- 程式執行無限迴圈
- 管線測試超時
- `PipelineVsSingleCycleComparison` 失敗

**修正範例**:
```assembly
# ❌ 錯誤寫法
addi $t0, $zero, 5
syscall  # $v0 未設定，無法執行

# ✅ 正確寫法  
addi $t0, $zero, 5
addi $v0, $zero, 10  # 設定 exit syscall
syscall              # 正確終止
```

**教訓**: 所有測試程式都必須正確設定 syscall 參數，特別是 `$v0` 暫存器。

### 3. **編譯器編碼警告** ⚠️ **編譯問題**

**問題描述**:
```
MSVC 編譯器報告檔案編碼警告，導致編譯失敗：
warning C4819: 檔案含有無法在目前字碼頁 (950) 中表示的字元
```

**暫時解決方案**:
```cmake
# 在 CMakeLists.txt 中暫時註釋 GUI 檔案
# gui/MipsSimulatorGUI.cpp  # Temporarily commented out due to encoding issues
```

**長期解決方案**:
- 將所有檔案另存為 UTF-8 BOM 格式
- 或在 CMake 中設定編譯器選項: `/utf-8`

### 4. **Windows 路徑問題** ⚠️ **執行環境**

**問題描述**:
```
Windows 環境下相對路徑經常失效，導致測試找不到執行檔。
```

**解決方案**:
- 使用絕對路徑執行測試
- 確保在正確的工作目錄執行命令
- PowerShell 比 CMD 更穩定

**建議寫法**:
```powershell
# ✅ 推薦 - 絕對路徑
C:\full\path\to\project\build\tests\Debug\unit_tests.exe

# ⚠️ 需小心 - 相對路徑 (確保在專案根目錄)
.\build\tests\Debug\unit_tests.exe
```

---

## 📋 目前支援的功能清單

### MIPS 指令集 (7個指令)
| 類型 | 指令 | 語法 | 狀態 | 測試 |
|------|------|------|------|------|
| **R-Type** | ADD | `add $rd, $rs, $rt` | ✅ | ✅ |
| **R-Type** | SUB | `sub $rd, $rs, $rt` | ✅ | ✅ |
| **I-Type** | ADDI | `addi $rt, $rs, imm` | ✅ | ✅ |
| **I-Type** | LW | `lw $rt, offset($rs)` | ✅ | ✅ |
| **I-Type** | SW | `sw $rt, offset($rs)` | ✅ | ✅ |
| **I-Type** | BEQ | `beq $rs, $rt, label` | ✅ | ✅ |
| **J-Type** | J | `j address` | ✅ | ✅ |
| **特殊** | SYSCALL | `syscall` | ✅ | ✅ |

### 系統調用 (4個 Syscall)
| $v0 | 功能 | 描述 | 輸入 | 輸出 | 狀態 |
|-----|------|------|------|------|------|
| **1** | print_int | 印出整數 | $a0 = 整數值 | 控制台 | ✅ |
| **4** | print_string | 印出字串 | $a0 = 字串位址 | 控制台 | ✅ |
| **5** | read_int | 讀取整數 | - | $v0 = 值 | ✅ |
| **10** | exit | 程式終止 | - | 程式結束 | ✅ |

### 執行模式
- ✅ **單週期模式**: 一個時脈週期執行一個指令
- ✅ **管線模式**: 5階段管線 (IF → ID → EX → MEM → WB)
- ✅ **模式切換**: 執行時動態切換

---

## 🎨 GUI 開發成果 (BDD 方法)

### 已完成的 BDD 階段

#### 1. **Feature 規格定義** ✅
- **檔案**: `features/gui_interface.feature`
- **內容**: 37 個 Gherkin scenarios
- **涵蓋**: 完整 GUI 使用者體驗

#### 2. **測試框架設計** ✅
- **檔案**: `tests/test_gui_interface.cpp`
- **架構**: Google Test + BDD pattern
- **特色**: Headless 測試支援

#### 3. **介面 API 設計** ✅
- **檔案**: `src/gui/MipsSimulatorGUI.h`
- **設計原則**: 模組化、可測試、可擴展
- **介面數量**: 50+ 公開方法

#### 4. **基礎實作** 🚧 進行中
- **檔案**: `src/gui/MipsSimulatorGUI.cpp`
- **狀態**: Mock/Simulation 版本 80% 完成
- **問題**: 編碼問題暫時阻擋編譯

### GUI 功能規格摘要
```gherkin
Feature: 主要 GUI 功能
  - 程式碼編輯器 (語法高亮、檔案操作)
  - 暫存器檢視器 (即時更新、變更高亮)
  - 記憶體檢視器 (十六進制、位址跳轉)
  - 管線視覺化 (5階段、狀態追蹤)
  - 執行控制 (Run/Step/Reset/Stop)
  - 除錯功能 (中斷點、錯誤處理)
  - 模式切換 (單週期 ↔ 管線)
```

---

## 🚀 下一階段開發建議

### 優先級排序

#### **P0 - 緊急** (1週內)
1. **解決 GUI 編碼問題**
   - 將檔案轉存為 UTF-8 BOM
   - 或設定 CMake 編譯器選項
   - 恢復 GUI 編譯能力

2. **選擇 GUI 框架**
   - **選項 A**: Qt 6 (專業級，學習曲線較陡)
   - **選項 B**: Dear ImGui (輕量級，容易整合)
   - **建議**: 先用 Dear ImGui 快速原型，再考慮 Qt

#### **P1 - 重要** (2-4週)
3. **實作核心 GUI 組件**
   ```cpp
   // 開發順序建議
   1. 主視窗框架
   2. 程式碼編輯器 (基本功能)
   3. 暫存器檢視器
   4. 簡單執行控制
   5. 控制台輸出
   ```

4. **整合 GUI 與核心**
   - 連接 GUI 事件與 CPU 操作
   - 實作即時狀態更新
   - 測試 GUI 與核心互動

#### **P2 - 功能增強** (1-2月)
5. **進階 GUI 功能**
   - 記憶體檢視器
   - 管線視覺化
   - 中斷點系統
   - 語法高亮

6. **使用者體驗優化**
   - 動畫效果
   - 快捷鍵
   - 工具提示
   - 錯誤處理改善

### 技術架構建議

#### **GUI 框架整合模式**
```cpp
// 建議架構：橋接模式
class MipsSimulatorGUI {          // 抽象介面 (已完成)
public:
    virtual void setCodeEditorText(const std::string& text) = 0;
    // ... 其他介面方法
};

class QtMipsSimulatorGUI : public MipsSimulatorGUI {    // Qt 實作
    // Qt 特定實作
};

class ImGuiMipsSimulatorGUI : public MipsSimulatorGUI { // ImGui 實作
    // ImGui 特定實作
};
```

#### **資料流設計**
```
User Input → GUI Events → CPU Operations → State Changes → GUI Updates
     ↑                                                           ↓
     └─────────────── Real-time Feedback Loop ──────────────────┘
```

---

## ⚠️ 重要注意事項 & 陷阱

### 開發環境注意事項

#### **1. Windows 特定問題**
```powershell
# ❌ 避免這些問題
- 使用中文路徑名稱
- 在 CMD 中執行複雜命令
- 使用相對路徑執行測試

# ✅ 建議做法
- 使用英文路徑
- 優先使用 PowerShell
- 使用絕對路徑執行
```

#### **2. Visual Studio 設定**
- 確保使用 **Visual Studio 2022** (不是 VS Code)
- 開啟 **C++17** 支援
- 安裝 **CMake** 整合工具

#### **3. Git 使用注意**
```bash
# 設定正確的行結束符
git config core.autocrlf true

# 處理大檔案 (如果需要)
git lfs track "*.exe"
git lfs track "*.lib"
```

### 程式碼開發注意事項

#### **1. 記憶體管理**
```cpp
// ✅ 使用智慧指標
std::unique_ptr<Instruction> instruction = std::make_unique<AddInstruction>();

// ✅ RAII 原則
class ResourceManager {
public:
    ResourceManager() { acquire(); }
    ~ResourceManager() { release(); }
};

// ❌ 避免裸指標
Instruction* instruction = new AddInstruction(); // 不要這樣做
```

#### **2. 測試撰寫原則**
```cpp
// ✅ BDD 風格測試
TEST_F(TestClass, FeatureName) {
    // Given: 設定測試環境
    setup_test_environment();
    
    // When: 執行操作
    auto result = system_under_test.perform_action();
    
    // Then: 驗證結果
    EXPECT_EQ(expected_value, result);
}

// ✅ 測試隔離
void SetUp() override {
    // 每個測試都重新初始化
    cpu = std::make_unique<Cpu>();
}
```

#### **3. 錯誤處理模式**
```cpp
// ✅ 使用異常處理關鍵邏輯錯誤
if (registerNumber < 0 || registerNumber >= 32) {
    throw std::out_of_range("Invalid register number");
}

// ✅ 使用返回值處理可預期錯誤
bool loadProgram(const std::string& assembly) {
    try {
        assembler.parse(assembly);
        return true;
    } catch (const std::exception&) {
        return false;  // 解析失敗是可預期的
    }
}
```

### 效能優化建議

#### **1. 編譯時最佳化**
```cpp
// ✅ 使用 const 引用避免複製
void processInstruction(const Instruction& instruction);

// ✅ 使用 move 語義
std::vector<std::unique_ptr<Instruction>> instructions;
instructions.emplace_back(std::make_unique<AddInstruction>());

// ✅ 避免在迴圈中分配記憶體
std::vector<uint32_t> results;
results.reserve(expected_size);  // 預先分配
```

#### **2. 除錯與profiling**
```cpp
// ✅ 條件編譯除錯資訊
#ifdef DEBUG_PIPELINE
    std::cout << "Pipeline state: " << getPipelineState() << std::endl;
#endif

// ✅ 使用 assert 檢查前置條件
assert(pc < m_instructions.size() && "PC out of bounds");
```

---

## 📚 學習資源 & 參考文件

### 技術參考

#### **MIPS 架構**
- [MIPS32 指令集手冊](https://www.mips.com/products/architectures/mips32-2/)
- [計算機組織與設計 Patterson & Hennessy](https://www.amazon.com/Computer-Organization-Design-MIPS-Architecture/dp/0124077269)
- [MIPS 指令參考卡](https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_help.html)

#### **C++ 開發**
- [Modern C++ Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/)

#### **測試與 BDD**
- [Google Test Documentation](https://google.github.io/googletest/)
- [BDD 入門指南](https://cucumber.io/docs/bdd/)
- [Gherkin 語法參考](https://cucumber.io/docs/gherkin/)

#### **GUI 框架**
- [Qt 6 Documentation](https://doc.qt.io/qt-6/)
- [Dear ImGui GitHub](https://github.com/ocornut/imgui)
- [ImGui 學習指南](https://github.com/ocornut/imgui/wiki)

### 內部文檔
- `ARCHITECTURE_DECISIONS.md` - 架構決策記錄
- `QUICK_REFERENCE.md` - 快速參考
- `GUI_BDD_DEVELOPMENT_SUMMARY.md` - GUI BDD 開發總結
- `features/*.feature` - BDD 功能規格

---

## 🔄 交接檢查清單

### 環境設定確認
- [ ] Visual Studio 2022 已安裝
- [ ] CMake 3.20+ 已安裝
- [ ] PowerShell 可正常使用
- [ ] Git 設定正確

### 專案狀態確認
- [ ] 可以成功編譯專案：`cmake --build build --config Debug`
- [ ] 所有測試通過：64/64 tests passed
- [ ] 可以執行基本功能測試
- [ ] 文檔已更新到最新狀態

### 開發工具確認
- [ ] 了解基本 CMake 指令
- [ ] 熟悉 Google Test 使用方法
- [ ] 理解 BDD 開發流程
- [ ] 掌握 Git 基本操作

### 後續開發準備
- [ ] 閱讀 GUI BDD 規格文件
- [ ] 選擇 GUI 框架 (Qt vs ImGui)
- [ ] 解決編碼問題
- [ ] 開始實作第一個 GUI 組件

---

## 📞 支援 & 聯絡

### 問題回報
- **GitHub Issues**: [專案 Issues 頁面](https://github.com/aloha1357/MIPS-Assembly-Simulator/issues)
- **文檔問題**: 直接修改對應的 `.md` 檔案
- **程式問題**: 提供詳細的錯誤訊息與重現步驟

### 開發建議
- **先看文檔**: 遇到問題先查閱現有文檔
- **小步迭代**: 每次只做一個小改動並測試
- **保持測試**: 任何修改後都要確保測試通過
- **更新文檔**: 新功能要同步更新文檔

---

**專案交接完成日期**: 2025年7月31日  
**交接負責人**: MIPS Assembly Simulator Development Team  
**下一階段目標**: GUI 框架選擇與實作

> 🎉 **專案已達成重要里程碑！** 核心 MIPS 模擬器功能完整，管線架構穩定，GUI 設計完備。下一位開發者可以專注於 GUI 實作，將這個強大的教學工具帶給更多使用者！

**祝開發順利！** 🚀
