# MIPS Logical Instructions Demo Program
# This program demonstrates all four logical instructions functionality

.data
    msg_and:    .asciiz "AND result: "
    msg_or:     .asciiz "OR result: "
    msg_xor:    .asciiz "XOR result: "
    msg_nor:    .asciiz "NOR result: "
    msg_encrypt:.asciiz "Encryption demo - Original: "
    msg_decrypt:.asciiz "Decrypted: "
    newline:    .asciiz "\n"

.text
main:
    # Setup test data
    addi $t0, $zero, 0xAAAA        # $t0 = 0x0000AAAA (alternating bits)
    addi $t1, $zero, 0x5555        # $t1 = 0x00005555 (complementary bits)
    
    # === AND Instruction Demo ===
    # Print label
    addi $v0, $zero, 4
    la $a0, msg_and
    syscall
    
    # Execute AND operation
    and $t2, $t0, $t1              # $t2 = 0x0000AAAA & 0x00005555 = 0x00000000
    
    # Print result
    addi $v0, $zero, 1
    add $a0, $zero, $t2
    syscall
    
    # Print newline
    addi $v0, $zero, 4
    la $a0, newline
    syscall
    
    # === OR Instruction Demo ===
    # Print label
    addi $v0, $zero, 4
    la $a0, msg_or
    syscall
    
    # Execute OR operation
    or $t3, $t0, $t1               # $t3 = 0x0000AAAA | 0x00005555 = 0x0000FFFF
    
    # Print result
    addi $v0, $zero, 1
    add $a0, $zero, $t3
    syscall
    
    # Print newline
    addi $v0, $zero, 4
    la $a0, newline
    syscall
    
    # === XOR Instruction Demo ===
    # Print label
    addi $v0, $zero, 4
    la $a0, msg_xor
    syscall
    
    # Execute XOR operation
    xor $t4, $t0, $t1              # $t4 = 0x0000AAAA ^ 0x00005555 = 0x0000FFFF
    
    # Print result
    addi $v0, $zero, 1
    add $a0, $zero, $t4
    syscall
    
    # Print newline
    addi $v0, $zero, 4
    la $a0, newline
    syscall
    
    # === NOR Instruction Demo ===
    # Print label
    addi $v0, $zero, 4
    la $a0, msg_nor
    syscall
    
    # Execute NOR operation
    nor $t5, $t0, $t1              # $t5 = ~(0x0000AAAA | 0x00005555) = ~0x0000FFFF = 0xFFFF0000
    
    # Print result
    addi $v0, $zero, 1
    add $a0, $zero, $t5
    syscall
    
    # Print newline
    addi $v0, $zero, 4
    la $a0, newline
    syscall
    
    # === XOR Encryption/Decryption Demo ===
    # Print encryption message
    addi $v0, $zero, 4
    la $a0, msg_encrypt
    syscall
    
    addi $t6, $zero, 0x1234        # Original data
    addi $t7, $zero, 0xABCD        # Encryption key
    
    # Print original data
    addi $v0, $zero, 1
    add $a0, $zero, $t6
    syscall
    
    # Print newline
    addi $v0, $zero, 4
    la $a0, newline
    syscall
    
    # Encrypt: data ^ key
    xor $t8, $t6, $t7              # Encrypted = 0x1234 ^ 0xABCD = 0xB9F9
    
    # Decrypt: encrypted ^ key = original
    xor $t9, $t8, $t7              # Decrypted = 0xB9F9 ^ 0xABCD = 0x1234
    
    # Print decryption message
    addi $v0, $zero, 4
    la $a0, msg_decrypt
    syscall
    
    # Print decrypted result (should equal original)
    addi $v0, $zero, 1
    add $a0, $zero, $t9
    syscall
    
    # Print newline
    addi $v0, $zero, 4
    la $a0, newline
    syscall
    
    # Program exit
    addi $v0, $zero, 10
    syscall

# Expected Output:
# AND result: 0
# OR result: 65535
# XOR result: 65535
# NOR result: -65536
# Encryption demo - Original: 4660
# Decrypted: 4660
