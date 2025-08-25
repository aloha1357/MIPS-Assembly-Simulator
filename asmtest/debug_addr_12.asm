# Test to check address 12
.text
main:
# Check what's at address 12
llo $a0, 12
lw $a1, 0($a0)  # Load word from address 12
or $a0, $a1, $zero  # Copy to $a0
trap print_int
trap exit

.data
test_word:
.word 12345
