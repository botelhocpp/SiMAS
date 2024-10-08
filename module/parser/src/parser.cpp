// Copyright (c) 2024 SiMAS
// All rights reserved

#include "module/parser/include/parser.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "module/decoder/include/decoder.hpp"
#include "module/instruction/include/instruction.hpp"
#include "module/parser/include/parser_exception.hpp"

const uint32_t kInstructionsInitialAddress = 0x00400000;

void simas::parser::ParseInstructions(std::ifstream &input_file, std::ofstream &output_file, bool print_output) {
  uint32_t address = kInstructionsInitialAddress;
  int input_file_line = 0;
  std::string file_line;
  std::vector<std::vector<std::string>> instructions;
  std::vector<std::string> instruction_elements;
  std::map<uint32_t, std::string> program_labels;

  while (std::getline(input_file, file_line)) {
    try {
      input_file_line += 1;

      std::string instruction = file_line;

      simas::instruction::CleanInstruction(instruction);

      if (instruction.empty()) {
        continue;
      }
      else if(instruction.back() == ':') {
        instruction.pop_back();
        program_labels[address] = instruction;
        continue;
      }
      
      instruction_elements.push_back(std::to_string(address));
      simas::instruction::SplitInstruction(instruction, instruction_elements);
      instructions.push_back(instruction_elements);
      instruction_elements.clear();
      address += 4;
    } catch (std::exception &e) {
      throw ParserException(e.what(), input_file_line);
    }
  }

  std::vector<uint32_t> instructions_binaries;
  std::for_each(instructions.begin(), instructions.end(), [&](auto const& elements) {
    try {
      uint32_t instruction_binary = simas::decoder::DecodeInstruction(elements, program_labels);
      instructions_binaries.push_back(instruction_binary);
    } catch (std::exception &e) {
      std::for_each(elements.begin(), elements.end(), [&](auto const& e) {
        std::cout << e << " ";
      });
      std::cout << "\n";
      throw ParserException(e.what(), input_file_line);
    }
  });

  for (auto const &binary : instructions_binaries) {
    output_file << std::setfill('0') << std::setw(8) << std::hex << binary << "\n";
  }
}
