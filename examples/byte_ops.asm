# Copyright (c) 2024 SiMAS
# All rights reserved

main:
lui $s0, 0x1001
lb $a0, 0($s0)
lbu $a1, 4($s0)
add $a0, $a0, $a1
sb $a0, 8($s0)
j .