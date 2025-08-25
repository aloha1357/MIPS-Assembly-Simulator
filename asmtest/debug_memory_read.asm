# Test to examine memory contents
.text
main:
# Check what's at address 24
llo $a0, 24
lw $a1, 0($a0)  # Load word from address 24
or $a0, $a1, $zero  # Copy to $a0
trap print_int

# Check what's at address 28
llo $a0, 28  
lw $a1, 0($a0)  # Load word from address 28
or $a0, $a1, $zero  # Copy to $a0
trap print_int

trap exit

.data
hello_msg:
.asciiz "Hello"
