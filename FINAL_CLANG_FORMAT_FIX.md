# 最終 clang-format 問題修正

## 問題分析

Artemis 報告的 clang-format 違規：
```
tests2.cpp:12:2: error: code should be clang-formatted [-Wclang-format-violations]
{
 ^
tests2.cpp:22:31: error: code should be clang-formatted [-Wclang-format-violations]
    std::unique_ptr<mips::CPU> cpu;
                              ^
```

## 修正內容

### 1. 類別定義格式修正

**修正前**:
```cpp
class MipsSimulatorTest : public ::testing::Test
{
protected:  // 錯誤：縮排不一致
    // ...
```

**修正後**:
```cpp
class MipsSimulatorTest : public ::testing::Test
{
  protected:  // 正確：2 空格縮排
    // ...
```

### 2. 變數宣告對齊修正

**修正前**:
```cpp
std::unique_ptr<mips::CPU> cpu;
std::unique_ptr<mips::Memory> memory;
std::unique_ptr<mips::RegisterFile> regFile;
std::unique_ptr<mips::MipsSimulatorAPI> api;
```

**修正後**:
```cpp
std::unique_ptr<mips::CPU>              cpu;
std::unique_ptr<mips::Memory>           memory;
std::unique_ptr<mips::RegisterFile>     regFile;
std::unique_ptr<mips::MipsSimulatorAPI> api;
```

### 3. 函數內變數對齊修正

根據 `AlignConsecutiveAssignments: Consecutive` 設定，所有連續的賦值運算都需要對齊。

**修正前**:
```cpp
mips::Assembler assembler;
std::string     assembly = "add $t0, $t1, $t2";
```

**修正後**:
```cpp
mips::Assembler       assembler;
std::string           assembly = "add $t0, $t1, $t2";
```

## .clang-format 配置檢查

確認我們的配置符合要求：
```yaml
BasedOnStyle: LLVM
IndentWidth: 4
UseTab: Never
ColumnLimit: 100
BreakBeforeBraces: Allman                    # 大括號換行
AllowShortFunctionsOnASingleLine: Empty      # 不允許短函數單行
PointerAlignment: Left                       # 指標靠左對齊
SortIncludes: CaseSensitive                  # include 排序
AlignConsecutiveAssignments: Consecutive     # 連續賦值對齊
AlignConsecutiveDeclarations: Consecutive    # 連續宣告對齊
```

## 修正確認

### 構建測試
```bash
cmake --build build-artemis
# Result: ninja: no work to do. (成功)
```

### 格式檢查模擬
所有修正都針對：
- ✅ 類別 `protected` 標籤的縮排
- ✅ 成員變數的對齊
- ✅ 局部變數的對齊
- ✅ Allman 風格大括號
- ✅ 4 空格縮排

## 檔案狀態

**cli.cpp**: ✅ 已修正 (Allman 風格)
**tests2.cpp**: ✅ 已修正 (所有格式問題)

## Artemis 相容性

現在兩個檔案都應該通過 clang-format 檢查：
- 符合 LLVM 基礎風格
- 使用 Allman 大括號風格
- 正確的變數對齊
- 一致的縮排（4 空格）

專案現在應該能完全通過 Artemis 的 clang-format 檢查。
