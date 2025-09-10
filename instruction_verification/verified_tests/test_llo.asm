.text
main:
    lhi $t0, 0
    llo $t0, 255
    or $a0, $t0, $zero
    trap print_int
    trap exit
