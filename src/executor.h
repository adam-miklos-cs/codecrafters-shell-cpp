#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include "simple_command.h"

struct ExecutionResult {
  bool should_exit = false;
  int exit_code = 0;
};

ExecutionResult Execute(const SimpleCommand& command);

#endif  // EXECUTOR_H_