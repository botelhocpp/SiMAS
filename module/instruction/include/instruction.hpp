// Copyright (c) 2024 SiMAS
// All rights reserved

#pragma once

#include <string>
#include <vector>

namespace simas {
namespace instruction {

enum class InstructionType {
    kTypeR,
    kTypeI,
    kTypeJ
};

struct Instruction {
    std::string mnemonic;
    uint32_t binary_mask;
    int number_operands;
    InstructionType type;
};

const Instruction* GetInstruction(const std::string& mnemonic);

void SplitInstruction(const std::string& line, std::vector<std::string>& instruction_elements);

void CleanInstruction(std::string& line);

}  // namespace instruction
}  // namespace simas
