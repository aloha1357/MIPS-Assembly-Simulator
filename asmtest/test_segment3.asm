# Test Segment 3: Load address instruction
la $a0, label1
trap print_int
jr $a0
trap exit
label1:
trap print_int
trap exit
