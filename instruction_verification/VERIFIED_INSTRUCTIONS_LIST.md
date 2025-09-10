# 已驗證支援的 MIPS 指令清單

## 算術運算指令 (3個)

### ADD - 加法運算
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 5
    lhi $t1, 0
    llo $t1, 10
    add $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
```

### SUB - 減法運算
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 10
    lhi $t1, 0
    llo $t1, 5
    sub $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
```

### ADDI - 立即數加法
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 10
    addi $t1, $t0, 3
    or $a0, $t1, $zero
    trap print_int
    trap exit
```

## 邏輯運算指令 (3個)

### AND - 位元與運算
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 12
    lhi $t1, 0
    llo $t1, 10
    and $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
```

### OR - 位元或運算
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 8
    lhi $t1, 0
    llo $t1, 4
    or $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
```

### XOR - 位元異或運算
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 15
    lhi $t1, 0
    llo $t1, 7
    xor $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
```

## 移位指令 (2個)

### SLL - 邏輯左移
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 5
    sll $t1, $t0, 2
    or $a0, $t1, $zero
    trap print_int
    trap exit
```

### SRL - 邏輯右移
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 20
    srl $t1, $t0, 2
    or $a0, $t1, $zero
    trap print_int
    trap exit
```

## 記憶體操作指令 (3個)

### LA - 載入位址
```assembly
.data
test_data: .word 42

.text
main:
    la $t0, test_data
    lw $t1, 0($t0)
    or $a0, $t1, $zero
    trap print_int
    trap exit
```

### LW - 載入字組
```assembly
.data
value: .word 100

.text
main:
    la $t0, value
    lw $t1, 0($t0)
    or $a0, $t1, $zero
    trap print_int
    trap exit
```

### SW - 儲存字組
```assembly
.data
storage: .word 0

.text
main:
    lhi $t0, 0
    llo $t0, 200
    la $t1, storage
    sw $t0, 0($t1)
    lw $t2, 0($t1)
    or $a0, $t2, $zero
    trap print_int
    trap exit
```

## 分支跳躍指令 (3個)

### BEQ - 相等時分支
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 5
    lhi $t1, 0
    llo $t1, 5
    beq $t0, $t1, equal_label
    or $a0, $zero, $zero
    trap print_int
    j end_label
equal_label:
    lhi $t2, 0
    llo $t2, 99
    or $a0, $t2, $zero
    trap print_int
end_label:
    trap exit
```

### BNE - 不相等時分支
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 5
    lhi $t1, 0
    llo $t1, 3
    bne $t0, $t1, not_equal_label
    or $a0, $zero, $zero
    trap print_int
    j end_label
not_equal_label:
    lhi $t2, 0
    llo $t2, 88
    or $a0, $t2, $zero
    trap print_int
end_label:
    trap exit
```

### J - 無條件跳躍
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 77
    or $a0, $t0, $zero
    trap print_int
    j skip_section
    lhi $t1, 0
    llo $t1, 999
    or $a0, $t1, $zero
    trap print_int
skip_section:
    trap exit
```

## 特殊載入指令 (2個)

### LHI - 載入高位立即數
```assembly
.text
main:
    lhi $t0, 1
    llo $t0, 0
    or $a0, $t0, $zero
    trap print_int
    trap exit
```

### LLO - 載入低位立即數
```assembly
.text
main:
    lhi $t0, 0
    lhi $t0, 0
    llo $t0, 255
    or $a0, $t0, $zero
    trap print_int
    trap exit
```

## 系統指令 (2個)

### TRAP - 陷阱指令
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 42
    or $a0, $t0, $zero
    trap print_int
    trap exit
```

### SYSCALL - 系統呼叫
```assembly
.text
main:
    lhi $t0, 0
    llo $t0, 123
    or $a0, $t0, $zero
    lhi $v0, 0
    llo $v0, 1
    syscall
    lhi $v0, 0
    llo $v0, 10
    syscall
```

## 總計: 18個已驗證指令
