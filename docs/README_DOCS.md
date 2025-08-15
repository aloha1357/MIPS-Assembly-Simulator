# MIPS Assembly Simulator - 文件索引

**專案版本**：Dear ImGui Version 1.1 - 邏輯指令擴展版  
**最後更新**：2025年8月15日  
**專案狀態**：🔄 進行中 - 邏輯指令模組開發

## 📚 文件結構概覽

本專案的文件已重新整理為模組化結構，方便開發者和使用者快速找到所需資訊。

### 📁 目錄結構
```
docs/
├── CURRENT_PROJECT_STATUS_2025.md           # 🔥 當前專案狀態總覽
├── LOGICAL_INSTRUCTIONS_DEVELOPMENT_REPORT.md # 🆕 邏輯指令開發報告
├── XOR_NOR_QUICKSTART_GUIDE.md             # 🚀 XOR/NOR快速入門指南  
├── DEVELOPMENT_HANDOVER_CHECKLIST.md       # 📋 開發交接檢查清單
├── development-reports/                     # 📊 開發報告資料夾
├── user-guides/                            # 📖 使用者指南
├── architecture/                           # 🏗️ 架構設計文件
└── README_DOCS.md                          # 📋 本文件索引
```

---

## 🎯 快速導航

### 🆕 新用戶入門
1. **[當前專案狀態](CURRENT_PROJECT_STATUS_2025.md)** - 了解專案完成度和功能
2. **[GUI 使用手冊](user-guides/GUI_USER_MANUAL.md)** - 學習如何使用圖形介面
3. **[快速參考指南](../QUICK_REFERENCE.md)** - 常用指令和快捷操作

### 👨‍💻 開發者資源
1. **[邏輯指令開發報告](LOGICAL_INSTRUCTIONS_DEVELOPMENT_REPORT.md)** 🆕 - 最新開發成果
2. **[XOR/NOR 快速入門](XOR_NOR_QUICKSTART_GUIDE.md)** 🚀 - 下一階段實作指南
3. **[開發交接檢查清單](DEVELOPMENT_HANDOVER_CHECKLIST.md)** 📋 - 開發者必讀
4. **[開發移交報告](development-reports/DEVELOPMENT_HANDOVER_REPORT.md)** - 基礎開發指南
5. **[最終完成報告](development-reports/FINAL_COMPLETION_REPORT.md)** - 完整技術實現
6. **[架構決策文件](architecture/ARCHITECTURE_DECISIONS.md)** - 設計決策說明

### 🔍 詳細技術文件
1. **[核心指令設計](architecture/01_core_instructions.md)** - MIPS 指令實現
2. **[管線架構](architecture/02_pipeline.md)** - 5階段管線設計
3. **[BDD 開發總結](development-reports/GUI_BDD_DEVELOPMENT_SUMMARY.md)** - 測試驅動開發

---

## 📊 文件分類詳細說明

### 1. 開發報告類 (`development-reports/`)

#### 核心開發文件
- **`DEVELOPMENT_HANDOVER_REPORT.md`** 📋
  - **目的**：完整的開發者移交指南
  - **適用對象**：新加入開發團隊的程式設計師
  - **內容**：專案架構、編譯指令、測試流程、擴展指南

- **`FINAL_COMPLETION_REPORT.md`** 🎯
  - **目的**：專案最終完成狀態的詳細報告
  - **適用對象**：專案管理者、技術主管
  - **內容**：技術成就、解決的挑戰、系統規格

- **`DEVELOPMENT_REPORT.md`** 📝
  - **目的**：詳細的技術開發歷程文件
  - **適用對象**：技術研究者、未來維護者
  - **內容**：完整開發時間線、技術決策過程

#### Sprint 進度報告
- **`SPRINT3_COMPLETION_REPORT.md`** - 管線階段完成報告
- **`SPRINT5_PROGRESS_REPORT.md`** - GUI 設計階段進度
- **`SPRINT6_PROGRESS_REPORT.md`** - GUI 實現階段進度

#### 專業開發文件
- **`GUI_BDD_DEVELOPMENT_SUMMARY.md`** 🧪
  - **目的**：行為驅動開發 (BDD) 方法論應用總結
  - **適用對象**：軟體品質工程師、測試開發者
  - **內容**：37個 BDD 場景、測試策略、品質保證流程

- **`DEVELOPMENT_REPORT_OLD.md`** & **`DEVELOPMENT_REPORT_UPDATED.md`** 📚
  - **目的**：歷史開發文件保存
  - **適用對象**：研究專案演進過程的開發者
  - **內容**：早期設計決策、廢棄方案、學習歷程

### 2. 使用者指南類 (`user-guides/`)

- **`GUI_USER_MANUAL.md`** 🎮
  - **目的**：完整的 GUI 應用程式使用手冊
  - **適用對象**：學生、教師、MIPS 學習者
  - **內容**：
    - 介面概覽和佈局說明
    - 每個功能模組詳細操作指南
    - 鍵盤快捷鍵參考
    - MIPS 程式範例和學習建議
    - 常見問題解答

### 3. 架構設計類 (`architecture/`)

- **`ARCHITECTURE_DECISIONS.md`** 🏛️
  - **目的**：記錄重要的架構設計決策
  - **適用對象**：系統架構師、資深開發者
  - **內容**：設計原則、技術選型、權衡考量

- **`01_core_instructions.md`** ⚙️
  - **目的**：MIPS 核心指令集實現詳細說明
  - **適用對象**：編譯器開發者、指令集研究者
  - **內容**：8個指令的完整實現、編碼格式、執行邏輯

- **`02_pipeline.md`** 🔄
  - **目的**：5階段管線架構設計文件
  - **適用對象**：處理器設計者、電腦架構學習者
  - **內容**：管線階段定義、資料流、危機處理

---

## 📖 文件閱讀順序建議

### 🎓 學習 MIPS 彙編語言
```
1. CURRENT_PROJECT_STATUS_2025.md     (了解整體功能)
2. user-guides/GUI_USER_MANUAL.md     (學習操作介面)
3. architecture/01_core_instructions.md (深入指令集)
4. architecture/02_pipeline.md         (理解管線概念)
```

### 💻 參與專案開發
```
1. development-reports/DEVELOPMENT_HANDOVER_REPORT.md (開發環境設置)
2. development-reports/FINAL_COMPLETION_REPORT.md     (了解技術實現)
3. architecture/ARCHITECTURE_DECISIONS.md             (理解設計決策)
4. development-reports/GUI_BDD_DEVELOPMENT_SUMMARY.md (學習測試方法)
```

### 🔬 學術研究用途
```
1. development-reports/DEVELOPMENT_REPORT.md          (完整開發歷程)
2. development-reports/GUI_BDD_DEVELOPMENT_SUMMARY.md (方法論應用)
3. architecture/ (所有架構文件)                       (技術深度分析)
4. development-reports/SPRINT*.md                     (敏捷開發實踐)
```

### 🏫 教學使用
```
1. CURRENT_PROJECT_STATUS_2025.md         (課程介紹材料)
2. user-guides/GUI_USER_MANUAL.md         (學生操作手冊)
3. architecture/01_core_instructions.md   (指令集教學)
4. architecture/02_pipeline.md            (管線概念教學)
```

---

## 🔗 外部資源與參考文件

### 主要專案文件（根目錄）
- **`readme.md`** - 專案主要 README，包含快速開始指南
- **`QUICK_REFERENCE.md`** - 開發者日常參考手冊
- **`DEVELOPMENT_HISTORY.md`** - 專案歷史變更記錄

### 技術規格文件
- **`features/`** 目錄 - BDD 規格文件（37個測試場景）
- **`tests/`** 目錄 - 完整測試套件（64個單元測試）
- **`src/`** 目錄 - 完整原始碼實現

### 建置與部署
- **`CMakeLists.txt`** - CMake 建置配置
- **`build/`** 目錄 - 編譯輸出和執行檔

---

## 📊 文件統計資訊

### 文件完整度
- **開發報告**：9個檔案，100% 完成
- **使用者指南**：1個檔案，100% 完成  
- **架構文件**：3個檔案，100% 完成
- **總覆蓋率**：所有主要功能都有完整文件

### 內容統計
- **總頁數**：約150頁文件內容
- **程式碼範例**：50+ 個實際可執行範例
- **技術圖表**：20+ 個架構圖和流程圖
- **教學場景**：37個 BDD 學習場景

### 多語言支援
- **主要語言**：繁體中文
- **技術術語**：中英對照
- **程式碼註解**：英文為主
- **國際化**：考慮未來英文版本擴展

---

## 🚀 未來文件計畫

### 短期改進 (下一版本)
- [ ] 增加互動式教學指南
- [ ] 建立 FAQ 常見問題集
- [ ] 新增影片教學材料連結
- [ ] 完善錯誤代碼參考手冊

### 長期規劃
- [ ] 英文版本文件
- [ ] Web 版線上文件
- [ ] 整合式幫助系統
- [ ] 自動產生 API 文件

---

## 📞 文件回饋與貢獻

### 如何提供回饋
1. **內容錯誤**：請指出具體的檔案位置和錯誤內容
2. **新增建議**：說明需要新增的文件類型和目標讀者
3. **改進意見**：提供具體的改進方向和預期效果

### 文件貢獻指南
1. **格式標準**：遵循現有的 Markdown 格式規範
2. **中文撰寫**：使用繁體中文，專業術語提供英文對照
3. **結構一致**：參考現有文件的章節結構和編排方式
4. **實例豐富**：提供具體的程式碼範例和操作步驟

---

**總結**：本文件索引提供了 MIPS Assembly Simulator 專案的完整文件導航，無論您是初學者、開發者還是研究者，都能快速找到所需的資訊資源。專案文件結構清晰、內容豐富、持續更新，為專案的長期發展和知識傳承提供了堅實基礎。

**最後更新**：2025-07-31 | **維護者**：MIPS Simulator 開發團隊
