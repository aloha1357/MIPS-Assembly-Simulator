# Simple debug test for TDD approach - Stage 1: Basic LHI/LLO
lhi $a0, 1
llo $a0, 0
trap print_int
trap exit
