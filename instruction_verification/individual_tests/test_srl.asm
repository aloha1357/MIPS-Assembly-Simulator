.text
main:
    lhi $t0, 0
    llo $t0, 20
    srl $t1, $t0, 2
    or $a0, $t1, $zero
    trap print_int
    trap exit
