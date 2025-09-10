.text
main:
    lhi $t0, 0
    llo $t0, 15
    lhi $t1, 0
    llo $t1, 7
    xor $t2, $t0, $t1
    or $a0, $t2, $zero
    trap print_int
    trap exit
