.text
main:
    lhi $t0, 0
    llo $t0, 77
    or $a0, $t0, $zero
    trap print_int
    j skip_section
    lhi $t1, 0
    llo $t1, 999
    or $a0, $t1, $zero
    trap print_int
skip_section:
    trap exit
