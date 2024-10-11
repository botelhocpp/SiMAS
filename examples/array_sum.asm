_start:
lui $t0, 0x1001
addi $t1, $zero, 5
xor $t2, $t2, $t2

_start_fill_array:
sw $t2, 0($t0)
addi $t2, $t2, 1
addi $t0, $t0, 4
bne $t2, $t1, _start_fill_array
jal main
j .

main:
addi $sp, $sp, -4
sw $ra, 0($sp)

lui $a0, 0x1001
addi $a1, $zero, 5
jal sum_array

lui $t0, 0x1001
sw $a0, 20($t0)

lw $ra, 0($sp)
addi $sp, $sp, 4
xor $a0, $a0, $a0
jr $ra

sum_array:
addi $sp, $sp, -4
sw $ra, 0($sp)

add $t0, $zero, $a0
xor $a0, $a0, $a0

sum_array_loop:
lw $t1, 0($t0)
add $a0, $a0, $t1
addi $t0, $t0, 4
addi $a1, $a1, -1
bgtz $a1, sum_array_loop

lw $ra, 0($sp)
addi $sp, $sp, 4
jr $ra
