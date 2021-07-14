.globl main
.globl mark

.data
m1: .word 6
m2: .word 5
m3: .word 0

.text
main:
	la $t0, m1
	lw $t3, 0($t0)
	la $t0, m2
	lw $t4, 0($t0)
	li $t0, 1
mark:
	bltz $t1, lab
	li $t2, 0
lab:
	add $t2, $t2, $t3
	and $t2, $t2, $t3
	sub $t1, $t0, $t4
	addi $t0, $t0, 1
	nop
	la $t1, m3
	bltz $t1, mark
	sge $t0, $t0, $t1
	sw $t2, 0($t1)
	beq $t4, $t4, roki
roki:
	nop
