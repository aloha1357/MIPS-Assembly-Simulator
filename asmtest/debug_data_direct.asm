# Test data directive processing
.text
main:
llo $a0, 99  # Test number
trap print_int

# Try manual address calculation
# If program has 3 instructions, data should start at address 12 (3*4)
llo $a0, 12  # Try address 12
lb $a1, 0($a0)   # Load byte (not word) to avoid alignment issues
or $a0, $a1, $zero
trap print_int   # Should print ASCII value of 'T' (84) if data is there

trap exit

.data
test_string:
.asciiz "Test"
