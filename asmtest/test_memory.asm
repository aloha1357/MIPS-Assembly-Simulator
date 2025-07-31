addi $s0, $zero, 0x1000    # Set base address to 0x1000
addi $t0, $zero, 42         # Load value 42 into $t0  
sw $t0, 0($s0)              # Store $t0 to memory[0x1000]
lw $t1, 0($s0)              # Load from memory[0x1000] to $t1
addi $t2, $t1, 100          # Add 100 to loaded value
