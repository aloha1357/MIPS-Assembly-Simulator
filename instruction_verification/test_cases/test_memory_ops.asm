.data
test_value: .word 42

.text
main:
    la $t0, test_value
    
    lw $t1, 0($t0)
    or $a0, $t1, $zero
    trap print_int
    
    lhi $t2, 0
    llo $t2, 77
    sw $t2, 0($t0)
    
    lw $t3, 0($t0)
    or $a0, $t3, $zero
    trap print_int
    
    lhi $t4, 0
    llo $t4, 200
    addi $t5, $t0, 4
    sw $t4, 0($t5)
    
    lw $t6, 4($t0)
    or $a0, $t6, $zero
    trap print_int
    
    trap exit
