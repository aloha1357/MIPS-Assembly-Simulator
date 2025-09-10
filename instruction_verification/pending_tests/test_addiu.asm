.text
main:
    lhi $t0, 0
    llo $t0, 10
    addiu $t1, $t0, 5
    or $a0, $t1, $zero
    trap print_int
    trap exit
