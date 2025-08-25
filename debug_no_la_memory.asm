# Test without la instruction
.text
main:
llo $a0, 99
trap print_int
llo $a1, 32  # Hardcode address instead of using la
trap print_int
lb $a0, 0 ($a1)
trap print_int
trap exit

.data
data:
.word 255
