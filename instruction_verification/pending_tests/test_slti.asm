.text
main:
    lhi $t0, 0
    llo $t0, 10
    slti $t1, $t0, 15
    or $a0, $t1, $zero
    trap print_int
    trap exit
