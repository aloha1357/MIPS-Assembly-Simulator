la $a1, data
lb $a0, 0 ($a1)
trap print_int
lbu $a0, 0 ($a1)
trap print_int
lh $a0, 0 ($a1)
trap print_int
lhu $a0, 0 ($a1)
trap print_int
lh $a0, 4 ($a1)
trap print_int
lw $a0, 0 ($a1)
trap print_int
trap exit
data:
.word 255
.word 65535
.byte 123
.byte 111
