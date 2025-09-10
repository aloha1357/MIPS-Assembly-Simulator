# 已驗證 MIPS 指令格式與使用說明

## 算術運算指令

### ADD - 加法運算
**格式**: `add $rd, $rs, $rt`
**功能**: rd = rs + rt
**範例**:
```assembly
lhi $t0, 0
llo $t0, 5
lhi $t1, 0
llo $t1, 10
add $t2, $t0, $t1    # $t2 = 5 + 10 = 15
```

### SUB - 減法運算
**格式**: `sub $rd, $rs, $rt`
**功能**: rd = rs - rt
**範例**:
```assembly
lhi $t0, 0
llo $t0, 10
lhi $t1, 0
llo $t1, 5
sub $t2, $t0, $t1    # $t2 = 10 - 5 = 5
```

### ADDI - 立即數加法
**格式**: `addi $rt, $rs, immediate`
**功能**: rt = rs + immediate
**範例**:
```assembly
lhi $t0, 0
llo $t0, 10
addi $t1, $t0, 3     # $t1 = 10 + 3 = 13
```

## 邏輯運算指令

### AND - 位元與運算
**格式**: `and $rd, $rs, $rt`
**功能**: rd = rs & rt
**範例**:
```assembly
lhi $t0, 0
llo $t0, 15          # 1111 (binary)
lhi $t1, 0
llo $t1, 7           # 0111 (binary)
and $t2, $t0, $t1    # $t2 = 7 (0111)
```

### OR - 位元或運算
**格式**: `or $rd, $rs, $rt`
**功能**: rd = rs | rt
**範例**:
```assembly
lhi $t0, 0
llo $t0, 8           # 1000 (binary)
lhi $t1, 0
llo $t1, 4           # 0100 (binary)
or $t2, $t0, $t1     # $t2 = 12 (1100)
```

### XOR - 位元異或運算
**格式**: `xor $rd, $rs, $rt`
**功能**: rd = rs ^ rt
**範例**:
```assembly
lhi $t0, 0
llo $t0, 15          # 1111 (binary)
lhi $t1, 0
llo $t1, 7           # 0111 (binary)
xor $t2, $t0, $t1    # $t2 = 8 (1000)
```

## 移位指令

### SLL - 邏輯左移
**格式**: `sll $rd, $rt, shamt`
**功能**: rd = rt << shamt
**範例**:
```assembly
lhi $t0, 0
llo $t0, 5
sll $t1, $t0, 2      # $t1 = 5 << 2 = 20
```

### SRL - 邏輯右移
**格式**: `srl $rd, $rt, shamt`
**功能**: rd = rt >> shamt (無符號)
**範例**:
```assembly
lhi $t0, 0
llo $t0, 20
srl $t1, $t0, 2      # $t1 = 20 >> 2 = 5
```

## 記憶體操作指令

### LW - 載入字組
**格式**: `lw $rt, offset($rs)`
**功能**: rt = Memory[rs + offset]
**範例**:
```assembly
.data
value: .word 42

.text
la $t0, value
lw $t1, 0($t0)       # $t1 = 42
```

### SW - 儲存字組
**格式**: `sw $rt, offset($rs)`
**功能**: Memory[rs + offset] = rt
**範例**:
```assembly
.data
storage: .word 0

.text
la $t0, storage
lhi $t1, 0
llo $t1, 100
sw $t1, 0($t0)       # Memory[storage] = 100
```

### LA - 載入位址
**格式**: `la $rt, label`
**功能**: rt = address of label
**範例**:
```assembly
.data
data_label: .word 42

.text
la $t0, data_label   # $t0 = address of data_label
```

## 分支跳躍指令

### BEQ - 相等時分支
**格式**: `beq $rs, $rt, label`
**功能**: if (rs == rt) goto label
**範例**:
```assembly
lhi $t0, 0
llo $t0, 5
lhi $t1, 0
llo $t1, 5
beq $t0, $t1, equal_label
```

### BNE - 不相等時分支
**格式**: `bne $rs, $rt, label`
**功能**: if (rs != rt) goto label
**範例**:
```assembly
lhi $t0, 0
llo $t0, 5
lhi $t1, 0
llo $t1, 3
bne $t0, $t1, not_equal_label
```

### J - 無條件跳躍
**格式**: `j label`
**功能**: goto label
**範例**:
```assembly
j end_program
```

## 特殊載入指令

### LHI - 載入高位立即數
**格式**: `lhi $rt, immediate`
**功能**: rt[31:16] = immediate, rt[15:0] = 0
**範例**:
```assembly
lhi $t0, 0x1234      # $t0 = 0x12340000
```

### LLO - 載入低位立即數
**格式**: `llo $rt, immediate`
**功能**: rt[15:0] = immediate, rt[31:16] 保持不變
**範例**:
```assembly
lhi $t0, 0x1234
llo $t0, 0x5678      # $t0 = 0x12345678
```

## 系統指令

### TRAP - 陷阱指令
**格式**: `trap function_name`
**支援功能**:
- `trap print_int` - 列印 $a0 的整數值
- `trap exit` - 結束程式

**範例**:
```assembly
lhi $t0, 0
llo $t0, 42
or $a0, $t0, $zero
trap print_int       # 輸出: 42
trap exit
```

### SYSCALL - 系統呼叫
**格式**: `syscall`
**使用方式**: 
- $v0 = 系統呼叫號碼
- $a0-$a3 = 參數

**支援的系統呼叫**:
- syscall 1: print_int ($a0 = 要列印的整數)
- syscall 10: exit (結束程式)

**範例**:
```assembly
lhi $t0, 0
llo $t0, 42
or $a0, $t0, $zero
lhi $v0, 0
llo $v0, 1
syscall              # 輸出: 42

lhi $v0, 0
llo $v0, 10
syscall              # 結束程式
```

## 注意事項

1. **註解限制**: 避免在 trap 指令後使用包含冒號(:)的複雜註解
2. **暫存器**: 使用標準 MIPS 暫存器名稱 ($t0-$t9, $s0-$s7, $a0-$a3, $v0-$v1 等)
3. **記憶體對齊**: 字組操作需要4位元組對齊
4. **立即數範圍**: LHI/LLO 的立即數範圍為 0-65535
