# Copyright (c) 2024 SiMAS
# All rights reserved

main:
lui $s0, 0x1001
lw $a0, 0($s0)
lw $a1, 4($s0)
bne $a0, $a1, goto_here

wait_here:
j .

goto_here:
jal func
sw $a0, 8($s0)
j wait_here

# int func(int, int)
func:
#push $ra
addi $sp, $sp, -4
sw $ra, 0($sp)

sll $a0, $a0, 1
sll $a1, $a1, 1
add $a0, $a0, $a1
srl $a0, $a0, 1
andi $a0, $a0, 0xFF

#push $ra
lw $ra, 0($sp)
addi $sp, $sp, 4
jr $ra
