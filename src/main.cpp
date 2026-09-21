#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class SimpleCommand
{
private:
  std::string name_;
  std::vector<std::string> arguments_;

public:
  static SimpleCommand tokenize(const std::string_view command_string)
  {
    SimpleCommand simple_command;
    // TODO: Check how this works step by step.
    const char *whitespace = " \t\r\n";

    // Skip leading whitespace.
    size_t start = command_string.find_first_not_of(whitespace);

    // Empty command.
    if (start == std::string_view::npos)
      return simple_command;

    size_t end = command_string.find_first_of(whitespace, start);
    simple_command.name_ = command_string.substr(start, end - start);

    // Command without arguments.
    if (end == std::string_view::npos)
      return simple_command;

    while ((start = command_string.find_first_not_of(whitespace, end)) !=
           std::string_view::npos)
    {
      end = command_string.find_first_of(whitespace, start);
      simple_command.arguments_.emplace_back(command_string.substr(start, end - start));
      if (end == std::string_view::npos)
        break;
    }

    return simple_command;
  }

  std::string_view name() const
  {
    return name_;
  }

  const std::vector<std::string> &arguments() const
  {
    return arguments_;
  }
};

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true)
  {
    std::cout << "$ ";

    // Read the command string from the user.
    std::string command_string;
    std::getline(std::cin, command_string);

    // Parse the command string into a SimpleCommand object.
    SimpleCommand simple_command = SimpleCommand::tokenize(command_string);

    if (simple_command.name().compare("exit") == 0)
    {
      break;
    }
    else if (simple_command.name().compare("echo") == 0)
    {
      bool first = true;
      for (const std::string &argument : simple_command.arguments())
      {
        if (!first)
        {
          std::cout << " ";
        }
        std::cout << argument;
        first = false;
      }
      std::cout << std::endl;
    }
    else
    {
      std::cout << simple_command.name() << ": command not found" << std::endl;
    }
  }
}
