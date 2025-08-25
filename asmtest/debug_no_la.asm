# Test string operations with hardcoded address
.text
main:
llo $a0, 42
trap print_int
# Try to print character without la instruction
llo $a0, 72  # ASCII 'H'
trap print_character
trap exit
