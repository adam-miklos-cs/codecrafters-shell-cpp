#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

#include "simple_command.h"

SimpleCommand ParseSimpleCommand(std::vector<std::string> tokens);

#endif  // PARSER_H_
