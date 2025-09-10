.data
value: .word 100

.text
main:
    la $t0, value
    lw $t1, 0($t0)
    or $a0, $t1, $zero
    trap print_int
    trap exit
