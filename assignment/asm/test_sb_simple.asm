la $a1, data
llo $a0, 259
sb $a0, 8 ($a1)
lbu $a0, 8 ($a1)
trap print_int
trap exit
data:
.word 255
.word 65535
.byte 123
.byte 111
