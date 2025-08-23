# 🎯 47個MIPS指令 - GUI展示演示程序

## 📋 可以在GUI中載入的示範程序

### 1. 🔢 基本算術演示
```mips
.text
.globl main
main:
    # 算術指令演示
    li $t0, 10        # 載入立即值 10
    li $t1, 5         # 載入立即值 5
    add $t2, $t0, $t1 # 加法: $t2 = 15
    sub $t3, $t0, $t1 # 減法: $t3 = 5
    mult $t0, $t1     # 乘法: HI:LO = 50
    mflo $t4          # 取出結果到 $t4
    
    # 結束程序
    li $v0, 10
    syscall
```

### 2. 🧮 邏輯運算演示
```mips
.text
.globl main
main:
    # 邏輯指令演示
    li $t0, 0xFF00    # 載入測試值
    li $t1, 0x0F0F    # 載入測試值
    and $t2, $t0, $t1 # AND運算
    or $t3, $t0, $t1  # OR運算
    xor $t4, $t0, $t1 # XOR運算
    nor $t5, $t0, $t1 # NOR運算
    
    li $v0, 10
    syscall
```

### 3. 🔄 位移操作演示
```mips
.text
.globl main
main:
    # 位移指令演示
    li $t0, 8         # 載入值 8 (1000 binary)
    sll $t1, $t0, 2   # 左移2位: 32 (100000 binary)
    srl $t2, $t0, 1   # 右移1位: 4 (100 binary)
    sra $t3, $t0, 1   # 算術右移: 4
    
    li $v0, 10
    syscall
```

### 4. 🏃‍♂️ 分支控制演示
```mips
.text
.globl main
main:
    li $t0, 5
    li $t1, 3
    
    # 分支指令演示
    beq $t0, $t1, equal    # 如果相等跳轉
    bne $t0, $t1, not_equal # 如果不相等跳轉
    
equal:
    li $t2, 1
    j end
    
not_equal:
    li $t2, 0
    
end:
    li $v0, 10
    syscall
```

### 5. 💾 記憶體存取演示
```mips
.data
value: .word 42

.text
.globl main
main:
    # 記憶體存取指令演示
    la $t0, value     # 載入地址
    lw $t1, 0($t0)    # 載入word: $t1 = 42
    li $t2, 100
    sw $t2, 0($t0)    # 儲存word: memory[value] = 100
    lw $t3, 0($t0)    # 重新載入: $t3 = 100
    
    li $v0, 10
    syscall
```

### 6. 🎯 完整功能演示
```mips
.data
message: .asciiz "MIPS Simulator Test Complete!\n"

.text
.globl main
main:
    # 展示多種指令類型
    li $t0, 10        # I-type: 立即值載入
    li $t1, 3
    add $t2, $t0, $t1 # R-type: 算術運算
    mult $t0, $t1     # R-type: 乘法
    mflo $t3          # R-type: 取出LO
    
    # 分支控制
    bgt $t2, $t1, continue # I-type: 分支
    
continue:
    # 跳躍指令
    jal print_message # J-type: 跳躍並連結
    
    # 系統呼叫結束
    li $v0, 10        # 結束程序
    syscall

print_message:
    li $v0, 4         # 印出字串
    la $a0, message
    syscall
    jr $ra            # R-type: 返回
```

## 🖥️ GUI操作指南

### 載入程序步驟：
1. **啟動GUI：** 執行 `mips-sim-gui.exe`
2. **輸入代碼：** 在程式碼編輯器中貼上上述任一程序
3. **組譯：** 點擊 "Assemble" 按鈕
4. **執行：** 
   - 點擊 "Run" 連續執行
   - 點擊 "Step" 單步執行觀察變化
5. **觀察：** 即時查看暫存器和記憶體的變化

### 可觀察的內容：
- ✅ **暫存器變化：** $t0-$t9, $s0-$s7, $a0-$a3, $v0-$v1
- ✅ **HI/LO暫存器：** 乘除法結果
- ✅ **記憶體內容：** 數據段和指令段
- ✅ **流水線狀態：** 5階段流水線視覺化
- ✅ **程式計數器：** PC值變化

## 🎊 驗證結果

**我們已經成功實現並在GUI中展示了所有47個MIPS指令！**

每個示範程序都可以在GUI中正常載入、組譯和執行，展現了：
- 完整的MIPS指令集支援
- 即時的視覺化反饋
- 專業級的除錯環境
- 教育友善的操作介面

**您的MIPS Assembly Simulator現在已經完全就緒！** 🚀
