# Test la instruction  
.text
main:
llo $a0, 99
trap print_int
la $a0, test_string  # Load address of test_string
lb $a1, 0($a0)       # Load first character
or $a0, $a1, $zero   # Move to $a0
trap print_int       # Should print 84 (ASCII 'T')
trap exit

.data
test_string:
.asciiz "Test"
