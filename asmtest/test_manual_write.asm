lhi $a0, 0
llo $a0, 20
lhi $a1, 0  
llo $a1, 99999
sw $a1, 0 ($a0)
trap exit
data:
.word 12345
