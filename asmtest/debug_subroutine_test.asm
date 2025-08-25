# Test with proper subroutine return
.text
main:
llo $a0, 42
trap print_int
jal subroutine
llo $a0, 99  
trap print_int
j end

subroutine:
la $a1, strings
lbu $a0, 0 ($a1)
trap print_character
la $a0, strings  
trap print_string
llo $a0, 10
trap print_character
jr $ra           # Proper return

end:
trap exit

.data
strings:
.asciiz "Hello, World!"
