# Test jalr
lhi $a0, 1
llo $a0, 0
trap print_int

la $a0, label1
jalr $a0

trap exit  # Should never reach here

label1:
trap print_int
addi $a0, $ra, 0
trap print_int
trap exit
