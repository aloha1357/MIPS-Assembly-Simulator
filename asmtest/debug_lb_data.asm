# Test lb instruction with data directives
la $a1, mybyte
lb $a0, 0($a1)       # Load byte from data
trap print_int
trap exit

mybyte:
.byte 99
