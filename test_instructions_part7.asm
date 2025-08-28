# Test Part 7: Function calls and string operations (most likely problem)
jal label7
j end
label7:
addi $a0, $ra, 0
trap print_int
la $a1, strings
lbu $a0, 0 ($a1)
trap print_character
la $a0, strings
trap print_string
llo $a0, 10
trap print_character
jr $ra
end:
trap exit
strings:
.asciiz "Hello, World!"
