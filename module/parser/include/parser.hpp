// Copyright (c) 2024 SiMAS
// All rights reserved

#pragma once

#include <fstream>

namespace simas {
namespace parser {

void ParseInstructions(std::ifstream& input_file, std::ofstream& output_file, bool print_output = true);

}
}  // namespace simas
