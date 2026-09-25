#include "tokenizer.h"

std::vector<std::string_view> Split(std::string_view input,
                                    const char *delimiters) {
  std::vector<std::string_view> tokens;

  size_t start = input.find_first_not_of(delimiters);

  while (start != std::string_view::npos) {
    size_t end = input.find_first_of(delimiters, start);
    tokens.push_back(input.substr(start, end - start));
    start = input.find_first_not_of(delimiters, end);
  }

  return tokens;
}
