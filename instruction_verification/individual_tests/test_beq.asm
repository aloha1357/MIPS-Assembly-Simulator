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
