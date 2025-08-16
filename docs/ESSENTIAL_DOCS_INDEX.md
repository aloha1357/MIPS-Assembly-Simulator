# MIPS Assembly Simulator - 文件組織結構

本專案採用簡化的文件組織結構，將所有重要資訊整合到核心文件中。

## 📁 新的文件結構

```
MIPS-Assembly-Simulator/
├── MIPS_SIMULATOR_COMPLETE_GUIDE.md    # 🎯 主要指南 - 包含所有核心資訊
├── README.md                           # 📄 專案概覽
├── QUICK_REFERENCE.md                  # ⚡ 快速參考
└── docs/                              # 📚 保留的重要文檔
    ├── ESSENTIAL_DOCS_INDEX.md         # 📋 重要文檔索引
    ├── user-guides/
    │   └── GUI_USER_MANUAL.md          # 🎮 GUI使用手冊
    └── architecture/
        ├── ARCHITECTURE_DECISIONS.md   # 🏗️ 架構決策
        ├── 01_core_instructions.md     # ⚙️ 核心指令
        └── 02_pipeline.md              # 🔄 管線架構
```

## 📋 文件說明

### 🎯 核心文件
- **`MIPS_SIMULATOR_COMPLETE_GUIDE.md`** - **主要文件**，包含：
  - 專案完整概覽
  - 技術架構詳細說明
  - 使用指南與範例
  - 開發歷程總結
  - 測試與品質保證
  - 所有重要資訊的統整

### 📚 保留的重要文檔
- **`docs/DEVELOPMENT_HANDOVER_REPORT.md`** - 🏆 **專案完成總結報告** (47/47指令，346測試)
- **`docs/NEXT_STAGE_DEVELOPMENT_GUIDE.md`** - 🚀 **下階段開發完整指南** (4個發展路徑)
- **`docs/user-guides/GUI_USER_MANUAL.md`** - 詳細的GUI使用說明
- **`docs/architecture/`** - 技術架構相關文檔
- **`README.md`** - 專案快速概覽

### 🗑️ 已整合/簡化的文檔
以下文檔的重要內容已整合到主要指南中：
- `docs/CURRENT_PROJECT_STATUS_2025.md` → 整合到完整指南
- `docs/development-reports/` 所有報告 → 開發歷程章節
- `docs/PROJECT_DEVELOPMENT_SUMMARY.md` → 專案概覽章節
- 重複的狀態報告 → 統一到完整指南

## 🎯 使用建議

### 新使用者
1. 先閱讀 **`MIPS_SIMULATOR_COMPLETE_GUIDE.md`** 了解整體
2. 參考 **`docs/user-guides/GUI_USER_MANUAL.md`** 學習GUI操作

### 開發者
1. 閱讀 **`MIPS_SIMULATOR_COMPLETE_GUIDE.md`** 的技術架構章節
2. 參考 **`docs/architecture/`** 目錄下的詳細技術文檔

### 快速查詢
- 使用 **`QUICK_REFERENCE.md`** 進行日常開發參考

---

**整理完成日期**：2025年8月15日  
**整理目標**：簡化文檔結構，提高可讀性，避免資訊重複
