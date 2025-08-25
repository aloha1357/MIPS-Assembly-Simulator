# Test step by step
.text
main:
llo $a0, 99
trap print_int
llo $a1, 32
trap print_int
llo $a0, 77  # Another test instruction before lb
trap print_int
trap exit
