.text
main:
    lhi $t0, 0
    llo $t0, 0
    blez $t0, zero_or_neg
    lhi $t1, 0
    llo $t1, 999
    or $a0, $t1, $zero
    trap print_int
    j end_test

zero_or_neg:
    lhi $t2, 0
    llo $t2, 100
    or $a0, $t2, $zero
    trap print_int

end_test:
    trap exit
