# Test Part 7a: Only function calls without string operations
jal label7
j end
label7:
addi $a0, $ra, 0
trap print_int
jr $ra
end:
trap exit
