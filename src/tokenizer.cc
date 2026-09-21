#include "tokenizer.h"

std::vector<std::string_view> Tokenize(std::string_view input) {
  std::vector<std::string_view> tokens;
  const char* whitespace = " \t\r\n";

  size_t start = input.find_first_not_of(whitespace);
  while (start != std::string_view::npos) {
    size_t end = input.find_first_of(whitespace, start);
    tokens.push_back(input.substr(start, end - start));
    start = input.find_first_not_of(whitespace, end);
  }

  return tokens;
}