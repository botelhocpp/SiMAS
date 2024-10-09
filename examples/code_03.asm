# Copyright (c) 2024 SiMAS
# All rights reserved

main:
addi $s0, $zero, 0x1001
addi $t0, $zero, 16
sllv $s0, $s0, $t0
lw $a0, 0($s0)
lw $a1, 4($s0)
bne $a0, $a1, goto_here

wait_here:
j .

goto_here:
jal sum
sw $a0, 8($s0)
j wait_here

# int sum(int, int)
sum:
#push $ra
addi $sp, $sp, -4
sw $ra, 0($sp)

add $a0, $a0, $a1

#push $ra
lw $ra, 0($sp)
addi $sp, $sp, 4
jr $ra
