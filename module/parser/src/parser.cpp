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

std::vector<std::string> simas::parser::ReadFileToVector(std::ifstream &input_file) {
  std::vector<std::string> file_contents;
  
  std::string file_line;
  while (std::getline(input_file, file_line)) {
    file_contents.push_back(file_line);
  }

  return file_contents;
}

void simas::parser::PreParseFile(std::vector<std::string>& file_contents, std::map<int, std::vector<std::string>> &instructions, std::map<uint32_t, std::string> &labels) {
  uint32_t address = kInstructionsInitialAddress;

  for (int line = 0; line < file_contents.size(); line++) {
    try {
      std::string instruction = file_contents.at(line);

      simas::instruction::CleanInstruction(instruction);

      if (instruction.empty()) {
        continue;
      } else if (instruction.back() == ':') {
        instruction.pop_back();
        
        auto it = std::find_if(labels.cbegin(), labels.cend(), [&](auto const &label) {
          return label.second == instruction;
        });

        if(it != labels.cend()) {
          throw std::invalid_argument("Duplicated label '" + it->second + "'");
        }

        labels[address] = instruction;
        continue;
      }

      std::vector<std::string> instruction_elements;
      instruction_elements.push_back(std::to_string(address));
      simas::instruction::SplitInstruction(instruction, instruction_elements);
      instructions[line + 1] = instruction_elements;

      address += 4;
    } catch (std::exception &e) {
      throw ParserException(e.what(), line + 1);
    }
  }
}

void simas::parser::ParseInstructions(std::ofstream &output_file, std::vector<std::string>& file_contents, std::map<int, std::vector<std::string>> &instructions, std::map<uint32_t, std::string> &labels, bool print_output) {
  std::vector<uint32_t> instructions_binaries;
  uint32_t current_line = 0;
  std::for_each(instructions.begin(), instructions.end(), [&](auto const &instruction) {
    try {
      uint32_t instruction_binary = simas::decoder::DecodeInstruction(instruction.second, labels);
      instructions_binaries.push_back(instruction_binary);
      output_file << std::setfill('0') << std::setw(8) << std::hex << instruction_binary << "\n";
      
      if(print_output) {
        const uint32_t address = std::stoi(instruction.second.front());
        std::printf("%04d: %-30s : 0x%08X: 0x%08X\n", (address - kInstructionsInitialAddress)/4, file_contents.at(instruction.first - 1).c_str(), address, instruction_binary);
      }
    } catch (std::exception &e) {
      throw ParserException(e.what(), instruction.first);
    }
  });
}
