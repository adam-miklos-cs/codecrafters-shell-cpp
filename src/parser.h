#ifndef PARSER_H_
#define PARSER_H_

#include <string_view>
#include <vector>

#include "simple_command.h"

SimpleCommand ParseSimpleCommand(const std::vector<std::string_view>& tokens);

#endif  // PARSER_H_