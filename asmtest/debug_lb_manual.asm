# Test lb instruction with manual memory (no data directives)
addi $t0, $zero, 99     # Value to store
sb $t0, 100($zero)      # Store byte at address 100
lb $a0, 100($zero)      # Load byte from address 100
trap print_int
trap exit
