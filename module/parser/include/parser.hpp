// Copyright (c) 2024 SiMAS
// All rights reserved

#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace simas {
namespace parser {

std::vector<std::string> ReadFileToVector(std::ifstream &input_file);

void PreParseFile(std::vector<std::string>& file_contents, std::map<int, std::vector<std::string>>& instructions, std::map<uint32_t, std::string>& labels);

void ParseInstructions(std::ofstream& output_file, std::vector<std::string>& file_contents, std::map<int, std::vector<std::string>>& instructions, std::map<uint32_t, std::string>& labels, bool print_output = true);

}  // namespace parser
}  // namespace simas
