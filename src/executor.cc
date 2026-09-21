#include "executor.h"

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace
{

    using BuiltinHandler = ExecutionResult (*)(const SimpleCommand &);

    // Forward declarations of individual builtin handlers.
    ExecutionResult HandleExit(const SimpleCommand &command);
    ExecutionResult HandleEcho(const SimpleCommand &command);
    ExecutionResult HandleType(const SimpleCommand &command);

    // The Builtin Registry (Dispatch Table).
    const std::unordered_map<std::string, BuiltinHandler> &GetBuiltins()
    {
        static const auto *builtins = new std::unordered_map<std::string,
                                                             BuiltinHandler>{
            {"exit", HandleExit},
            {"echo", HandleEcho},
            {"type", HandleType},
        };
        return *builtins;
    }

    // Handler implementations.
    ExecutionResult HandleExit(const SimpleCommand &command)
    {
        int exit_code = 0;
        if (!command.arguments.empty())
        {
            try
            {
                exit_code = std::stoi(command.arguments[0]);
            }
            catch (...)
            {
                exit_code = 0;
            }
        }
        return ExecutionResult{.should_exit = true, .exit_code = exit_code};
    }

    ExecutionResult HandleEcho(const SimpleCommand &command)
    {
        for (size_t i = 0; i < command.arguments.size(); ++i)
        {
            if (i > 0)
                std::cout << " ";
            std::cout << command.arguments[i];
        }
        std::cout << "\n";
        return ExecutionResult{.should_exit = false, .exit_code = 0};
    }

    ExecutionResult HandleType(const SimpleCommand &command)
    {
        if (command.arguments.empty())
        {
            return ExecutionResult{.should_exit = false, .exit_code = 1};
        }

        const std::string &target = command.arguments[0];
        const auto &builtins = GetBuiltins();

        if (builtins.contains(target))
        {
            std::cout << target << " is a shell builtin\n";
            return ExecutionResult{.should_exit = false, .exit_code = 0};
        }

        std::cout << target << ": not found\n";
        return ExecutionResult{.should_exit = false, .exit_code = 1};
    }

} // End of the anonymus namespace.

ExecutionResult Execute(const SimpleCommand &command)
{
    const auto &builtins = GetBuiltins();

    auto it = builtins.find(command.name);
    if (it != builtins.end())
    {
        return it->second(command);
    }

    std::cout << command.name << ": command not found\n";
    return ExecutionResult{.should_exit = false, .exit_code = 127};
}