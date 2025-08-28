# MIPS 模擬器專案架構與設計

## 專案目錄結構

`
MIPS-Assembly-Simulator/
 src/                    # 核心模擬器程式碼
    Cpu.h/.cpp         # CPU 管線實現 (5階段)
    Memory.h/.cpp      # 記憶體管理系統
    RegisterFile.h/.cpp # 暫存器檔案
    Assembler.h/.cpp   # 組譯器 (雙階段)
    Instruction.h/.cpp # 指令類別層次結構
    IFStage.h/.cpp     # 取指階段
    IDStage.h/.cpp     # 解碼階段
    EXStage.h/.cpp     # 執行階段
    MEMStage.h/.cpp    # 記憶體階段
    WBStage.h/.cpp     # 寫回階段
    tools/             # 實用工具
        clean_runner.cpp     # 清除輸出執行器
        cli_trace_runner.cpp # 追蹤除錯器
        dump_labels.cpp      # 標籤分析器
 tests/                 # 測試套件
    test_*.cpp         # 各指令測試
    CMakeLists.txt     # 測試建構配置
 assignment/            # 最終專案交付
    test/              # 最終測試檔案
       instructions.asm    # 完整測試套件
       instructions.out    # 預期輸出
    report.md          # 專案報告
 CMakeLists.txt        # 主要建構配置
 README.md             # 專案說明
`

## MIPS 設計架構

### 1. 五階段管線設計
`
IF (Instruction Fetch) -> ID (Instruction Decode) -> EX (Execute) -> MEM (Memory) -> WB (Write Back)
`

### 2. 核心元件

#### CPU 類別 (Cpu.h/.cpp)
`cpp
class Cpu {
public:
    void tick();                    // 執行一個時鐘週期
    void loadProgram(string path);  // 載入程式
    void loadProgramFromString(string assembly); // 從字串載入
    
private:
    unique_ptr<RegisterFile> m_registerFile;  // 32個暫存器
    unique_ptr<Memory> m_memory;              // 4KB 記憶體
    unique_ptr<Assembler> m_assembler;        // 組譯器
    
    // 管線階段
    unique_ptr<IFStage> m_ifStage;
    unique_ptr<IDStage> m_idStage;
    unique_ptr<EXStage> m_exStage;
    unique_ptr<MEMStage> m_memStage;
    unique_ptr<WBStage> m_wbStage;
};
`

#### 指令類別層次結構 (Instruction.h)
`cpp
class Instruction {           // 基礎指令類別
    virtual void execute(Cpu& cpu) = 0;
    virtual string getName() const = 0;
};

class RTypeInstruction : public Instruction {  // R型指令 (3個暫存器)
    int m_rd, m_rs, m_rt;  // 目的、來源1、來源2
};

class ITypeInstruction : public Instruction {  // I型指令 (2個暫存器+立即數)
    int m_rt, m_rs, m_immediate;
};

class JTypeInstruction : public Instruction {  // J型指令 (跳躍)
    int m_address;
};
`

### 3. 支援的 MIPS 指令

#### 算術指令 (14個)
- dd/sub/addu/subu rd, rs, rt - 加減法
- ddi/addiu rt, rs, immediate - 加立即數
- mult/multu/div/divu rs, rt - 乘除法
- mfhi/mflo rd - 讀取 HI/LO 暫存器

#### 邏輯指令 (7個)
- nd/or/xor/nor rd, rs, rt - 位元運算
- ndi/ori/xori rt, rs, immediate - 位元運算立即數

#### 記憶體指令 (8個)
- lw/sw rt, offset(rs) - 載入/儲存字組
- lb/lbu rt, offset(rs) - 載入位元組 (有/無符號)
- lh/lhu rt, offset(rs) - 載入半字組 (有/無符號)
- sb/sh rt, offset(rs) - 儲存位元組/半字組

#### 跳躍指令 (4個)
- j label - 跳躍
- jal label - 跳躍並連結
- jr rs - 跳躍暫存器
- jalr rd, rs - 跳躍並連結暫存器

#### 分支指令 (6個)
- eq/bne rs, rt, label - 分支相等/不相等
- lez/bgtz rs, label - 分支小於等於/大於零
- ltz/bgez rs, label - 分支小於/大於等於零

#### 移位指令 (6個)
- sll/srl/sra rd, rt, shamt - 邏輯/算術移位
- sllv/srlv/srav rd, rt, rs - 可變移位

#### 比較指令 (3個)
- slt/sltu rd, rs, rt - 設定小於 (有/無符號)
- slti/sltiu rt, rs, immediate - 設定小於立即數

#### 載入立即數 (2個)
- lui rt, immediate - 載入上半部
- llo rt, immediate - 載入下半部

#### 系統呼叫 (8個)
- syscall 支援：
  - 1: print_int
  - 4: print_string  
  - 5: read_int
  - 8: read_string
  - 10: exit
  - 11: print_char
  - 12: read_char
  - 2: print_float

## 程式設計

### 1. 組譯器設計
專案使用雙階段組譯器：
- 第一階段：建立符號表，解析標籤
- 第二階段：產生機器碼

### 2. 管線實現
每個管線階段都實現為獨立的類別：
- IFStage: 從記憶體取指令
- IDStage: 解碼指令，讀取暫存器
- EXStage: 執行算術邏輯運算
- MEMStage: 處理記憶體存取
- WBStage: 寫回結果到暫存器

### 3. 記憶體系統
- 4KB 位址空間
- 字組對齊存取
- 支援位元組、半字組、字組操作

### 4. 暫存器檔案
- 32個通用暫存器 (0-31)
- 特殊用途暫存器 (ra, sp, 等)
- HI/LO 暫存器用於乘除法

## Windows 系統使用指南

### 1. 環境需求
- 編譯器: Visual Studio 2022 (MSVC) 或 MinGW
- 建構工具: CMake 3.20+
- 測試框架: Google Test

### 2. 建構專案

#### 基本建構
`ash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
`

#### 完整測試建構
`ash
cmake -B build-full -DFULL_TESTS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-full --config Release
`

### 3. 執行測試

#### 運行所有測試
`ash
./build/tests/mips_tests.exe
`

#### 運行特定測試類別
`ash
# 運行算術指令測試
./build/tests/mips_tests.exe --gtest_filter="*Arithmetic*"

# 運行記憶體指令測試
./build/tests/mips_tests.exe --gtest_filter="*Memory*"

# 運行跳躍指令測試
./build/tests/mips_tests.exe --gtest_filter="*Jump*"
`

#### 運行單一測試
`ash
# 運行特定測試案例
./build/tests/mips_tests.exe --gtest_filter="ArithmeticTest.AddInstruction"
`

### 4. 執行 MIPS 程式

#### 使用 CLI 執行器
`ash
# 執行完整測試套件
./build/src/clean_runner.exe assignment/test/instructions.asm

# 執行自訂程式
./build/src/clean_runner.exe your_program.asm
`

#### 使用主程式
`ash
# 載入並執行程式
./build/src/mips_cli.exe run assignment/test/instructions.asm
`

### 5. 測試輸出驗證

#### 比較實際輸出與預期輸出
`ash
# 產生實際輸出
./build/src/clean_runner.exe assignment/test/instructions.asm > actual_output.txt

# 使用 PowerShell 比較檔案
Compare-Object (Get-Content actual_output.txt) (Get-Content assignment/test/instructions.out)
`

### 6. 除錯功能

#### 使用追蹤執行器
`ash
# 顯示詳細執行追蹤
./build/src/cli_trace_runner.exe assignment/test/instructions.asm
`

#### 分析標籤
`ash
# 顯示程式中的所有標籤
./build/src/dump_labels.exe assignment/test/instructions.asm
`

## CLI 指令說明

### 1. 主要 CLI 工具

專案提供了兩套 CLI 工具系統來支援不同的使用情境：

#### A. 舊版簡單工具 (src/tools/)

##### clean_runner.exe - 清除輸出執行器
**用途**: 執行 MIPS 程式並產生詳細的執行追蹤和輸出

**語法**:
```bash
./build/src/clean_runner.exe <asm-file>
```

**功能特點**:
- 載入並執行指定的 MIPS 組譯程式
- 顯示詳細的執行追蹤資訊 (DEBUG/TRACE)
- 輸出程式執行的結果到控制台
- 將程式輸出寫入 `tmp_clean_utf8.out` 檔案
- 顯示每個指令的執行細節和暫存器變化

**實際輸出範例**:
```
DEBUG: Exec pc=0 instr='lhi'
TRACE: RegisterFile::write reg=4 value=65536
DEBUG: Exec pc=1 instr='llo'
TRACE: RegisterFile::write reg=4 value=65536
DEBUG: Exec pc=2 instr='trap'
TRACE: Cpu::printInt pc=2 value=65536
...
65536
131072
32768
```

##### cli_trace_runner.exe - 追蹤執行器
**用途**: 逐步執行程式並顯示程式計數器和返回地址

**語法**:
```bash
./build/src/cli_trace_runner.exe <asm-file> <maxSteps>
```

**功能特點**:
- 逐步執行程式，最多執行指定的步數
- 顯示每個步驟的程式計數器 (PC) 和返回地址暫存器 ($ra)
- 當程式終止或達到步數限制時自動停止
- 適合用於除錯和程式流程分析

**輸出格式**:
```
step=0 pc=0 $ra=0
step=1 pc=4 $ra=0
step=2 pc=8 $ra=16
```

##### dump_labels.exe - 標籤分析器
**用途**: 分析程式中的標籤、指令和資料結構

**語法**:
```bash
./build/src/dump_labels.exe <asm-file>
```

**功能特點**:
- 顯示程式中的所有標籤及其地址
- 列出所有指令及其原始碼
- 分析資料區段的結構
- 提供程式組譯的詳細統計資訊

**輸出資訊**:
- 指令數量和資料指示符統計
- 標籤映射表 (label -> address)
- 指令列表 (索引、指令名稱、原始碼)
- 資料區段分析

##### jal_debug_runner.exe - JAL 除錯器
**用途**: 專門用於除錯函數呼叫 (jal/jr) 的工具

**語法**:
```bash
./build/src/jal_debug_runner.exe <asm-file> [maxSteps]
```

**功能特點**:
- 專注於函數呼叫的除錯
- 顯示程式計數器和返回地址暫存器的變化
- 預設最大步數為1000，可自訂
- 提供函數呼叫流程的可視化

##### mips_cli.exe - 主程式
**用途**: 基本的 MIPS 程式載入和執行工具

**語法**:
```bash
./build/src/mips_cli.exe <assembly_file>
```

**功能特點**:
- 載入指定的組譯程式
- 預設執行10個週期
- 顯示執行週期數
- 簡單的程式測試工具

#### B. 新版完整CLI工具 (cli/)

##### mipsim.exe - 完整CLI工具
**用途**: 功能完整的MIPS模擬器CLI工具

**語法**:
```bash
./build/cli/mipsim.exe <command> [options]
```

**支援的命令**:
- `run` - 執行程式
- `assemble` - 組譯程式
- `disasm` - 反組譯程式
- `repl` - 互動式shell
- `dump` - 顯示狀態
- `help` - 顯示幫助
- `version` - 顯示版本

**主要功能**:
```bash
# 執行程式
./build/cli/mipsim.exe run program.asm --limit 1000 --trace regs

# 組譯程式
./build/cli/mipsim.exe assemble src.asm -o out.bin --map symbols.map

# 顯示版本
./build/cli/mipsim.exe --version
```

### 2. CLI 工具使用情境

#### 開發和測試階段
```bash
# 1. 分析程式結構
./build/src/dump_labels.exe program.asm

# 2. 逐步除錯
./build/src/cli_trace_runner.exe program.asm 50

# 3. 產生測試輸出
./build/src/clean_runner.exe program.asm > output.txt
```

#### 生產環境使用
```bash
# 快速執行程式
./build/src/clean_runner.exe program.asm

# 批次處理多個檔案
for %f in (*.asm) do ./build/src/clean_runner.exe "%f"
```

#### 除錯複雜程式
```bash
# 分析函數呼叫
./build/src/jal_debug_runner.exe complex_program.asm 200

# 詳細追蹤執行流程
./build/src/cli_trace_runner.exe complex_program.asm 1000
```

#### 使用新版CLI工具
```bash
# 設定執行限制
./build/cli/mipsim.exe run program.asm --limit 1000 --timeout 30

# 啟用追蹤
./build/cli/mipsim.exe run program.asm --trace regs
```

### 3. 錯誤處理

所有 CLI 工具都提供基本的錯誤處理：

- **檔案不存在**: 顯示 "file open failed" 錯誤
- **組譯失敗**: 顯示具體的組譯錯誤訊息
- **參數錯誤**: 顯示正確的使用語法
- **執行錯誤**: 顯示相關的錯誤資訊

### 4. 輸出格式說明

#### 控制台輸出
- 程式執行結果直接輸出到控制台
- 錯誤訊息輸出到 stderr
- 除錯資訊以結構化格式顯示

#### 檔案輸出
- `tmp_clean_utf8.out`: 程式輸出的 UTF-8 檔案
- 避免 PowerShell 的 UTF-16 重新導向問題
- 適合用於檔案比較和測試驗證

#### 追蹤輸出
- 顯示每個指令的執行細節
- 暫存器讀寫操作的追蹤
- 程式計數器的變化
- 記憶體存取操作的記錄

## 程式設計範例

### 基本算術操作
`mips
# 載入數字到暫存器
addi , , 15    #  = 15
addi , , 25    #  = 25

# 執行算術運算
add , ,       #  = 40 (15+25)
sub , ,       #  = 10 (25-15)

# 輸出結果
trap print_int         # 輸出 40
add , ,     # 設定輸出值
trap print_int         # 輸出 10
`

### 記憶體操作
`mips
# 載入資料地址
la , data

# 載入不同大小的資料
lb , 0()         # 載入位元組 (有符號)
trap print_int
lbu , 0()        # 載入位元組 (無符號)
trap print_int
lh , 0()         # 載入半字組
trap print_int
lw , 0()         # 載入字組
trap print_int

# 資料區段
data:
.word 255              # 字組資料
.word 65535            # 另一個字組
.byte 123              # 位元組資料
.byte 111              # 另一個位元組
`

### 控制流程
`mips
# 條件分支
addi , , 5
addi , , 5
beq , , equal    # 如果相等就跳轉
addi , , 99    # 這行不會執行
equal:
addi , , 42    #  = 42

# 函數呼叫
jal my_function        # 呼叫函數
trap exit              # 結束程式

my_function:
addi , , 100   # 設定回傳值
jr                  # 返回呼叫點
`

### 系統呼叫
`mips
# 輸出整數
addi , , 1     # 系統呼叫編號 1 (print_int)
addi , , 42    # 要輸出的數字
syscall                # 輸出 42

# 輸出字串
addi , , 4     # 系統呼叫編號 4 (print_string)
la , message        # 字串地址
syscall                # 輸出字串

# 結束程式
addi , , 10    # 系統呼叫編號 10 (exit)
syscall

message:
.asciiz "Hello, World!"
`

這個專案實現了一個完整的 MIPS 模擬器，支援從基本算術到複雜控制流程的所有核心指令，並提供完整的測試框架和除錯工具。
