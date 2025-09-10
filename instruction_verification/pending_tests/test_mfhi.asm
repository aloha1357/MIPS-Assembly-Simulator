.text
main:
    lhi $t0, 0
    llo $t0, 123
    mthi $t0
    mfhi $t1
    or $a0, $t1, $zero
    trap print_int
    trap exit
