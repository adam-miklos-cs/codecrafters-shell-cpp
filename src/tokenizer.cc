#include "tokenizer.h"

#include <utility>

std::vector<std::string_view> Split(std::string_view input,
                                    const char* delimiters) {
  std::vector<std::string_view> tokens;

  size_t start = input.find_first_not_of(delimiters);

  while (start != std::string_view::npos) {
    size_t end = input.find_first_of(delimiters, start);
    tokens.push_back(input.substr(start, end - start));
    start = input.find_first_not_of(delimiters, end);
  }

  return tokens;
}

std::vector<std::string> SplitWithShellBehaviour(std::string_view input) {
  std::vector<std::string> tokens;

  bool single_quote = false;
  bool double_quote = false;
  bool backslash = false;

  std::string current_token;

  for (char c : input) {
    if (!single_quote && !double_quote) {
      if (backslash) {
        current_token += c;
        backslash = false;
        continue;
      }
      switch (c) {
        case '\\':
          backslash = true;
          break;
        case '\'':
          single_quote = true;
          break;
        case '\"':
          double_quote = true;
          break;
        case ' ':
          if (!current_token.empty()) {
            tokens.push_back(std::move(current_token));
            current_token.clear();
          }
          break;
        default:
          current_token += c;
          break;
      }
    } else if (single_quote) {
      if (c == '\'') {
        single_quote = false;
      } else {
        current_token += c;
      }
    } else if (double_quote) {
      if (backslash) {
        if (c != '\\' && c != '\"') {
          current_token += '\\';
          break;
        }
        current_token += c;
        backslash = false;
        continue;
      }
      switch (c) {
        case '\\':
          backslash = true;
          break;
        case '\"':
          double_quote = false;
          break;
        default:
          current_token += c;
          break;
      }
    }
  }

  if (!current_token.empty()) {
    tokens.push_back(std::move(current_token));
  }

  return tokens;
}
