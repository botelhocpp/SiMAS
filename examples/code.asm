# Copyright (c) 2024 SiMAS
# All rights reserved

# main
addi $s0, $zero, 0x1001
addi $t0, $zero, 16
sllv $s0, $s0, $t0
lw $s1, 0($s0)
lw $s2, 4($s0)
bne $s1, $s2, 1

# wait here
j .

# goto here
sw $s1, 8($s0)
j 24
