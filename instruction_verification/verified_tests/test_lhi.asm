.text
main:
    lhi $t0, 1
    llo $t0, 0
    or $a0, $t0, $zero
    trap print_int
    trap exit
