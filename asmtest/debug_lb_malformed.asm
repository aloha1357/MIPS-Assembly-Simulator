# Test lb instruction with malformed syntax (space in offset)
la $a1, mybyte
lb $a0, 0 ($a1)       # Note the space between 0 and ($a1) - this is malformed!
trap print_int
trap exit

mybyte:
.byte 99
