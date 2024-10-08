// Copyright (c) 2024 SiMAS
// All rights reserved

#include "module/instruction/include/instruction.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "module/instruction/include/instructionlist.hpp"
#include "utils/strmanip/include/strmanip.hpp"

constexpr char kCommentSymbol[] = "#";

void simas::instruction::CleanInstruction(std::string& line) {
  strmanip::RemoveComma(line);
  strmanip::RightTrim(line);
  strmanip::LeftTrim(line);
  strmanip::MiddleTrim(line);
  strmanip::RemoveSubstring(line, kCommentSymbol);
}

void simas::instruction::SplitInstruction(const std::string& line, std::vector<std::string>& instruction_elements) {
  std::stringstream line_stream(line);

  std::string token;
  while (std::getline(line_stream, token, ' ')) {
    instruction_elements.push_back(token);
  }

  if (instruction_elements.at(1) == "sw" || instruction_elements.at(1) == "lw") {
    char reg[10];
    char imm[10];

    sscanf(instruction_elements.at(3).c_str(), "%[^(](%[^)])", imm, reg);

    instruction_elements.at(3) = reg;
    instruction_elements.push_back(imm);
  }
}

const simas::instruction::Instruction* simas::instruction::GetInstruction(const std::string& mnemonic) {
  const Instruction* instruction = std::find_if(std::begin(instructions::kInstructionList), std::end(instructions::kInstructionList),
                                                [&](const auto& instruction) {
                                                  return mnemonic == instruction.mnemonic;
                                                });

  if (!instruction) {
    throw std::invalid_argument("Invalid mnemonic");
  }

  return instruction;
}
