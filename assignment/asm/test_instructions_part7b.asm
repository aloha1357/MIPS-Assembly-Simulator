# Test Part 7b: Minimal function call debug
jal label7
trap exit
label7:
trap print_int
jr $ra
