# Test branch instructions
llo $a0, 0
lhi $a0, 0
trap print_int

addi $a1, $a0, 0  # Copy a0 to a1
addi $a0, $a1, 0  # Copy a1 back to a0 (should still be 0)
trap print_int

beq $a0, $a1, label3   # Should branch since both are 0
trap print_int  # Should not execute
j end

label3:
trap print_int  # Should execute (print 0)
bne $a0, $a1, end  # Should not branch since they're equal
blez $a0, label4   # Should branch since a0 is 0 (≤ 0)
j end

label4:
llo $a1, 1  # Set a1 to 1
bgtz $a1, label5  # Should branch since 1 > 0
j end

label5:
addiu $a0, $a0, 1  # a0 becomes 1
trap print_int
j end

end:
trap exit
