.globl main

.data
m111: .word 55
m222: .word 10

.text
main:
	la $t2, m222
	lw $t1, 0($t2)
	li $t2, 1
	li $t0, 0
	li $t3, 0
loop:
	add $t3, $t3, $t1
decreasen:
	sub $t1, $t1, $t2
	beq $t1, $t0, checkresult
	b loop
checkresult:
	nop
	nop
	nop
	li $t1, 0
	la $t0, m111
	lw $t0, 0($t0)
	beq $t3, $t0, sveok
jbgnevalja:
	li $t1, 1
sveok:
	and $t1, $t1, $t2
	nop
	nop
	nop
