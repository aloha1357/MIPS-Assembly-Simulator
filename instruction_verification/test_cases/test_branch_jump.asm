.text
main:
    lhi $t0, 0
    llo $t0, 10
    lhi $t1, 0
    llo $t1, 10
    
    beq $t0, $t1, equal_branch
    or $a0, $zero, $zero
    trap print_int
    j end_test

equal_branch:
    lhi $t2, 0
    llo $t2, 99
    or $a0, $t2, $zero
    trap print_int

end_test:
    lhi $t3, 0
    llo $t3, 5
    lhi $t4, 0
    llo $t4, 3
    
    bne $t3, $t4, not_equal_branch
    or $a0, $zero, $zero
    trap print_int
    j final_end

not_equal_branch:
    lhi $t5, 0
    llo $t5, 88
    or $a0, $t5, $zero
    trap print_int

final_end:
    trap exit
