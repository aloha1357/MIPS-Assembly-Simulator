# Test just the first few instructions
.text
main:
lhi $a0, 1
llo $a0, 0
trap print_int
trap exit
