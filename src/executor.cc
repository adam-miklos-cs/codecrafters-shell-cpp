#include "executor.h"

#include "tokenizer.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

namespace {

using BuiltinHandler = ExecutionResult (*)(const SimpleCommand &);

// Forward declarations of individual builtin handlers.
ExecutionResult HandleExit(const SimpleCommand &command);
ExecutionResult HandleEcho(const SimpleCommand &command);
ExecutionResult HandleType(const SimpleCommand &command);

// The Builtin Registry (Dispatch Table).
const std::unordered_map<std::string, BuiltinHandler> &GetBuiltins() {
  static const auto *builtins =
      new std::unordered_map<std::string, BuiltinHandler>{
          {"exit", HandleExit},
          {"echo", HandleEcho},
          {"type", HandleType},
      };
  return *builtins;
}

// Utility functions.
bool IsExecutableFile(const fs::path &path, std::error_code &ec) {
  if (!fs::is_regular_file(path, ec)) {
    return false;
  }
  auto perms = fs::status(path, ec).permissions();
  return (perms & (fs::perms::owner_exec | fs::perms::group_exec |
                   fs::perms::others_exec)) != fs::perms::none;
}

std::optional<fs::path> FindExecutable(std::string_view command_name) {
  const char *path_env = std::getenv("PATH");
  if (path_env == nullptr) {
    return std::nullopt;
  }

  std::error_code ec;
  for (const auto &dir : Split(path_env, ":")) {
    fs::path full_path = fs::path(dir) / command_name;
    if (IsExecutableFile(full_path, ec)) {
      return full_path;
    }
  }

  return std::nullopt;
}

//// Handler implementations.
ExecutionResult HandleExit(const SimpleCommand &command) {
  int exit_code = 0;
  if (!command.arguments.empty()) {
    try {
      exit_code = std::stoi(command.arguments[0]);
    } catch (...) {
      exit_code = 0;
    }
  }
  return ExecutionResult{.should_exit = true, .exit_code = exit_code};
}

ExecutionResult HandleEcho(const SimpleCommand &command) {
  for (size_t i = 0; i < command.arguments.size(); ++i) {
    if (i > 0)
      std::cout << " ";
    std::cout << command.arguments[i];
  }
  std::cout << "\n";
  return ExecutionResult{.should_exit = false, .exit_code = 0};
}

ExecutionResult HandleType(const SimpleCommand &command) {
  if (command.arguments.empty()) {
    return ExecutionResult{.should_exit = false, .exit_code = 1};
  }

  const std::string &target = command.arguments[0];
  const auto &builtins = GetBuiltins();

  if (builtins.contains(target)) {
    std::cout << target << " is a shell builtin\n";
    return ExecutionResult{.should_exit = false, .exit_code = 0};
  }

  if (auto path = FindExecutable(target)) {
    std::cout << target << " is " << path->string() << "\n";
    return ExecutionResult{.should_exit = false, .exit_code = 0};
  }

  std::cout << target << ": not found\n";
  return ExecutionResult{.should_exit = false, .exit_code = 1};
}

ExecutionResult ExecuteExternal(const fs::path &path,
                                const SimpleCommand &command) {
  // Build null-terminated argv array: [command_name, arg1, arg2, ..., nullptr].
  std::vector<char *> args;
  args.reserve(command.arguments.size() + 2);
  args.push_back(const_cast<char *>(command.name.c_str()));
  for (const auto &arg : command.arguments) {
    args.push_back(const_cast<char *>(arg.c_str()));
  }
  args.push_back(nullptr);

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return ExecutionResult{.should_exit = false, .exit_code = 1};
  }

  if (pid == 0) {
    // Child: replace process image with target binary.
    execv(path.c_str(), args.data());
    // Only reached if execv fails.
    perror("execv");
    std::exit(1);
  }

  // Parent: wait for child to finish and extract exit code.
  int status = 0;
  waitpid(pid, &status, 0);

  int exit_code = 0;
  if (WIFEXITED(status)) {
    exit_code = WEXITSTATUS(status);
  }

  return ExecutionResult{.should_exit = false, .exit_code = exit_code};
}

} // namespace

ExecutionResult Execute(const SimpleCommand &command) {
  const auto &builtins = GetBuiltins();

  auto it = builtins.find(command.name);
  if (it != builtins.end()) {
    return it->second(command);
  }

  if (auto path = FindExecutable(command.name)) {
    return ExecuteExternal(*path, command);
  }

  std::cout << command.name << ": command not found\n";
  return ExecutionResult{.should_exit = false, .exit_code = 127};
}
