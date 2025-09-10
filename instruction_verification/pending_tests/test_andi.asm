.text
main:
    lhi $t0, 0
    llo $t0, 12
    andi $t1, $t0, 10
    or $a0, $t1, $zero
    trap print_int
    trap exit
