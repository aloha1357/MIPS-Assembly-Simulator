.text
main:
    lhi $t0, 0
    llo $t0, 8
    ori $t1, $t0, 4
    or $a0, $t1, $zero
    trap print_int
    trap exit
