# Simple finite loop for testing - loops 5 times then exits
# Using only basic instructions
addi $t0, $zero, 5     # counter = 5 (count down)

loop:
    addi $t0, $t0, -1      # counter--
    bne $t0, $zero, loop   # if counter != 0, continue loop
    
# Exit properly with syscall 10
addi $v0, $zero, 10    # exit syscall
syscall
