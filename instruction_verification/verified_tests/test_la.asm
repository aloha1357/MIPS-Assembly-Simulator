.data
test_data: .word 42

.text
main:
    la $t0, test_data
    lw $t1, 0($t0)
    or $a0, $t1, $zero
    trap print_int
    trap exit
