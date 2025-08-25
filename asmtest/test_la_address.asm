llo $a0, 1   ; 指令 0
trap print_int  ; 指令 1
llo $a0, 2   ; 指令 2
trap print_int  ; 指令 3
la $a0, label1  ; 指令 4 - 看看這裡返回什麼地址
trap print_int  ; 指令 5 - 打印 la 返回的地址
label1:         ; 指令 6
llo $a0, 777  ; 指令 6
trap print_int  ; 指令 7
trap exit     ; 指令 8
