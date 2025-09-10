.data
test_data: .word 0x12345678

.text
main:
    la $t0, test_data
    lb $t1, 0($t0)
    or $a0, $t1, $zero
    trap print_int
    trap exit
