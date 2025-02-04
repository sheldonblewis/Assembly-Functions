lis $1
.word 0x1000
add $6, $0, $0
lis $4
.word 4
lis $5
.word 0xffffffff
lis $2
.word 0xffff0004
lw $3, 0($2)
beq $3, $5, 4
sw $3, 0($1)
add $1, $1, $4
add $6, $6, $4
beq $0, $0, -6
sub $1, $1, $4
lis $2
.word 0xffff000c
lw $3, 0($1)
sw $3, 0($2)
sub $1, $1, $4
sub $6, $6, $4
bne $6, $0, -5
jr $31
