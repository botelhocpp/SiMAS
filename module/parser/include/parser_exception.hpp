// Copyright (c) 2024 SiMAS
// All rights reserved

#pragma once

#include <stdexcept>

namespace simas {
namespace parser {

class ParserException : private std::exception {
 public:
  ParserException(const std::string& message, int line) noexcept;
  virtual ~ParserException() = default;
  virtual char const* what() const noexcept;
  int GetLine() const noexcept;

 private:
  std::string message;
  int line;
};

}  // namespace parser
}  // namespace simas
