# Test label mapping bug
llo $a0, 1           # PC=0
llo $a0, 2           # PC=1  
beq $a0, $a0, target # PC=2, should jump to PC=4
llo $a0, 3           # PC=3 (this should be skipped)
target:              # This should point to PC=4
llo $a0, 999         # PC=4 (this should execute)
trap print_int       # PC=5
trap exit            # PC=6
