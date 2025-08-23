# test_simple_finite.asm - finite program without labels for CLI testing

addi $t0, $zero, 3      # counter = 3
addi $t0, $t0, -1       # counter = 2
addi $t0, $t0, -1       # counter = 1
addi $t0, $t0, -1       # counter = 0

# Exit properly with syscall 10
addi $v0, $zero, 10     # exit syscall
syscall
