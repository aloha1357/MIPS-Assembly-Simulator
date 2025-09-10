.data
hello_msg: .asciiz "Hello"

.text
main:
    la $a0, hello_msg
    lhi $v0, 0
    llo $v0, 4
    syscall
    lhi $v0, 0
    llo $v0, 10
    syscall
