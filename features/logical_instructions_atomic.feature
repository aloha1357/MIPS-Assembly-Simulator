# language: en

Feature: Atomic Logical Instructions Implementation
  As a MIPS programmer
  I want to perform bitwise logical operations
  So that I can efficiently manipulate data at the bit level

  Background:
    Given I have a MIPS CPU simulator
    And the CPU is in a clean state with all registers set to zero
    And the program counter is at address 0

  # ====================================
  # Scenario 1: AND 邏輯指令測試
  # ====================================

  場景大綱: AND 指令執行位元 AND 運算
    假設 暫存器 <source1> 包含 <value1>
    並且 暫存器 <source2> 包含 <value2>
    當 我執行指令 "and <dest>, <source1>, <source2>"
    那麼 暫存器 <dest> 應該包含 <expected>
    並且 暫存器 <source1> 應該仍然包含 <value1>
    並且 暫存器 <source2> 應該仍然包含 <value2>
    並且 指令應該在恰好 1 個 CPU 週期內完成

    例子:
      | source1 | source2 | dest | value1     | value2     | expected   | 測試目的                |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000 | 互補位元模式 AND        |
      | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0x00000000 | 交替位元模式 AND        |
      | $t3     | $t4     | $t5  | 0xFFFFFFFF | 0xFFFFFFFF | 0xFFFFFFFF | 全 1 AND                |
      | $a0     | $a1     | $v0  | 0x12345678 | 0xFFFFF000 | 0x12345000 | 實用遮罩運算            |
      | $t6     | $t7     | $t8  | 0x00000000 | 0xFFFFFFFF | 0x00000000 | 零值 AND                |
      | $t0     | $t0     | $t0  | 0xDEADBEEF | 0xDEADBEEF | 0xDEADBEEF | 暫存器別名 (自我 AND)   |

  @ignore
  場景: AND 指令異常狀況測試
    假設 暫存器 $t0 包含 0x80000000
    並且 暫存器 $t1 包含 0x7FFFFFFF
    當 我執行指令 "and $t2, $t0, $t1"
    那麼 暫存器 $t2 應該包含 0x00000000
    並且 不應該產生任何異常
    並且 不應該影響狀態暫存器

  # ====================================
  # Scenario 2: OR 邏輯指令測試
  # ====================================

  @ignore
  場景大綱: OR 指令執行位元 OR 運算
    假設 暫存器 <source1> 包含 <value1>
    並且 暫存器 <source2> 包含 <value2>
    當 我執行指令 "or <dest>, <source1>, <source2>"
    那麼 暫存器 <dest> 應該包含 <expected>
    並且 源暫存器保持不變

    例子:
      | source1 | source2 | dest | value1     | value2     | expected   | 測試目的                |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0xFFFFFFFF | 互補位元模式 OR         |
      | $s0     | $s1     | $s2  | 0xAAAAAAAA | 0x55555555 | 0xFFFFFFFF | 交替位元模式 OR         |
      | $t3     | $t4     | $t5  | 0x00000000 | 0x00000000 | 0x00000000 | 全零 OR                 |
      | $a0     | $a1     | $v0  | 0x12340000 | 0x00005678 | 0x12345678 | 位元合併運算            |
      | $t6     | $t7     | $t8  | 0xDEADBEEF | 0x00000000 | 0xDEADBEEF | 與零 OR (恆等運算)      |

  @ignore
  場景: OR 指令效能要求驗證
    假設 我準備了 1000 個 OR 指令
    當 我執行所有 1000 個指令
    那麼 總執行時間應該少於 10 微秒
    並且 每個指令平均執行時間應該少於 10 奈秒

  # ====================================
  # Scenario 3: XOR 邏輯指令測試
  # ====================================

  @ignore
  場景大綱: XOR 指令執行位元 XOR 運算
    假設 暫存器 <source1> 包含 <value1>
    並且 暫存器 <source2> 包含 <value2>
    當 我執行指令 "xor <dest>, <source1>, <source2>"
    那麼 暫存器 <dest> 應該包含 <expected>

    例子:
      | source1 | source2 | dest | value1     | value2     | expected   | 測試目的                |
      | $t0     | $t1     | $t2  | 0xAAAAAAAA | 0x55555555 | 0xFFFFFFFF | 交替位元 XOR            |
      | $s0     | $s1     | $s2  | 0x12345678 | 0x12345678 | 0x00000000 | 自我 XOR (歸零)         |
      | $t3     | $t4     | $t5  | 0xDEADBEEF | 0x00000000 | 0xDEADBEEF | 與零 XOR (恆等)         |
      | $a0     | $a1     | $v0  | 0xFFFFFFFF | 0xAAAAAAAA | 0x55555555 | 位元翻轉運算            |
      | $t6     | $t7     | $t8  | 0x0F0F0F0F | 0xF0F0F0F0 | 0xFFFFFFFF | 完全互補 XOR            |

  場景: XOR 指令加密應用測試
    假設 暫存器 $t0 包含 0x12345678  # 原始資料
    並且 暫存器 $t1 包含 0xABCDEF00  # 金鑰
    當 我執行指令 "xor $t2, $t0, $t1"  # 加密
    並且 我執行指令 "xor $t3, $t2, $t1"  # 解密
    那麼 暫存器 $t3 應該包含 0x12345678  # 復原原始資料
    並且 這應該展示 XOR 的可逆性

  # ====================================
  # Scenario 4: NOR 邏輯指令測試
  # ====================================

  場景大綱: NOR 指令執行位元 NOR 運算
    假設 暫存器 <source1> 包含 <value1>
    並且 暫存器 <source2> 包含 <value2>
    當 我執行指令 "nor <dest>, <source1>, <source2>"
    那麼 暫存器 <dest> 應該包含 <expected>

    例子:
      | source1 | source2 | dest | value1     | value2     | expected   | 測試目的                |
      | $t0     | $t1     | $t2  | 0xF0F0F0F0 | 0x0F0F0F0F | 0x00000000 | 互補模式 NOR            |
      | $s0     | $s1     | $s2  | 0x00000000 | 0x00000000 | 0xFFFFFFFF | 全零 NOR (NOT 功能)     |
      | $t3     | $t4     | $t5  | 0xAAAAAAAA | 0x00000000 | 0x55555555 | 作為 NOT 運算使用       |
      | $a0     | $a1     | $v0  | 0xFFFFFFFF | 0xFFFFFFFF | 0x00000000 | 全一 NOR                |

  場景: NOR 指令作為通用 NOT 運算
    假設 暫存器 $t0 包含 0xDEADBEEF
    並且 暫存器 $zero 包含 0x00000000
    當 我執行指令 "nor $t1, $t0, $zero"
    那麼 暫存器 $t1 應該包含 0x21524110  # ~0xDEADBEEF
    並且 這應該等同於 NOT 運算

  # ====================================
  # Scenario 5: 複合邏輯運算測試
  # ====================================

  場景: 複雜邏輯運算序列
    假設 暫存器 $t0 包含 0x12345678
    並且 暫存器 $t1 包含 0x87654321
    當 我執行以下指令序列:
      """
      and $t2, $t0, $t1     # AND 運算
      or  $t3, $t0, $t1     # OR 運算
      xor $t4, $t0, $t1     # XOR 運算
      nor $t5, $t0, $t1     # NOR 運算
      """
    那麼 暫存器 $t2 應該包含 0x02244220  # 12345678 & 87654321
    並且 暫存器 $t3 應該包含 0x97755779  # 12345678 | 87654321
    並且 暫存器 $t4 應該包含 0x95511559  # 12345678 ^ 87654321
    並且 暫存器 $t5 應該包含 0x688AA886  # ~(12345678 | 87654321)

  場景: 邏輯運算與現有指令整合測試
    假設 我有一個混合指令程式:
      """
      addi $t0, $zero, 0x1234
      sll  $t0, $t0, 16        # $t0 = 0x12340000
      addi $t1, $zero, 0x5678
      or   $t2, $t0, $t1       # $t2 = 0x12345678
      and  $t3, $t2, $t0       # $t3 = 0x12340000
      sw   $t2, 0x1000($zero)  # 儲存到記憶體
      lw   $t4, 0x1000($zero)  # 從記憶體載入
      """
    當 我執行這個程式
    那麼 暫存器 $t2 應該等於 0x12345678
    並且 暫存器 $t3 應該等於 0x12340000
    並且 暫存器 $t4 應該等於 0x12345678
    並且 記憶體位址 0x1000 應該包含 0x12345678

  # ====================================
  # Scenario 6: 邊界條件與錯誤處理
  # ====================================

  場景: 邏輯指令邊界值測試
    假設 暫存器 $t0 包含 0x80000000  # 最大負數
    並且 暫存器 $t1 包含 0x7FFFFFFF  # 最大正數
    當 我執行所有邏輯指令:
      """
      and $s0, $t0, $t1
      or  $s1, $t0, $t1
      xor $s2, $t0, $t1
      nor $s3, $t0, $t1
      """
    那麼 暫存器 $s0 應該包含 0x00000000
    並且 暫存器 $s1 應該包含 0xFFFFFFFF
    並且 暫存器 $s2 應該包含 0xFFFFFFFF
    並且 暫存器 $s3 應該包含 0x00000000
    並且 所有運算都不應該產生溢位或異常

  場景: $zero 暫存器保護測試
    假設 暫存器 $t0 包含 0xFFFFFFFF
    當 我嘗試執行 "or $zero, $t0, $t0"
    那麼 暫存器 $zero 應該仍然包含 0x00000000
    並且 系統應該正確維護 $zero 暫存器的不變性

  # ====================================
  # Scenario 7: 效能與壓力測試
  # ====================================

  場景: 邏輯指令效能基準測試
    假設 我準備了 10000 個隨機邏輯指令
    當 我測量執行時間
    那麼 總執行時間應該少於 100 微秒
    並且 平均每個指令執行時間應該少於 10 奈秒
    並且 記憶體使用量不應該增加

  場景: 邏輯指令連續執行穩定性測試
    假設 我設定一個迴圈執行 1000000 次邏輯運算
    當 我執行完整迴圈
    那麼 所有結果應該保持一致
    並且 不應該出現記憶體洩漏
    並且 CPU 狀態應該穩定
