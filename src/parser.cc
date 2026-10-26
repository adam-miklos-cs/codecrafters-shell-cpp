#include "parser.h"

#include <cassert>
#include <optional>
#include <string_view>
#include <utility>

#include "simple_command.h"

namespace {

std::optional<StreamRedirect> ParseRedirect(std::string_view token) {
  if (token == ">" || token == "1>") {
    return StreamRedirect{.target_fd = 1, .append = false};
  }
  if (token == ">>" || token == "1>>") {
    return StreamRedirect{.target_fd = 1, .append = true};
  }
  if (token == "2>") {
    return StreamRedirect{.target_fd = 2, .append = false};
  }
  if (token == "2>>") {
    return StreamRedirect{.target_fd = 2, .append = true};
  }
  return std::nullopt;
}

}  // namespace

SimpleCommand ParseSimpleCommand(std::vector<std::string> tokens) {
  assert(!tokens.empty() &&
         "Tokens cannot be empty when parsing a simple command.");

  SimpleCommand command;
  command.name = std::move(tokens[0]);
  command.arguments.reserve(tokens.size() - 1);

  for (size_t i = 1; i < tokens.size(); ++i) {
    if (auto redirect = ParseRedirect(tokens[i]);
        redirect && i + 1 < tokens.size()) {
      redirect->path = std::move(tokens[++i]);
      command.redirects.push_back(std::move(*redirect));
    } else {
      command.arguments.push_back(std::move(tokens[i]));
    }
  }
  return command;
}
