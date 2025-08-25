# Test print_string
.text
main:
llo $a0, 99
trap print_int
la $a0, test_string
trap print_string
trap exit

.data
test_string:
.asciiz "Hello"
