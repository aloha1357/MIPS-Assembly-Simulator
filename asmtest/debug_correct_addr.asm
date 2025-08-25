# Test with correct address calculation
.text
main:
llo $a0, 99
trap print_int
llo $a0, 36  # Use the actual address from debug output
lb $a1, 0($a0)
or $a0, $a1, $zero
trap print_int   # Should print 84 (ASCII 'T')
trap exit

.data
test_string:
.asciiz "Test"
