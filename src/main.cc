#include <iostream>
#include <string>
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

    // What can I assume here? This input_line will end at a correct place even with quotes?
    std::string input_line;
    if (!std::getline(std::cin, input_line)) {
      break;
    }

    std::vector<std::string> tokens = SplitWithQuoting(input_line);
    if (tokens.empty()) {
      continue;
    }

    SimpleCommand simple_command = ParseSimpleCommand(std::move(tokens));

    ExecutionResult result = Execute(simple_command);
    if (result.should_exit) {
      return result.exit_code;
    }
  }

  return 0;
}
