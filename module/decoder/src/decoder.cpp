// Copyright (c) 2024 SiMAS
// All rights reserved

#include "module/decoder/include/decoder.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <regex>
#include <stdexcept>
#include <unordered_map>

const uint32_t k16bitMask = 0xFFFF;
const uint32_t k26bitMask = 0x3FFFFFF;

const std::unordered_map<std::string, uint32_t> kRegisterMap = {
    {"$zero", 0},
    {"$at", 1},
    {"$v0", 2},
    {"$v1", 3},
    {"$a0", 4},
    {"$a1", 5},
    {"$a2", 6},
    {"$a3", 7},
    {"$t0", 8},
    {"$t1", 9},
    {"$t2", 10},
    {"$t3", 11},
    {"$t4", 12},
    {"$t5", 13},
    {"$t6", 14},
    {"$t7", 15},
    {"$s0", 16},
    {"$s1", 17},
    {"$s2", 18},
    {"$s3", 19},
    {"$s4", 20},
    {"$s5", 21},
    {"$s6", 22},
    {"$s7", 23},
    {"$t8", 24},
    {"$t9", 25},
    {"$k0", 26},
    {"$k1", 27},
    {"$gp", 28},
    {"$sp", 29},
    {"$fp", 30},
    {"$ra", 31}};

uint32_t simas::decoder::DecodeRegister(const std::string& reg) {
  auto it = kRegisterMap.find(reg);
  if (it != kRegisterMap.end()) {
    return it->second;
  }

  std::regex pattern(R"(\$(\d+))");
  std::smatch match;
  uint32_t reg_number = 0;
  if (std::regex_match(reg, match, pattern)) {
    int reg_number = std::stoi(match[1].str());
    if (reg_number > 31) {
      throw std::invalid_argument("Invalid register '" + reg + "'");
    }
  } else {
    throw std::invalid_argument("Invalid register '" + reg + "'");
  }

  return reg_number;
}

int32_t simas::decoder::DecodeImmediate(const std::string& imm, uint32_t size_bits) {
  int32_t imm_val = 0;

  try {
    if (std::strncmp(imm.c_str(), "0x", 2) == 0) {
      imm_val = std::stoi(imm, nullptr, 16);
    } else if (std::strncmp(imm.c_str(), "0o", 2) == 0) {
      imm_val = std::stoi(imm, nullptr, 8);
    } else if (std::strncmp(imm.c_str(), "0b", 2) == 0) {
      imm_val = std::stoi(imm, nullptr, 2);
    } else {
      imm_val = std::stoi(imm, nullptr, 10);
    }
  } catch (...) {
    throw std::invalid_argument("Invalid immediate '" + imm + "'");
  }

  const int32_t imm_max_val = std::pow(2, size_bits) - 1;
  if (imm_max_val < imm_val) {
    throw std::invalid_argument("Immediate exceeds its " + std::to_string(size_bits) + "-bit boundaries");
  }

  return imm_val & imm_max_val;
}

uint32_t simas::decoder::DecodeLabel(const std::string& label, const std::map<uint32_t, std::string>& program_labels) {
  auto it = std::find_if(program_labels.cbegin(), program_labels.cend(), [&](const auto& lbl) {
    return lbl.second == label;
  });
  if (it == program_labels.cend()) {
    throw std::invalid_argument("Invalid label '" + label + "'");
  }
  return it->first;
}

uint32_t simas::decoder::CalculateLabelAddress(const std::string& label, const std::string& current_address, const std::map<uint32_t, std::string>& program_labels, bool is_jump) {
  uint32_t address = 0;
  uint32_t mask = is_jump? k26bitMask : k16bitMask;

  if (label == ".") {
    address = std::stoi(current_address, nullptr, 10);
  } else {
    address = DecodeLabel(label, program_labels);
  }
  
  if(!is_jump) {
    address = address - std::stoi(current_address) - 4;
  }

  return (address >> 2) & mask;
}

uint32_t simas::decoder::DecodeInstruction(const std::vector<std::string>& instruction_elements, const std::map<uint32_t, std::string>& program_labels) {
  const instruction::Instruction* instruction_ptr = instruction::GetInstruction(instruction_elements.at(1));

  uint32_t number_operands = instruction_elements.size() - 2;

  if (instruction_ptr->number_operands != number_operands) {
    throw std::invalid_argument("Invalid instruction format. Expected " +
                                std::to_string(instruction_ptr->number_operands) + " operands. " +
                                std::to_string(number_operands) + " were informed");
  }

  uint32_t instruction_binary = instruction_ptr->binary_mask;

  switch (instruction_ptr->type) {
    case instruction::InstructionType::kTypeR:
      if (instruction_ptr->mnemonic == "jalr" || instruction_ptr->mnemonic == "jr") {
        instruction_binary |= DecodeRegister(instruction_elements.at(2)) << 21;  // rs
      } else {
        instruction_binary |= DecodeRegister(instruction_elements.at(2)) << 11;  // rd

        bool is_shift_shamt = instruction_ptr->mnemonic == "sll" || instruction_ptr->mnemonic == "sra" || instruction_ptr->mnemonic == "srl";
        bool is_shift_value = instruction_ptr->mnemonic == "sllv" || instruction_ptr->mnemonic == "srav" || instruction_ptr->mnemonic == "srlv";
        
        if(is_shift_shamt || is_shift_value) {
          instruction_binary |= DecodeRegister(instruction_elements.at(3)) << 16;  // rt

          if(is_shift_shamt) {
            instruction_binary |= DecodeImmediate(instruction_elements.at(4), 5) << 6;  // shamt
          }
          else {
            instruction_binary |= DecodeRegister(instruction_elements.at(4)) << 21;  // rs
          }
        }
        else {
          instruction_binary |= DecodeRegister(instruction_elements.at(3)) << 21;  // rs
          instruction_binary |= DecodeRegister(instruction_elements.at(4)) << 16;  // rt
        }
      }
      break;

    case instruction::InstructionType::kTypeI:
      if (instruction_ptr->mnemonic != "lui") {
        instruction_binary |= DecodeRegister(instruction_elements.at(3)) << 21;  // rs
      }

      if (instruction_ptr->mnemonic == "bgtz" || instruction_ptr->mnemonic == "blez") {
        instruction_binary |= CalculateLabelAddress(instruction_elements.at(3), instruction_elements.at(0), program_labels, false);
      } else {
        instruction_binary |= DecodeRegister(instruction_elements.at(2)) << 16;  // rt

        if (instruction_ptr->mnemonic == "beq" || instruction_ptr->mnemonic == "bne") {
          instruction_binary |= CalculateLabelAddress(instruction_elements.at(4), instruction_elements.at(0), program_labels, false);
        } else {
          instruction_binary |= DecodeImmediate(instruction_elements.back(), 16);
        }
      }

      break;

    case instruction::InstructionType::kTypeJ:
      instruction_binary |= CalculateLabelAddress(instruction_elements.at(2), instruction_elements.at(0), program_labels, true);
      break;
  }

  return instruction_binary;
}
