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
