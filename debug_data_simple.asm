# Simple test to debug data loading
.text
main:
llo $a0, 42
trap print_int
trap exit

.data
test_word:
.word 12345
