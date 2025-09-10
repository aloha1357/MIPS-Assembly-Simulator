.text
main:
    lui $t0, 0x1234
    or $a0, $t0, $zero
    trap print_int
    trap exit
