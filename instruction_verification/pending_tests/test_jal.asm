.text
main:
    jal function_test
    or $a0, $v0, $zero
    trap print_int
    trap exit

function_test:
    lhi $v0, 0
    llo $v0, 100
    jr $ra
