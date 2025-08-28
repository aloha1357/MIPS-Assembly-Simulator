(# Debug JAL/JR: print $ra and PC around call
# We'll use trap print_int where $a0 holds value to print
addi $v0, $zero, 1
jal label_debug
# After return, print a marker
addi $a0, $zero, 999999
trap print_int
trap exit
label_debug:
# Inside function: move $ra to $a0 and print
addu $a0, $ra, $zero
trap print_int
jr $ra
)
