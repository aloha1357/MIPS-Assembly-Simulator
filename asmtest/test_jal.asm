jal label7
j end
label7:
addi $a0, $ra, 0
trap print_int
llo $a0, 888
trap print_int
trap exit
end:
llo $a0, 999
trap print_int
trap exit
