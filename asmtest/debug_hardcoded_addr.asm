# Test string operations with hardcoded address
.text
main:
llo $a0, 42
trap print_int
# Try with hardcoded address
llo $a0, 24  # Address from debug output
trap print_string
trap exit

.data
hello_msg:
.asciiz "Hello"
