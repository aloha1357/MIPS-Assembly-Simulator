.text
main:
    lhi $t0, 0
    llo $t0, 200
    or $a0, $t0, $zero
    trap print_int
    la $t1, target_address
    jr $t1
    lhi $t2, 0
    llo $t2, 999
    or $a0, $t2, $zero
    trap print_int

target_address:
    trap exit
