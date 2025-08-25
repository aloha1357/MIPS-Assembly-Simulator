# Test just before lb instruction
.text
main:
llo $a0, 99
trap print_int
llo $a1, 32
trap print_int
llo $a0, 88  # Marker before lb
trap print_int
lb $a0, 0 ($a1)
llo $a0, 66  # Marker after lb (should not be reached if lb hangs)
trap print_int
trap exit

.data
data:
.word 255
