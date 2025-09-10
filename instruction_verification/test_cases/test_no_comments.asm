.text
main:
    lhi $t0, 0
    llo $t0, 5
    lhi $t1, 0  
    llo $t1, 10
    add $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    
    lhi $t0, 0
    llo $t0, 100
    lhi $t1, 0
    llo $t1, 200
    add $t3, $t0, $t1
    or $a0, $t3, $zero
    trap print_int
    
    lhi $t0, 65535
    llo $t0, 65535
    lhi $t1, 0
    llo $t1, 1
    add $t4, $t0, $t1
    or $a0, $t4, $zero
    trap print_int
    
    lhi $t0, 32767
    llo $t0, 65535
    lhi $t1, 0
    llo $t1, 1
    add $t5, $t0, $t1
    or $a0, $t5, $zero
    trap print_int
    
    trap exit
