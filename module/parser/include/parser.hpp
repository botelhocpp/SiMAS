// Copyright (c) 2024 SiMAS
// All rights reserved

#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace simas {
namespace parser {

void PreParseFile(std::ifstream& input_file, std::map<int, std::vector<std::string>>& instructions, std::map<uint32_t, std::string>& labels);

void ParseInstructions(std::ofstream& output_file, std::map<int, std::vector<std::string>>& instructions, std::map<uint32_t, std::string>& labels, bool print_output = true);

}  // namespace parser
}  // namespace simas
