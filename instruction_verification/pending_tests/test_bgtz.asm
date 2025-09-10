.text
main:
    lhi $t0, 0
    llo $t0, 5
    bgtz $t0, positive
    lhi $t1, 0
    llo $t1, 999
    or $a0, $t1, $zero
    trap print_int
    j end_test

positive:
    lhi $t2, 0
    llo $t2, 200
    or $a0, $t2, $zero
    trap print_int

end_test:
    trap exit
