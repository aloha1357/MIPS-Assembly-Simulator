.text
main:
    lhi $t0, 0
    llo $t0, 10
    lhi $t1, 0
    llo $t1, 5
    
    add $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    
    sub $t3, $t0, $t1
    or $a0, $t3, $zero
    trap print_int
    
    addi $t4, $t0, 3
    or $a0, $t4, $zero
    trap print_int
    
    and $t5, $t0, $t1
    or $a0, $t5, $zero
    trap print_int
    
    or $t6, $t0, $t1
    or $a0, $t6, $zero
    trap print_int
    
    xor $t7, $t0, $t1
    or $a0, $t7, $zero
    trap print_int
    
    sll $s0, $t1, 2
    or $a0, $s0, $zero
    trap print_int
    
    srl $s1, $t0, 1
    or $a0, $s1, $zero
    trap print_int
    
    trap exit
