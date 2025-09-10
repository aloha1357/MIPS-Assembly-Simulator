.text
main:
    lhi $t0, 0
    llo $t0, 456
    mtlo $t0
    mflo $t1
    or $a0, $t1, $zero
    trap print_int
    trap exit
