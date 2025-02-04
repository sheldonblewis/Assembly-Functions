lis $4
.word 1
lis $5
.word 4
lis $6
.word compare
lis $30
.word 0x1000
for:
    sw $1, 4($30)
    sw $2, 8($30)
    lw $7, 4($30)
    lw $1, 0($7)
    lw $2, 4($7)
    sw $31, 12($30)
    jalr $6
    lw $31, 12($30)
    lw $1, 4($30)
    lw $2, 8($30)
    bne $3, $4, exit
    add $1, $1, $5
    sub $2, $2, $4
    bne $2, $4, for
exit:
    jr $31
