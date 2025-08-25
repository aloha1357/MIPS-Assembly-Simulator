# Test Segment 1: Basic load high/low instructions
lhi $a0, 1
llo $a0, 0
trap print_int
trap exit
