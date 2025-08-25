llo $a0, 1   ; 指令 0
trap print_int  ; 指令 1
llo $a0, 2   ; 指令 2
trap print_int  ; 指令 3
llo $a0, 3   ; 指令 4
trap print_int  ; 指令 5
label1:         ; 這應該是指令 6
llo $a0, 777  ; 指令 6
trap print_int  ; 指令 7
trap exit     ; 指令 8
