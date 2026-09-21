#ifndef SIMPLE_COMMAND_H_
#define SIMPLE_COMMAND_H_

#include <string>
#include <vector>

struct SimpleCommand {
  std::string name;
  std::vector<std::string> arguments;
};

#endif  // SIMPLE_COMMAND_H_