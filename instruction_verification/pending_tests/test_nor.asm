.text
main:
    lhi $t0, 0
    llo $t0, 12
    lhi $t1, 0
    llo $t1, 10
    nor $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
