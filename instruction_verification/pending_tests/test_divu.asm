.text
main:
    lhi $t0, 0
    llo $t0, 42
    lhi $t1, 0
    llo $t1, 6
    divu $t0, $t1
    mflo $t2
    or $a0, $t2, $zero
    trap print_int
    trap exit
