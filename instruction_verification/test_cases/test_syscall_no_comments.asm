.text
main:
    lhi $t0, 0
    llo $t0, 25
    or $a0, $t0, $zero
    lhi $v0, 0
    llo $v0, 1
    syscall
    
    lhi $t1, 0
    llo $t1, 35
    or $a0, $t1, $zero
    lhi $v0, 0
    llo $v0, 1
    syscall
    
    lhi $v0, 0
    llo $v0, 10
    syscall
