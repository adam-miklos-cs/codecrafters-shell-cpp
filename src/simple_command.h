#ifndef SIMPLE_COMMAND_H_
#define SIMPLE_COMMAND_H_

#include <string>
#include <vector>

struct StreamRedirect {
  int target_fd;
  std::string path;
  bool append = false;
};

struct SimpleCommand {
  std::string name;
  std::vector<std::string> arguments;
  std::vector<StreamRedirect> redirects;
};

#endif  // SIMPLE_COMMAND_H_
