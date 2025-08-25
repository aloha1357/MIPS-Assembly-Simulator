# Test simple jumps
lhi $a0, 1
llo $a0, 0
trap print_int

# Simple jump
la $a0, label1
jr $a0

trap exit  # Should never reach here

label1:
trap print_int
trap exit
