// Copyright (c) 2024 SiMAS
// All rights reserved

#include "module/parser/include/parser.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "module/decoder/include/decoder.hpp"
#include "module/instruction/include/instruction.hpp"
#include "module/parser/include/parser_exception.hpp"

const uint32_t kInstructionsInitialAddress = 0x00400000;

void simas::parser::PreParseFile(std::ifstream &input_file, std::map<int, std::vector<std::string>> &instructions, std::map<uint32_t, std::string> &labels) {
  uint32_t address = kInstructionsInitialAddress;

  std::string file_line;
  int input_file_line = 0;
  while (std::getline(input_file, file_line)) {
    try {
      input_file_line += 1;

      std::string instruction = file_line;

      simas::instruction::CleanInstruction(instruction);

      if (instruction.empty()) {
        continue;
      } else if (instruction.back() == ':') {
        instruction.pop_back();
        labels[address] = instruction;
        continue;
      }

      std::vector<std::string> instruction_elements;
      instruction_elements.push_back(std::to_string(address));
      simas::instruction::SplitInstruction(instruction, instruction_elements);
      instructions[input_file_line] = instruction_elements;

      address += 4;
    } catch (std::exception &e) {
      throw ParserException(e.what(), input_file_line);
    }
  }
}

void simas::parser::ParseInstructions(std::ofstream &output_file, std::map<int, std::vector<std::string>> &instructions, std::map<uint32_t, std::string> &labels, bool print_output) {
  std::vector<uint32_t> instructions_binaries;
  std::for_each(instructions.begin(), instructions.end(), [&](auto const &instruction) {
    try {
      uint32_t instruction_binary = simas::decoder::DecodeInstruction(instruction.second, labels);
      instructions_binaries.push_back(instruction_binary);
      output_file << std::setfill('0') << std::setw(8) << std::hex << instruction_binary << "\n";
    } catch (std::exception &e) {
      throw ParserException(e.what(), instruction.first);
    }
  });
}
