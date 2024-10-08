// Copyright (c) 2024 SiMAS
// All rights reserved

#include "module/parser/include/parser_exception.hpp"

simas::parser::ParserException::ParserException(const std::string& message, int line) noexcept
    : std::exception(), message(message), line(line) {}

char const* simas::parser::ParserException::what() const noexcept {
  return message.c_str();
}

int simas::parser::ParserException::GetLine() const noexcept {
  return line;
}
