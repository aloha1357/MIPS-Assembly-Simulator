# Debug JAL/JR: print $ra and PC around call
# We'll use trap print_int where $a0 holds value to print
# Prepare: set $v0 = 1 (print_int), move $ra into $a0 via addi/add
# Use addu to copy ra into a0 is not possible directly; use addi with $ra as source is not supported
# So put $ra into $a0 via addu $a0, $ra, $zero (zero is register 0)
addi $v0, $zero, 1    # syscall print_int
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
