// Copyright (c) 2024 SiMAS
// All rights reserved

#include "module/parser/include/parser.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "module/decoder/include/decoder.hpp"
#include "module/instruction/include/instruction.hpp"
#include "module/parser/include/parser_exception.hpp"

const uint32_t kInstructionsInitialAddress = 0x0040000;

void simas::parser::ParseInstructions(std::ifstream &input_file, std::ofstream &output_file, bool print_output) {
  uint32_t address = kInstructionsInitialAddress;
  int input_file_line = 0;
  std::string file_line;
  std::vector<std::string> instruction_elements;
  std::vector<uint32_t> instructions_binaries;
  while (std::getline(input_file, file_line)) {
    try {
      input_file_line += 1;

      std::string instruction = file_line;

      simas::instruction::CleanInstruction(instruction);

      if (instruction.empty()) {
        continue;
      }

      instruction_elements.push_back(std::to_string(address));
      simas::instruction::SplitInstruction(instruction, instruction_elements);
      uint32_t instruction_binary = simas::decoder::DecodeInstruction(instruction_elements);
      instruction_elements.clear();

      if(print_output) {
        printf("%04d: %-30s : 0x%08X: 0x%08X\n", (address - kInstructionsInitialAddress)/4, file_line.c_str(), address, instruction_binary);
      }
      address += 4;

      instructions_binaries.push_back(instruction_binary);
    } catch (std::exception &e) {
      throw ParserException(e.what(), input_file_line);
    }
  }

  for (auto const &binary : instructions_binaries) {
    output_file << std::setfill('0') << std::setw(8) << std::hex << binary << "\n";
  }
}
