addi $t0, $zero, 5      # $t0 = 5
addi $t1, $zero, 5      # $t1 = 5
beq $t0, $t1, equal     # Branch if equal
addi $v0, $zero, 1      # Should be skipped
equal:
addi $v0, $zero, 42     # $v0 = 42
j end                   # Jump to end
addi $v0, $zero, 99     # Should be skipped
end:
addi $v1, $zero, 7      # $v1 = 7
