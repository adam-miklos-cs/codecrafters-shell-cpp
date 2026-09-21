#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "executor.h"
#include "parser.h"
#include "simple_command.h"
#include "tokenizer.h"

int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string input_line;
    if (!std::getline(std::cin, input_line)) {
      break;
    }

    std::vector<std::string_view> tokens = Tokenize(input_line);
    if (tokens.empty()) {
      continue;
    }

    SimpleCommand simple_command = ParseSimpleCommand(tokens);

    ExecutionResult result = Execute(simple_command);
    if (result.should_exit) {
      return result.exit_code;
    }
  }

  return 0;
}
