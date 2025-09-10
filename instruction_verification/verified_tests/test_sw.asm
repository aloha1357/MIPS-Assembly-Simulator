.data
storage: .word 0

.text
main:
    lhi $t0, 0
    llo $t0, 200
    la $t1, storage
    sw $t0, 0($t1)
    lw $t2, 0($t1)
    or $a0, $t2, $zero
    trap print_int
    trap exit
