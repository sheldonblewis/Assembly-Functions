print:
    sw $1, -4($30)
    sw $2, -8($30)
    sw $3, -12($30)
    sw $4, -16($30)
    sw $5, -20($30)
    sw $6, -24($30)
    lis $2
    .word 0xffff000c
    bne $1, $0, 4
    lis $4
    .word 0x30
    sw $4, 0($2)
    beq $0, $0, 35
    slt $3, $1, $0
    beq $3, $0, 4
    lis $4
    .word 0x2D
    sw $4, 0($2)
    sub $1, $0, $1
    lis $5
    .word 10
    lis $4
    .word 4
    lis $6
    .word 0x1000
    sw $30, 0($6)
    div $1, $5
    mfhi $3
    mflo $1
    sw $3, -28($30)
    sub $30, $30, $4
    bne $1, $0, -6
    lis $4
    .word 0x2000
    sw $30, 0($4)
    lw $30, 0($6)
    lis $5
    .word 32
    lis $1
    .word 48
    lw $4, 0($4)
    lis $6
    .word 4
    lw $3, -24($4)
    add $3, $3, $1
    sw $3, 0($2)
    add $4, $4, $6
    bne $4, $30, -5
    lis $4
    .word 0x0A
    sw $4, 0($2)
    lw $1, -4($30)
    lw $2, -8($30)
    lw $3, -12($30)
    lw $4, -16($30)
    lw $5, -20($30)
    lw $6, -24($30)
    jr $31
