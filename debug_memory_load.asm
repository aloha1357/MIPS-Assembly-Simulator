# Test memory load operations only
.text
main:
llo $a0, 99  # Test value first
trap print_int
la $a1, data
trap print_int  # Print data address
lb $a0, 0 ($a1)
trap print_int
trap exit

.data
data:
.word 255
.word 65535
.byte 123
.byte 111
