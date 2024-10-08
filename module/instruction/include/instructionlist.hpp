// Copyright (c) 2024 SiMAS
// All rights reserved

#pragma once

#include "module/instruction/include/instruction.hpp"

namespace simas {
namespace instructions {

const instruction::Instruction kInstructionList[] = {
    {"sllv", 0x00000004, 3, instruction::InstructionType::kTypeR},
    {"srlv", 0x00000006, 3, instruction::InstructionType::kTypeR},
    {"srav", 0x00000007, 3, instruction::InstructionType::kTypeR},
    {"jr",   0x00000008, 1, instruction::InstructionType::kTypeR},
    {"jalr", 0x00000009, 1, instruction::InstructionType::kTypeR},
    {"add",  0x00000020, 3, instruction::InstructionType::kTypeR},
    {"sub",  0x00000022, 3, instruction::InstructionType::kTypeR},
    {"and",  0x00000024, 3, instruction::InstructionType::kTypeR},
    {"or",   0x00000025, 3, instruction::InstructionType::kTypeR},
    {"xor",  0x00000026, 3, instruction::InstructionType::kTypeR},
    {"nor",  0x00000027, 3, instruction::InstructionType::kTypeR},
    {"slt",  0x0000002A, 3, instruction::InstructionType::kTypeR},
    {"addi", 0x20000000, 3, instruction::InstructionType::kTypeI},
    {"lw",   0x8C000000, 3, instruction::InstructionType::kTypeI},
    {"sw",   0xAC000000, 3, instruction::InstructionType::kTypeI},
    {"beq",  0x10000000, 3, instruction::InstructionType::kTypeI},
    {"bne",  0x14000000, 3, instruction::InstructionType::kTypeI},
    {"j",    0x08000000, 1, instruction::InstructionType::kTypeJ},
    {"jal",  0x0C000000, 1, instruction::InstructionType::kTypeJ}
};

}
}  // namespace simas
