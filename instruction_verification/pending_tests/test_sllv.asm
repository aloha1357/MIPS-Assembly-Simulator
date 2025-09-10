.text
main:
    lhi $t0, 0
    llo $t0, 5
    lhi $t1, 0
    llo $t1, 2
    sllv $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
