# Simple string test
.text
main:
# Test print_character
llo $a0, 72   # ASCII 'H'
trap print_character

# Test print_string
la $a0, hello_msg
trap print_string

# Test another character
llo $a0, 10   # ASCII newline
trap print_character

trap exit

.data
hello_msg:
.asciiz "Hello, World!"
