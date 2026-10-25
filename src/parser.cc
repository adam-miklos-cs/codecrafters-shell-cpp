#include "parser.h"

#include <cassert>
#include <utility>

SimpleCommand ParseSimpleCommand(std::vector<std::string> tokens) {
  assert(!tokens.empty() && "Tokens cannot be empty when parsing a simple command.");

  SimpleCommand command;
  command.name = std::move(tokens[0]);
  command.arguments.reserve(tokens.size() - 1);
  for (size_t i = 1; i < tokens.size(); ++i) {
    command.arguments.push_back(std::move(tokens[i]));
  }
  return command;
}
