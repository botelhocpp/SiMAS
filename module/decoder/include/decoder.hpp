// Copyright (c) 2024 SiMAS
// All rights reserved

#pragma once

#include <string>
#include <vector>
#include <map>

#include "module/instruction/include/instruction.hpp"

namespace simas {
namespace decoder {

uint32_t DecodeRegister(const std::string& reg);

int32_t DecodeImmediate(const std::string& imm, uint32_t size_bits);

uint32_t DecodeInstruction(const std::vector<std::string>& instruction_elements, const std::map<uint32_t, std::string>& program_labels);

}  // namespace decoder
}  // namespace simas
