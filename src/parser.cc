#include "parser.h"

#include <cassert>

SimpleCommand ParseSimpleCommand(const std::vector<std::string_view>& tokens) {
  assert(!tokens.empty() && "Tokens cannot be empty when parsing a simple command.");

  SimpleCommand command;
  command.name = std::string(tokens[0]);
  for (size_t i = 1; i < tokens.size(); ++i) {
    command.arguments.emplace_back(tokens[i]);
  }
  return command;
}